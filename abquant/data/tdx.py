# -*- coding: utf-8 -*-
from __future__ import print_function
from .tdx_api import get_stock_list
from abquant.helper import to_json_from_pandas, INSTRUMENT_TYPE
from abquant.utils.logger import system_log as slog
import pymongo
from abquant.config import Setting
from abquant.utils.qa import now_time
from abquant.utils.tdx import query_stock_day, stock_to_fq, save_error_log
from abquant.utils.tdx_financial import download_financialzip, parse_filelist
from abquant.data.tdx_api import (
    get_stock_day,
    get_index_day,
    get_index_min,
    get_stock_min,
    get_stock_xdxr,
    get_stock_info,
)

from abquant.data.base import ISecurity, ISecurityVisitor
from tqdm.auto import tqdm, trange
from concurrent.futures import ThreadPoolExecutor
from threading import RLock
from functools import partial
import datetime
import pandas as pd
import os


class Stock(ISecurity):
    def __init__(self, *args, **kwargs):
        "docstring"
        self._client = pymongo.MongoClient(Setting.get_mongo())
        self._db = self._client[Setting.DBNAME]
        self.codes = kwargs.get("codes", [])
        self.freqs = kwargs.get("freqs", ["1min", "5min", "15min", "30min", "60min"])

    def accept(self, visitor: ISecurityVisitor) -> None:
        visitor.create_day(self)
        visitor.create_min(self)
        visitor.create_xdxr(self)

    def create_day(self, *args, **kwargs):
        """save INSTRUMENT_TYPE.CS or INSTRUMENT_TYPE.INDX day

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        ins_type = kwargs.pop("ins_type", INSTRUMENT_TYPE.CS)
        instruments = (
            self.codes
            if self.codes
            else (
                get_stock_list(ins_type)
                if ins_type == INSTRUMENT_TYPE.INDX
                else get_stock_list().code.unique().tolist()
            )
        )
        coll = (
            self._db.index_day
            if ins_type == INSTRUMENT_TYPE.INDX
            else self._db.stock_day
        )
        coll.create_index(
            [("code", pymongo.ASCENDING), ("date_stamp", pymongo.ASCENDING),]
        )
        err = []

        def saving_work(idx):
            code = (
                instruments.index[idx][0]
                if ins_type == INSTRUMENT_TYPE.INDX
                else instruments[idx]
            )
            try:
                begin = datetime.datetime.now()
                ref_ = coll.find({"code": str(code)[0:6]})
                end_time = str(now_time())[0:10]
                start_time = (
                    ref_[ref_.count() - 1]["date"] if ref_.count() > 0 else "1990-01-01"
                )
                if start_time == end_time:
                    return
                df = (
                    get_index_day(str(code), start_time, end_time)
                    if ins_type == INSTRUMENT_TYPE.INDX
                    else get_stock_day(str(code), start_time, end_time)
                )
                if df is None or df.empty:
                    slog.warn(
                        "df is empty. {}|{}|{}".format(code, start_time, end_time)
                    )
                    return

                df_tmp = df.iloc[1:]
                coll.insert_many(to_json_from_pandas(df_tmp))
                start_time_plus_one = df.iloc[1]["date"]
                finish = datetime.datetime.now()
                interval = (finish - begin).total_seconds()
                text = "{}, {} -> {}, {:<04.2}s".format(
                    "{} {}".format(ins_type, code),
                    start_time_plus_one,
                    end_time,
                    interval,
                )
                for _ in trange(df.size, desc=text):
                    pass
            except Exception as e:
                slog.error("{}".format(e))
                err.append(code)

        tqdm.set_lock(RLock())
        with ThreadPoolExecutor(max_workers=4) as p:
            p.map(partial(saving_work), list(range(len(instruments))))

        save_error_log(err, "{}_{}_day".format(self.getClassName(), ins_type))

    def create_min(self, *args, **kwargs):
        """save index_min

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        ins_type = kwargs.pop("ins_type", "stock")
        instruments = (
            self.codes
            if self.codes
            else (
                get_stock_list(ins_type)
                if ins_type == INSTRUMENT_TYPE.INDX
                else get_stock_list().code.unique().tolist()
            )
        )
        coll = (
            self._db.index_min
            if ins_type == INSTRUMENT_TYPE.INDX
            else self._db.stock_min
        )
        coll.create_index(
            [
                ("code", pymongo.ASCENDING),
                ("time_stamp", pymongo.ASCENDING),
                ("date_stamp", pymongo.ASCENDING),
            ]
        )
        err = []

        def saving_work(idx):
            code = (
                instruments.index[idx][0]
                if ins_type == INSTRUMENT_TYPE.INDX
                else instruments[idx]
            )
            try:
                for freq in self.freqs:
                    begin = datetime.datetime.now()
                    ref_ = coll.find({"code": str(code)[0:6], "type": freq})
                    end_time = str(now_time())[0:19]
                    start_time = (
                        ref_[ref_.count() - 1]["datetime"]
                        if ref_.count() > 0
                        else "2015-01-01"
                    )
                    if start_time == end_time:
                        continue
                    df = (
                        get_index_min(str(code), start_time, end_time, freq)
                        if ins_type == INSTRUMENT_TYPE.INDX
                        else get_stock_min(str(code), start_time, end_time, freq)
                    )
                    if df is None or df.empty:
                        slog.warn(
                            "df is empty. {}|{}|{}|{}".format(
                                code, start_time, end_time, freq
                            )
                        )
                        continue
                    df_tmp = df.iloc[1:]
                    coll.insert_many(to_json_from_pandas(df_tmp))
                    start_time_plus_one = df.iloc[1]["datetime"]
                    finish = datetime.datetime.now()
                    interval = (finish - begin).total_seconds()
                    text = "{}, {}, {} -> {}, {:<04.2}s".format(
                        "{} {}".format(ins_type, code),
                        freq,
                        start_time_plus_one,
                        end_time,
                        interval,
                    )
                    for _ in trange(df.size, desc=text):
                        pass
            except Exception as e:
                slog.error("{}".format(e))
                err.append(code)

        tqdm.set_lock(RLock())
        with ThreadPoolExecutor(max_workers=4) as p:
            p.map(partial(saving_work), list(range(len(instruments))))

        save_error_log(err, "{}_{}_min".format(self.getClassName(), ins_type))

    def create_xdxr(self, *args, **kwargs):
        """save stock_xdxr

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        ins_type = kwargs.pop("ins_type", "")
        instruments = (
            self.codes
            if self.codes
            else (
                get_stock_list(ins_type)
                if ins_type == INSTRUMENT_TYPE.INDX
                else get_stock_list().code.unique().tolist()
            )
        )
        coll = self._db.stock_xdxr
        coll_adj = self._db.stock_adj
        coll.create_index(
            [("code", pymongo.ASCENDING), ("date", pymongo.ASCENDING)], unique=True
        )
        coll_adj.create_index(
            [("code", pymongo.ASCENDING), ("date", pymongo.ASCENDING)], unique=True,
        )
        err = []

        def saving_work(idx):
            code = (
                instruments.index[idx][0]
                if ins_type == INSTRUMENT_TYPE.INDX
                else instruments[idx]
            )
            try:
                begin = datetime.datetime.now()
                xdxr = get_stock_xdxr(str(code))
                try:
                    coll.insert_many(to_json_from_pandas(xdxr), ordered=False)
                except Exception as e:
                    pass
                    # slog.error(e)
                try:
                    data = query_stock_day(
                        str(code),
                        "1990-01-01",
                        str(datetime.date.today()),
                        self._db.stock_day,
                        "pd",
                    )
                    qfq = stock_to_fq(data, xdxr, "qfq")
                    qfq = qfq.assign(date=qfq.date.apply(lambda x: str(x)[0:10]))
                    adjdata = to_json_from_pandas(qfq.loc[:, ["date", "code", "adj"]])
                    coll_adj.delete_many({"code": code})
                    coll_adj.insert_many(adjdata)

                    finish = datetime.datetime.now()
                    interval = (finish - begin).total_seconds()
                    text = "{}, {:<04.2}s".format(
                        "{} {}".format(ins_type, code), interval,
                    )
                    for _ in trange(data.size, desc=text):
                        pass
                except Exception as e:
                    pass
                    # slog.error(e)
            except Exception as e:
                slog.error("{}".format(e))
                err.append(code)

        tqdm.set_lock(RLock())
        with ThreadPoolExecutor(max_workers=4) as p:
            p.map(partial(saving_work), list(range(len(instruments))))

        save_error_log(err, "{}_xdxr".format(self.getClassName()))

    def create_info(self, *args, **kwargs):
        """save stock_info

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        ins_type = kwargs.pop("ins_type", "stock")
        instruments = (
            self.codes
            if self.codes
            else (
                get_stock_list(ins_type)
                if ins_type == INSTRUMENT_TYPE.INDX
                else get_stock_list().code.unique().tolist()
            )
        )
        self._db.drop_collection("stock_info")
        coll = self._db.stock_info
        coll.create_index([("code", pymongo.ASCENDING)])
        err = []

        def saving_work(idx):
            code = (
                instruments.index[idx][0]
                if ins_type == INSTRUMENT_TYPE.INDX
                else instruments[idx]
            )
            try:
                begin = datetime.datetime.now()
                df = get_stock_info(str(code))
                if isinstance(df, pd.DataFrame) and df.empty:
                    slog.warn("df is empty.")
                    return
                if not isinstance(df, pd.DataFrame) and not df:
                    return
                coll.insert_many(to_json_from_pandas(df))
                finish = datetime.datetime.now()
                interval = (finish - begin).total_seconds()
                text = "{}, {:<04.2}s".format("{} {}".format(ins_type, code), interval,)
                for _ in trange(df.size, desc=text):
                    pass
            except Exception as e:
                slog.error("{}".format(e))
                err.append(code)

        tqdm.set_lock(RLock())
        with ThreadPoolExecutor(max_workers=4) as p:
            p.map(partial(saving_work), list(range(len(instruments))))

        save_error_log(err, "{}_{}_info".format(self.getClassName(), ins_type))

    def create_block(self, *args, **kwargs):
        """save stock block

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        brokers_api = kwargs.get("brokers_api", [])
        self._db.drop_collection("stock_block")
        coll = self._db.stock_block
        coll.create_index([("code", pymongo.ASCENDING)])

        def saving_work(b_api):
            try:
                begin = datetime.datetime.now()
                df = b_api.get_stock_block()
                if isinstance(df, pd.DataFrame) and df.empty:
                    slog.warn("df is empty.")
                    return
                if not isinstance(df, pd.DataFrame) and not df:
                    return
                coll.insert_many(to_json_from_pandas(df))
                finish = datetime.datetime.now()
                interval = (finish - begin).total_seconds()
                text = "stock block {}, {:<04.2}s".format(b_api.my_name(), interval)
                for _ in trange(df.size, desc=text):
                    pass
            except Exception as e:
                slog.error("{}".format(e))

        tqdm.set_lock(RLock())
        with ThreadPoolExecutor(max_workers=4) as p:
            p.map(partial(saving_work), brokers_api)

    def create_financial(self, *args, **kwargs):
        """save stock fundamentals

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        Setting.make_download_path()
        # return
        download_financialzip()
        coll = self._db.financial
        coll.create_index(
            [("code", pymongo.ASCENDING), ("report_date", pymongo.ASCENDING)],
            unique=True,
        )
        for item in os.listdir(Setting.DOWNLOAD_PATH):
            if item[0:4] != "gpcw":
                slog.debug(
                    "file {} is not start with gpcw , seems not a financial file, ignore!".format(
                        item
                    )
                )
                continue

            date = int(item.split(".")[0][-8:])
            slog.debug("NOW SAVING {}".format(date))
            slog.debug("在数据库中的条数 {}".format(coll.find({"report_date": date}).count()))
            try:
                data = to_json_from_pandas(
                    parse_filelist([item])
                    .reset_index()
                    .drop_duplicates(subset=["code", "report_date"])
                    .sort_index()
                )
                slog.debug("即将更新的条数 {}".format(len(data)))
                # data["crawl_date"] = str(datetime.date.today())
                try:
                    for d in data:
                        coll.update_one(
                            {"code": d["code"], "report_date": d["report_date"]},
                            {"$set": d},
                            upsert=True,
                        )

                except Exception as e:
                    if isinstance(e, MemoryError):
                        coll.insert_many(data, ordered=True)
                    elif isinstance(e, pymongo.bulk.BulkWriteError):
                        pass
            except Exception as e:
                slog.error("似乎没有数据")


