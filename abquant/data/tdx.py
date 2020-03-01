# -*- coding: utf-8 -*-
from __future__ import print_function
from .tdx_api import get_stock_list
from abquant.helper import to_json_from_pandas
from abquant.utils.logger import system_log as slog
import pymongo
import concurrent
from abquant.config import Setting
from abquant.utils.datetime import now_time
from abquant.data.tdx_api import get_stock_day, get_index_day, get_index_min, get_stock_min
# from abquant.data.base import ISecurityVisitor
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


class Stock(object):
    def __init__(self, *args, **kwargs):
        "docstring"
        self._client = pymongo.MongoClient(Setting.get_mongo())
        self._db = self._client[Setting.DBNAME]
        self.freqs = kwargs.get("freqs", ["1min", "5min", "15min", "30min", "60min"])

    # def accept(self, visitor: ISecurityVisitor) -> None:
    def accept(self, visitor) -> None:
        visitor.create_day(self)
        visitor.create_min(self)

    def create_day(self, *args, **kwargs):
        """save index_day

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        stockOrIndex = kwargs.pop("stockOrIndex", "stock")
        stockOrIndexList = (
            get_stock_list(stockOrIndex)
            if stockOrIndex == "index"
            else get_stock_list().code.unique().tolist()
        )
        coll = self._db.index_day if stockOrIndex == "index" else self._db.stock_day
        coll.create_index(
            [
                ("code", pymongo.ASCENDING),
                ("date_stamp", pymongo.ASCENDING),
            ]
        )
        err = []

        def __saving_work(idx):
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
                    ref_[ref_.count() - 1]["date"]
                    if ref_.count() > 0
                    else "1990-01-01"
                )
                if start_time == end_time:
                    return
                df = (
                    get_index_day(str(code), start_time, end_time)
                    if stockOrIndex == "index"
                    else get_stock_day(str(code), start_time, end_time)
                )
                if df.empty:
                    return
                coll.insert_many(
                    to_json_from_pandas(df)
                )
                finish = datetime.datetime.now()
                interval = (finish - begin).total_seconds()
                text = "{}, {} -> {}, {:<04.2}s".format(
                    "{} {}".format(stockOrIndex, code),
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
            p.map(partial(__saving_work), list(range(len(stockOrIndexList))))

        if len(err) < 1:
            slog.info("SUCCESS")
        else:
            slog.info(" ERROR CODE \n ")
            slog.info(err)


    def create_min(self, *args, **kwargs):
        """save index_min

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        stockOrIndex = kwargs.pop("stockOrIndex", "stock")

        stockOrIndexList = (
            get_stock_list(stockOrIndex)
            if stockOrIndex == "index"
            else get_stock_list().code.unique().tolist()
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

        def __saving_work(idx):
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
                    if df.empty:
                        continue
                    coll.insert_many(
                        to_json_from_pandas(df)
                    )
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
            p.map(partial(__saving_work), list(range(len(stockOrIndexList))))

        if len(err) < 1:
            slog.info("SUCCESS")
        else:
            slog.info(" ERROR CODE \n ")
            slog.info(err)


class Future(object):
    def __init__(self, *args, **kwargs):
        "docstring"
        pass

    # def accept(self, visitor: ISecurityVisitor) -> None:
    def accept(self, visitor) -> None:
        visitor.create_day(self)
        visitor.create_min(self)

    def create_day(self, *args, **kwargs):
        slog.debug("Future, create_day")

    def create_min(self, *args, **kwargs):
        slog.debug("Future, create_min")
