# -*- coding: utf-8 -*-
from __future__ import print_function
from .tdx_api import get_stock_list
from abquant.helper import to_json_from_pandas
from abquant.utils.logger import system_log as slog
import pymongo
import concurrent
from abquant.config import Setting
from abquant.utils.datetime import now_time
from abquant.data.tdx_api import get_index_min, get_stock_min
from .base import ISecurityVisitor
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
        stockOrIndex = kwargs.get("stockOrIndex", False)
        self.freqs = kwargs.get("freqs", ["1min", "5min", "15min", "30min", "60min"])

    def accept(self, visitor: ISecurityVisitor) -> None:
        visitor.create_day(self)
        visitor.create_min(self)

    def create_index_day(self):
        slog.debug("Stock, create_index_day")

    def create_day(self):
        slog.debug("Stock, create_day")

    def create_index_min(self, *args, **kwargs):
        slog.debug("Stock, create_index_min")
        slog.debug(self.freqs)
        # return
        """save index_min

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """

        stockOrIndexList = get_stock_list("index")
        coll = self._db.index_min
        coll.create_index(
            [
                ("code", pymongo.ASCENDING),
                ("time_stamp", pymongo.ASCENDING),
                ("date_stamp", pymongo.ASCENDING),
            ]
        )
        err = []

        def __saving_work(idx):
            code = stockOrIndexList.index[idx][0]
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
                    df = get_index_min(str(code), start_time, end_time, freq)
                    if df.empty:
                        continue
                    # coll.insert_many(
                    #     to_json_from_pandas(df[1::])
                    # )
                    finish = datetime.datetime.now()
                    interval = (finish - begin).total_seconds()
                    text = "{}, {}, {} -> {}, {:<04.2}s".format(
                        code, freq, start_time, end_time, interval
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
        slog.debug("Stock, create_min")
        slog.debug("Stock, create_index_min")
        slog.debug(self.freqs)
        # return
        """save index_min

        Keyword Arguments:
            client {[type]} -- [description] (default: {DATABASE})
        """
        stockOrIndex = kwargs.pop("stockOrIndex", "stock")
        slog.debug("stockOrIndex {}".format(stockOrIndex))
        stockOrIndexList = (
            get_stock_list(stockOrIndex)
            if stockOrIndex == "index"
            else get_stock_list().code.unique().tolist()
        )
        slog.debug("stockOrIndexList {}".format(len(stockOrIndexList)))
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

            slog.debug("stockOrIndex: code {}".format(code))
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
                    # coll.insert_many(
                    #     to_json_from_pandas(df[1::])
                    # )
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

    def accept(self, visitor: ISecurityVisitor) -> None:
        visitor.create_day(self)
        visitor.create_min(self)

    def create_index_day(self):
        slog.debug("Future, create_index_day")

    def create_day(self):
        slog.debug("Future, create_day")

    def create_index_min(self, *args, **kwargs):
        slog.debug("Future, create_index_min")

    def create_min(self, *args, **kwargs):
        slog.debug("Future, create_min")


# def create_index_min(client=DATABASE, ui_log=None, ui_progress=None):
#     """save index_min

#     Keyword Arguments:
#         client {[type]} -- [description] (default: {DATABASE})
#     """

#     stockOrIndexList = get_stock_list('index')
#     coll = client.index_min
#     coll.create_index(
#         [
#             ('code',
#              pymongo.ASCENDING),
#             ('time_stamp',
#              pymongo.ASCENDING),
#             ('date_stamp',
#              pymongo.ASCENDING)
#         ]
#     )
#     err = []

#     def __saving_work(code, coll):

#         slog.info(
#             '##JOB05 Now Saving Index_MIN ==== {}'.format(str(code)),
#             ui_log=ui_log
#         )
#         try:

#             for type in ['1min', '5min', '15min', '30min', '60min']:
#                 ref_ = coll.find({'code': str(code)[0:6], 'type': type})
#                 end_time = str(now_time())[0:19]
#                 if ref_.count() > 0:
#                     start_time = ref_[ref_.count() - 1]['datetime']

#                     slog.info(
#                         '##JOB05.{} Now Saving {} from {} to {} =={} '.format(
#                             ['1min',
#                              '5min',
#                              '15min',
#                              '30min',
#                              '60min'].index(type),
#                             str(code),
#                             start_time,
#                             end_time,
#                             type
#                         ),
#                         ui_log=ui_log
#                     )

#                     if start_time != end_time:
#                         __data = QA_fetch_get_index_min(
#                             str(code),
#                             start_time,
#                             end_time,
#                             type
#                         )
#                         if len(__data) > 1:
#                             coll.insert_many(
#                                 QA_util_to_json_from_pandas(__data[1::])
#                             )
#                 else:
#                     start_time = '2015-01-01'

#                     slog.info(
#                         '##JOB05.{} Now Saving {} from {} to {} =={} '.format(
#                             ['1min',
#                              '5min',
#                              '15min',
#                              '30min',
#                              '60min'].index(type),
#                             str(code),
#                             start_time,
#                             end_time,
#                             type
#                         ),
#                         ui_log=ui_log
#                     )

#                     if start_time != end_time:
#                         __data = QA_fetch_get_index_min(
#                             str(code),
#                             start_time,
#                             end_time,
#                             type
#                         )
#                         if len(__data) > 1:
#                             coll.insert_many(
#                                 QA_util_to_json_from_pandas(__data)
#                             )
#         except:
#             err.append(code)

#     executor = ThreadPoolExecutor(max_workers=4)

#     res = {
#         executor.submit(__saving_work,
#                         stockOrIndexList.index[i_][0],
#                         coll)
#         for i_ in range(len(stockOrIndexList))
#     }  # multi index ./.
#     count = 0
#     for i_ in concurrent.futures.as_completed(res):
#         strLogProgress = 'DOWNLOAD PROGRESS {} '.format(
#             str(float(count / len(stockOrIndexList) * 100))[0:4] + '%'
#         )
#         intLogProgress = int(float(count / len(stockOrIndexList) * 10000.0))
#         slog.info(
#             'The {} of Total {}'.format(count,
#                                         len(stockOrIndexList)),
#             ui_log=ui_log
#         )
#         slog.info(
#             strLogProgress,
#             ui_log=ui_log,
#             ui_progress=ui_progress,
#             ui_progress_int_value=intLogProgress
#         )
#         count = count + 1
#     if len(err) < 1:
#         slog.info('SUCCESS', ui_log=ui_log)
#     else:
#         slog.info(' ERROR CODE \n ', ui_log=ui_log)
#         slog.info(err, ui_log=ui_log)