class Future(ISecurity):
    def __init__(self, *args, **kwargs):
        "docstring"
        self.codes = kwargs.get("codes", [])
        self.freqs = []

    def accept(self, visitor: ISecurityVisitor) -> None:
        visitor.create_day(self)
        visitor.create_min(self)
        visitor.create_xdxr(self)

    def create_day(self, *args, **kwargs):
        ins_type = kwargs.pop("ins_type", "future")
        slog.debug("{} {} day".format(self.getClassName(), ins_type))
        return

    def create_min(self, *args, **kwargs):
        ins_type = kwargs.pop("ins_type", "future")
        slog.debug("{} {} min {}".format(self.getClassName(), ins_type, self.freqs))
        return


class Etf(ISecurity):
    def __init__(self, *args, **kwargs):
        "docstring"
        self._client = pymongo.MongoClient(Setting.get_mongo())
        self._db = self._client[Setting.DBNAME]
        self.codes = kwargs.get("codes", [])
        self.freqs = kwargs.get("freqs", ["1min", "5min", "15min", "30min", "60min"])

    def accept(self, visitor: ISecurityVisitor) -> None:
        visitor.create_day(self)
        visitor.create_min(self)

    def create_day(self, *args, **kwargs):
        """save etf_day

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        ins_type = kwargs.pop("ins_type", INSTRUMENT_TYPE.ETF)
        instruments = (
            self.codes
            if self.codes
            else (
                get_stock_list(ins_type)
                if ins_type == INSTRUMENT_TYPE.ETF
                else get_stock_list().code.unique().tolist()
            )
        )
        coll = (
            self._db.index_day
            if ins_type == INSTRUMENT_TYPE.ETF
            else self._db.stock_day
        )
        coll.create_index(
            [("code", pymongo.ASCENDING), ("date_stamp", pymongo.ASCENDING),]
        )
        err = []

        def saving_work(idx):
            code = (
                instruments.index[idx][0]
                if ins_type == INSTRUMENT_TYPE.ETF
                else instruments[idx]
            )
            try:
                begin = datetime.datetime.now()
                ref_ = coll.find({"code": str(code)[0:6]})
                end_time = str(now_time())[0:10]
                start_time = (
                    ref_[ref_.count() - 1]["date"] if ref_.count() > 0 else "1990-01-01"
                )
                if start_time == end_time:
                    return
                df = (
                    get_index_day(str(code), start_time, end_time)
                    if ins_type == INSTRUMENT_TYPE.ETF
                    else get_stock_day(str(code), start_time, end_time)
                )
                if df is None or df.empty:
                    slog.warn(
                        "df is empty. {}|{}|{}".format(code, start_time, end_time)
                    )
                    return

                df_tmp = df.iloc[1:]
                coll.insert_many(to_json_from_pandas(df_tmp))
                start_time_plus_one = df.iloc[1]["date"]
                finish = datetime.datetime.now()
                interval = (finish - begin).total_seconds()
                text = "{}, {} -> {}, {:<04.2}s".format(
                    "{} {}".format(ins_type, code),
                    start_time_plus_one,
                    end_time,
                    interval,
                )
                for _ in trange(df.size, desc=text):
                    pass
            except Exception as e:
                slog.error("{}".format(e))
                err.append(code)

        tqdm.set_lock(RLock())
        with ThreadPoolExecutor(max_workers=4) as p:
            p.map(partial(saving_work), list(range(len(instruments))))

        save_error_log(err, "{}_{}_day".format(self.getClassName(), ins_type))

    def create_min(self, *args, **kwargs):
        """save etf_min

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        ins_type = kwargs.pop("ins_type", "etf")
        instruments = (
            self.codes
            if self.codes
            else (
                get_stock_list(ins_type)
                if ins_type == INSTRUMENT_TYPE.ETF
                else get_stock_list().code.unique().tolist()
            )
        )
        coll = (
            self._db.index_min
            if ins_type == INSTRUMENT_TYPE.ETF
            else self._db.stock_min
        )
        coll.create_index(
            [
                ("code", pymongo.ASCENDING),
                ("time_stamp", pymongo.ASCENDING),
                ("date_stamp", pymongo.ASCENDING),
            ]
        )
        err = []

        def saving_work(idx):
            code = (
                instruments.index[idx][0]
                if ins_type == INSTRUMENT_TYPE.ETF
                else instruments[idx]
            )
            try:
                for freq in self.freqs:
                    begin = datetime.datetime.now()
                    ref_ = coll.find({"code": str(code)[0:6], "type": freq})
                    end_time = str(now_time())[0:19]
                    start_time = (
                        ref_[ref_.count() - 1]["datetime"]
                        if ref_.count() > 0
                        else "2015-01-01"
                    )
                    if start_time == end_time:
                        continue
                    df = (
                        get_index_min(str(code), start_time, end_time, freq)
                        if ins_type == INSTRUMENT_TYPE.ETF
                        else get_stock_min(str(code), start_time, end_time, freq)
                    )
                    if df is None or df.empty:
                        slog.warn(
                            "df is empty. {}|{}|{}|{}".format(
                                code, start_time, end_time, freq
                            )
                        )
                        continue
                    df_tmp = df.iloc[1:]
                    coll.insert_many(to_json_from_pandas(df_tmp))
                    start_time_plus_one = df.iloc[1]["datetime"]
                    finish = datetime.datetime.now()
                    interval = (finish - begin).total_seconds()
                    text = "{}, {}, {} -> {}, {:<04.2}s".format(
                        "{} {}".format(ins_type, code),
                        freq,
                        start_time_plus_one,
                        end_time,
                        interval,
                    )
                    for _ in trange(df.size, desc=text):
                        pass
            except Exception as e:
                slog.error("{}".format(e))
                err.append(code)

        tqdm.set_lock(RLock())
        with ThreadPoolExecutor(max_workers=4) as p:
            p.map(partial(saving_work), list(range(len(instruments))))

        save_error_log(err, "{}_{}_min".format(self.getClassName(), ins_type))

    def create_list(self, *args, **kwargs):
        """save etf list

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        brokers_api = kwargs.get("brokers_api", [])
        self._db.drop_collection("etf_list")
        coll = self._db.etf_list
        coll.create_index([("code", pymongo.ASCENDING)])

        def saving_work(b_api):
            try:
                begin = datetime.datetime.now()
                df = b_api.get_stock_list(type_=INSTRUMENT_TYPE.ETF)
                if isinstance(df, pd.DataFrame) and df.empty:
                    slog.warn("df is empty.")
                    return
                if not isinstance(df, pd.DataFrame) and not df:
                    return
                coll.insert_many(to_json_from_pandas(df))
                finish = datetime.datetime.now()
                interval = (finish - begin).total_seconds()
                text = "etf list {}, {:<04.2}s".format(b_api.my_name(), interval)
                for _ in trange(df.size, desc=text):
                    pass
            except Exception as e:
                slog.error("{}".format(e))

        tqdm.set_lock(RLock())
        with ThreadPoolExecutor(max_workers=1) as p:
            p.map(partial(saving_work), brokers_api)
