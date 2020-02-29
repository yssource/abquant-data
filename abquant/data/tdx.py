# -*- coding: utf-8 -*-
from .tdx_api import get_stock_list
from abquant.utils.logger import system_log as slog
import pymongo
import concurrent
from concurrent.futures import ThreadPoolExecutor
from .base import ISecurityVisitor

class Stock(object):
    def __init__(self, *args, **kwargs):
        "docstring"
        pass

    def accept(self, visitor :ISecurityVisitor) -> None:
        visitor.create_day(self)
        visitor.create_min(self)

    def create_index_day(self):
        slog.debug("Stock, create_index_day")

    def create_day(self):
        slog.debug("Stock, create_day")

    def create_index_min(self, *args, **kwargs):
        slog.debug("Stock, create_index_min")

    def create_min(self, *args, **kwargs):
        slog.debug("Stock, create_min")

class Future(object):
    def __init__(self, *args, **kwargs):
        "docstring"
        pass

    def accept(self, visitor :ISecurityVisitor) -> None:
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

#     __index_list = get_stock_list('index')
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
#                         __index_list.index[i_][0],
#                         coll)
#         for i_ in range(len(__index_list))
#     }  # multi index ./.
#     count = 0
#     for i_ in concurrent.futures.as_completed(res):
#         strLogProgress = 'DOWNLOAD PROGRESS {} '.format(
#             str(float(count / len(__index_list) * 100))[0:4] + '%'
#         )
#         intLogProgress = int(float(count / len(__index_list) * 10000.0))
#         slog.info(
#             'The {} of Total {}'.format(count,
#                                         len(__index_list)),
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
