# -*- coding: utf-8 -*-
from __future__ import print_function
from .tdx_api import get_stock_list
from abquant.helper import to_json_from_pandas
from abquant.utils.logger import system_log as slog
import pymongo
import concurrent
from abquant.config import Setting
from abquant.utils.datetime import now_time
from abquant.utils.tdx import query_stock_day, stock_to_fq
from abquant.data.tdx_api import (
    get_stock_day,
    get_index_day,
    get_index_min,
    get_stock_min,
    get_stock_xdxr,
    get_stock_info,
)

from abquant.data.base import ISecurity, ISecurityVisitor
from time import sleep
from tqdm.auto import tqdm, trange
from tqdm.contrib.concurrent import process_map, thread_map
from random import random
from multiprocessing import Pool, freeze_support
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor
from threading import RLock
from functools import partial
import sys
import datetime
import json


# class Stock(object):
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
        """save index_day

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        stockOrIndex = kwargs.pop("stockOrIndex", "stock")
        stockOrIndexList = (
            self.codes
            if self.codes
            else (
                get_stock_list(stockOrIndex)
                if stockOrIndex == "index"
                else get_stock_list().code.unique().tolist()
            )
        )
        coll = self._db.index_day if stockOrIndex == "index" else self._db.stock_day
        coll.create_index(
            [("code", pymongo.ASCENDING), ("date_stamp", pymongo.ASCENDING),]
        )
        err = []

        def saving_work(idx):
            code = (
                stockOrIndexList.index[idx][0]
                if stockOrIndex == "index"
                else stockOrIndexList[idx]
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
                    if stockOrIndex == "index"
                    else get_stock_day(str(code), start_time, end_time)
                )
                if df is None or df.empty:
                    slog.warn(
                        "df is empty. {}|{}|{}".format(code, start_time, end_time)
                    )
                    return
                coll.insert_many(to_json_from_pandas(df))
                finish = datetime.datetime.now()
                interval = (finish - begin).total_seconds()
                text = "{}, {} -> {}, {:<04.2}s".format(
                    "{} {}".format(stockOrIndex, code), start_time, end_time, interval,
                )
                for _ in trange(df.size, desc=text):
                    pass
            except Exception as e:
                slog.error("{}".format(e))
                err.append(code)

        tqdm.set_lock(RLock())
        with ThreadPoolExecutor(max_workers=4) as p:
            p.map(partial(saving_work), list(range(len(stockOrIndexList))))

        if len(err) < 1:
            slog.info("SUCCESS")
        else:
            slog.info(" ERROR CODE \n ")
            slog.info(err)
            errs = dict()
            if Setting.ERROR_CODES_JSON.exists():
                with Setting.ERROR_CODES_JSON.open(mode="r") as f:
                    try:
                        errs = json.load(f)
                    except Exception:
                        pass
            errs["{}_{}_day".format(self.getClassName(), stockOrIndex)] = list(set(err))
            with Setting.ERROR_CODES_JSON.open(mode="w") as f:
                json.dump(errs, f)

    def create_min(self, *args, **kwargs):
        """save index_min

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        stockOrIndex = kwargs.pop("stockOrIndex", "stock")
        stockOrIndexList = (
            self.codes
            if self.codes
            else (
                get_stock_list(stockOrIndex)
                if stockOrIndex == "index"
                else get_stock_list().code.unique().tolist()
            )
        )
        coll = self._db.index_min if stockOrIndex == "index" else self._db.stock_min
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
                stockOrIndexList.index[idx][0]
                if stockOrIndex == "index"
                else stockOrIndexList[idx]
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
                        if stockOrIndex == "index"
                        else get_stock_min(str(code), start_time, end_time, freq)
                    )
                    if df is None or df.empty:
                        slog.warn(
                            "df is empty. {}|{}|{}|{}".format(
                                code, start_time, end_time, freq
                            )
                        )
                        continue
                    coll.insert_many(to_json_from_pandas(df))
                    finish = datetime.datetime.now()
                    interval = (finish - begin).total_seconds()
                    text = "{}, {}, {} -> {}, {:<04.2}s".format(
                        "{} {}".format(stockOrIndex, code),
                        freq,
                        start_time,
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
            p.map(partial(saving_work), list(range(len(stockOrIndexList))))

        if len(err) < 1:
            slog.info("SUCCESS")
        else:
            slog.info(" ERROR CODE \n ")
            slog.info(err)
            errs = dict()
            if Setting.ERROR_CODES_JSON.exists():
                with Setting.ERROR_CODES_JSON.open(mode="r") as f:
                    try:
                        errs = json.load(f)
                    except Exception:
                        pass
            errs["{}_{}_min".format(self.getClassName(), stockOrIndex)] = list(set(err))
            with Setting.ERROR_CODES_JSON.open(mode="w") as f:
                json.dump(errs, f)

    def create_xdxr(self, *args, **kwargs):
        """save stock_xdxr

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        stockOrIndex = kwargs.pop("stockOrIndex", "")
        stockOrIndexList = (
            self.codes
            if self.codes
            else (
                get_stock_list(stockOrIndex)
                if stockOrIndex == "index"
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
                stockOrIndexList.index[idx][0]
                if stockOrIndex == "index"
                else stockOrIndexList[idx]
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
                        "{} {}".format(stockOrIndex, code), interval,
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
            p.map(partial(saving_work), list(range(len(stockOrIndexList))))

        if len(err) < 1:
            slog.info("SUCCESS")
        else:
            slog.info(" ERROR CODE \n ")
            slog.info(err)
            errs = dict()
            if Setting.ERROR_CODES_JSON.exists():
                with Setting.ERROR_CODES_JSON.open(mode="r") as f:
                    try:
                        errs = json.load(f)
                    except Exception:
                        pass
            errs["{}_xdxr".format(self.getClassName())] = list(set(err))
            with Setting.ERROR_CODES_JSON.open(mode="w") as f:
                json.dump(errs, f)

    def create_info(self, *args, **kwargs):
        """save index_day

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        stockOrIndex = kwargs.pop("stockOrIndex", "stock")
        stockOrIndexList = (
            self.codes
            if self.codes
            else (
                get_stock_list(stockOrIndex)
                if stockOrIndex == "index"
                else get_stock_list().code.unique().tolist()
            )
        )
        self._db.drop_collection("stock_info")
        coll = self._db.stock_info
        coll.create_index([("code", pymongo.ASCENDING)])
        err = []

        def saving_work(idx):
            code = (
                stockOrIndexList.index[idx][0]
                if stockOrIndex == "index"
                else stockOrIndexList[idx]
            )
            try:
                begin = datetime.datetime.now()
                df = get_stock_info(str(code))
                if df is None or df.empty:
                    slog.warn("df is empty. {}|{}|{}".format(code))
                    return
                coll.insert_many(to_json_from_pandas(df))
                finish = datetime.datetime.now()
                interval = (finish - begin).total_seconds()
                text = "{}, {:<04.2}s".format(
                    "{} {}".format(stockOrIndex, code), interval,
                )
                for _ in trange(df.size, desc=text):
                    pass
            except Exception as e:
                slog.error("{}".format(e))
                err.append(code)

        tqdm.set_lock(RLock())
        with ThreadPoolExecutor(max_workers=4) as p:
            p.map(partial(saving_work), list(range(len(stockOrIndexList))))

        if len(err) < 1:
            slog.info("SUCCESS")
        else:
            slog.info(" ERROR CODE \n ")
            slog.info(err)
            errs = dict()
            if Setting.ERROR_CODES_JSON.exists():
                with Setting.ERROR_CODES_JSON.open(mode="r") as f:
                    try:
                        errs = json.load(f)
                    except Exception:
                        pass
            errs["{}_{}_info".format(self.getClassName(), stockOrIndex)] = list(
                set(err)
            )
            with Setting.ERROR_CODES_JSON.open(mode="w") as f:
                json.dump(errs, f)

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
                data = b_api.get_stock_block()
                coll.insert_many(to_json_from_pandas(data))
                finish = datetime.datetime.now()
                interval = (finish - begin).total_seconds()
                text = "stock block {}, {:<04.2}s".format(b_api.my_name(), interval)
                for _ in trange(data.size, desc=text):
                    pass
            except Exception as e:
                slog.error("{}".format(e))

        tqdm.set_lock(RLock())
        with ThreadPoolExecutor(max_workers=4) as p:
            p.map(partial(saving_work), brokers_api)


class Future(ISecurity):
    def __init__(self, *args, **kwargs):
        "docstring"
        self.codes = kwargs.get("codes", [])
        self.freqs = []
        pass

    def accept(self, visitor: ISecurityVisitor) -> None:
        visitor.create_day(self)
        visitor.create_min(self)
        visitor.create_xdxr(self)

    def create_day(self, *args, **kwargs):
        stockOrIndex = kwargs.pop("stockOrIndex", "future")
        slog.debug("{} {} day".format(self.getClassName(), stockOrIndex))
        return

    def create_min(self, *args, **kwargs):
        stockOrIndex = kwargs.pop("stockOrIndex", "future")
        slog.debug("{} {} min {}".format(self.getClassName(), stockOrIndex, self.freqs))
        return
