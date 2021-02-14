# -*- coding: utf-8 -*-

import datetime
import threading
import time
import pandas as pd

from abquant.utils.logger import system_log as slog
from abquant.utils.trading_days import trade_date_sse

QATZInfo_CN = "Asia/Shanghai"


def QA_util_time_now():
    """
    explanation:
       获取当前日期时间

    return:
        datetime
    """
    return datetime.datetime.now()


def QA_util_date_today():
    """
    explanation:
       获取当前日期

    return:
        date
    """
    return datetime.date.today()


def QA_util_today_str():
    """
    explanation:
        返回今天的日期字符串

    return:
        str
    """
    dt = QA_util_date_today()
    return QA_util_datetime_to_strdate(dt)


def QA_util_date_str2int(date):
    """
    explanation:
        转换日期字符串为整数

    params:
        * date->
            含义: 日期字符串
            类型: date
            参数支持: []

    demonstrate:
        print(QA_util_date_str2int("2011-09-11"))

    return:
        int

    output:
        >>20110911
    """
    # return int(str(date)[0:4] + str(date)[5:7] + str(date)[8:10])
    if isinstance(date, str):
        return int(str().join(date.split("-")))
    elif isinstance(date, int):
        return date


def QA_util_date_int2str(int_date):
    """
    explanation:
        转换日期整数为字符串

    params:
        * int_date->
            含义: 日期转换得
            类型: int
            参数支持: []

    return:
        str
    """
    date = str(int_date)
    if len(date) == 8:
        return str(date[0:4] + "-" + date[4:6] + "-" + date[6:8])
    elif len(date) == 10:
        return date


def QA_util_to_datetime(time):
    """
    explanation:
        转换字符串格式的日期为datetime

    params:
        * time->
            含义: 日期
            类型: str
            参数支持: []

    return:
        datetime
    """
    if len(str(time)) == 10:
        _time = "{} 00:00:00".format(time)
    elif len(str(time)) == 19:
        _time = str(time)
    else:
        slog.error("WRONG DATETIME FORMAT {}".format(time))
    return datetime.datetime.strptime(_time, "%Y-%m-%d %H:%M:%S")


def QA_util_datetime_to_strdate(dt):
    """
    explanation:
        转换字符串格式的日期为datetime

    params:
        * dt->
            含义: 日期时间
            类型: datetime
            参数支持: []

    return:
        str
    """
    strdate = "%04d-%02d-%02d" % (dt.year, dt.month, dt.day)
    return strdate


def QA_util_datetime_to_strdatetime(dt):
    """
    explanation:
        转换日期时间为字符串格式

    params:
        * dt->
            含义: 日期时间
            类型: datetime
            参数支持: []

    return:
        datetime

    """
    strdatetime = "%04d-%02d-%02d %02d:%02d:%02d" % (
        dt.year,
        dt.month,
        dt.day,
        dt.hour,
        dt.minute,
        dt.second,
    )
    return strdatetime


def make_datestamp(date):
    """
    explanation:
        转换日期时间字符串为浮点数的时间戳

    params:
        * date->
            含义: 日期时间
            类型: str
            参数支持: []

    return:
        time
    """
    datestr = str(date)[0:10]
    date = time.mktime(time.strptime(datestr, "%Y-%m-%d"))
    return date


def make_timestamp(time_):
    """
    explanation:
       转换日期时间的字符串为浮点数的时间戳

    params:
        * time_->
            含义: 日期时间
            类型: str
            参数支持: ['2018-01-01 00:00:00']

    return:
        time
    """
    if len(str(time_)) == 10:
        # yyyy-mm-dd格式
        return time.mktime(time.strptime(time_, "%Y-%m-%d"))
    elif len(str(time_)) == 16:
        # yyyy-mm-dd hh:mm格式
        return time.mktime(time.strptime(time_, "%Y-%m-%d %H:%M"))
    else:
        timestr = str(time_)[0:19]
        return time.mktime(time.strptime(timestr, "%Y-%m-%d %H:%M:%S"))


def QA_util_tdxtimestamp(time_stamp):

    """
    explanation:
       转换tdx的realtimeQuote数据, [相关地址](https://github.com/rainx/pytdx/issues/187#issuecomment-441270487)

    params:
        * time_stamp->
            含义: 时间
            类型: str
            参数支持: []

    return:
        int

    """
    if time_stamp is not None:
        time_stamp = str(time_stamp)
        time = time_stamp[:-6] + ":"
        if int(time_stamp[-6:-4]) < 60:
            time += "%s:" % time_stamp[-6:-4]
            time += "%06.3f" % (int(time_stamp[-4:]) * 60 / 10000.0)
        else:
            time += "%02d:" % (int(time_stamp[-6:]) * 60 / 1000000)
            time += "%06.3f" % ((int(time_stamp[-6:]) * 60 % 1000000) * 60 / 1000000.0)
        return time


def QA_util_pands_timestamp_to_date(pandsTimestamp):
    """
    explanation:
        转换 pandas 的时间戳 到 datetime.date类型

    params:
        * pandsTimestamp->
            含义: pandas的时间戳
            类型:  pandas._libs.tslib.Timestamp
            参数支持: []
    return:
        date
    """
    return pandsTimestamp.to_pydatetime().date()


def QA_util_pands_timestamp_to_datetime(pandsTimestamp):
    """
    explanation:
        转换 pandas时间戳 到 datetime.datetime类型

    params:
        * pandsTimestamp->
            含义: pandas时间戳
            类型:  pandas._libs.tslib.Timestamp
            参数支持: []
    return:
        datetime
    """
    return pandsTimestamp.to_pydatetime()


def QA_util_stamp2datetime(timestamp):
    """
    explanation:
        datestamp转datetime,pandas转出来的timestamp是13位整数 要/1000,
        It’s common for this to be restricted to years from 1970 through 2038.
        从1970年开始的纳秒到当前的计数 转变成 float 类型时间 类似 time.time() 返回的类型

    params:
        * timestamp->
            含义: 时间戳
            类型: float
            参数支持: []

    return:
        datetime
    """
    try:
        return datetime.datetime.fromtimestamp(timestamp)
    except Exception as e:
        # it won't work ??
        try:
            return datetime.datetime.fromtimestamp(timestamp / 1000)
        except:
            try:
                return datetime.datetime.fromtimestamp(timestamp / 1000000)
            except:
                return datetime.datetime.fromtimestamp(timestamp / 1000000000)


def QA_util_ms_stamp(ms):
    """
    explanation:
        直接返回不做处理

    params:
        * ms->
            含义: 时间戳
            类型: float
            参数支持: []
    return:
        float
    """

    return ms


def QA_util_date_valid(date):
    """
    explanation:
        判断字符串格式(1982-05-11)

    params:
        * date->
            含义: 日期
            类型: str
            参数支持: []

    return:
        bool
    """
    try:
        time.strptime(date, "%Y-%m-%d")
        return True
    except:
        return False


def QA_util_get_date_index(date, trade_list):
    """
    explanation:
        返回在trade_list中的index位置

    params:
        * date->
            含义: 日期
            类型: str
            参数支持: []
        * trade_list->
            含义: 代码
            类型: ??
            参数支持: []

    return:
        ??
    """
    return trade_list.index(date)


def QA_util_get_index_date(id, trade_list):
    """
    explanation:
        根据id索引值

    params:
        * id->
            含义: 日期
            类型: str
            参数支持: []
        * trade_list->
            含义: 代码
            类型: dict
            参数支持: []

    return:
        ??
    """
    return trade_list[id]


def QA_util_select_hours(time=None, gt=None, lt=None, gte=None, lte=None):
    """
    explanation:
        quantaxis的时间选择函数,约定时间的范围,比如早上9点到11点

    params:
        * time->
            含义: 时间
            类型: str
            参数支持: []
        * gt->
            含义: 大于
            类型: Any
            参数支持: []
        * lt->
            含义: 小于
            类型: Any
            参数支持: []
        * gte->
            含义: 大于等于
            类型: Any
            参数支持: []
        * lte->
            含义: 小于等于
            类型: Any
            参数支持: []

    return:
        bool
    """
    if time is None:
        __realtime = datetime.datetime.now()
    else:
        __realtime = time

    fun_list = []
    if gt != None:
        fun_list.append(">")
    if lt != None:
        fun_list.append("<")
    if gte != None:
        fun_list.append(">=")
    if lte != None:
        fun_list.append("<=")

    assert len(fun_list) > 0
    true_list = []
    try:
        for item in fun_list:
            if item == ">":
                if __realtime.strftime("%H") > gt:
                    true_list.append(0)
                else:
                    true_list.append(1)
            elif item == "<":
                if __realtime.strftime("%H") < lt:
                    true_list.append(0)
                else:
                    true_list.append(1)
            elif item == ">=":
                if __realtime.strftime("%H") >= gte:
                    true_list.append(0)
                else:
                    true_list.append(1)
            elif item == "<=":
                if __realtime.strftime("%H") <= lte:
                    true_list.append(0)
                else:
                    true_list.append(1)

    except:
        return Exception
    if sum(true_list) > 0:
        return False
    else:
        return True


def QA_util_select_min(time=None, gt=None, lt=None, gte=None, lte=None):
    """
    explanation:
        择分钟

    params:
        * time->
            含义: 时间
            类型: str
            参数支持: []
        * gt->
            含义: 大于等于
            类型: Any
            参数支持: []
        * lt->
            含义: 小于
            类型: Any
            参数支持: []
        * gte->
            含义: 大于等于
            类型: Any
            参数支持: []
        * lte->
            含义: 小于等于
            类型: Any
            参数支持: []

    return:
        bool
    """
    if time is None:
        __realtime = datetime.datetime.now()
    else:
        __realtime = time

    fun_list = []
    if gt != None:
        fun_list.append(">")
    if lt != None:
        fun_list.append("<")
    if gte != None:
        fun_list.append(">=")
    if lte != None:
        fun_list.append("<=")

    assert len(fun_list) > 0
    true_list = []
    try:
        for item in fun_list:
            if item == ">":
                if __realtime.strftime("%M") > gt:
                    true_list.append(0)
                else:
                    true_list.append(1)
            elif item == "<":
                if __realtime.strftime("%M") < lt:
                    true_list.append(0)
                else:
                    true_list.append(1)
            elif item == ">=":
                if __realtime.strftime("%M") >= gte:
                    true_list.append(0)
                else:
                    true_list.append(1)
            elif item == "<=":
                if __realtime.strftime("%M") <= lte:
                    true_list.append(0)
                else:
                    true_list.append(1)
    except:
        return Exception
    if sum(true_list) > 0:
        return False
    else:
        return True


def QA_util_time_delay(time_=0):
    """
    explanation:
        这是一个用于复用/比如说@装饰器的延时函数,使用threading里面的延时,为了是不阻塞进程,
        有时候,同时发进去两个函数,第一个函数需要延时,第二个不需要的话,用sleep就会阻塞掉第二个进程

    params:
        * time_->
            含义: 时间
            类型: time
            参数支持: []

    return:
        func
    """

    def _exec(func):
        threading.Timer(time_, func)

    return _exec


def QA_util_calc_time(func, *args, **kwargs):
    """
    explanation:
        耗时长度的装饰器

    params:
        * func ->
            含义: 被装饰的函数
            类型: func
            参数支持: []
        * args ->
            含义: 函数接受的任意元组参数
            类型: tuple
            参数支持: []
        * kwargs ->
            含义: 函数接受的任意字典参数
            类型: dict
            参数支持: []

    return:
        None
    """
    _time = datetime.datetime.now()
    func(*args, **kwargs)
    print(datetime.datetime.now() - _time)
    # return datetime.datetime.now() - _time


month_data = pd.date_range("1/1/1996", "12/31/2023", freq="Q-MAR").astype(str).tolist()


def get_real_date(date, trade_list=trade_date_sse, towards=-1):
    """
    explanation:
        获取真实的交易日期

    params:
        * date->
            含义: 日期
            类型: date
            参数支持: []
        * trade_list->
            含义: 交易列表
            类型: List
            参数支持: []
        * towards->
            含义: 方向， 1 -> 向前, -1 -> 向后
            类型: int
            参数支持: [1， -1]
    """
    date = str(date)[0:10]
    if towards == 1:
        while date not in trade_list:
            date = str(
                datetime.datetime.strptime(str(date)[0:10], "%Y-%m-%d")
                + datetime.timedelta(days=1)
            )[0:10]
        else:
            return str(date)[0:10]
    elif towards == -1:
        while date not in trade_list:
            date = str(
                datetime.datetime.strptime(str(date)[0:10], "%Y-%m-%d")
                - datetime.timedelta(days=1)
            )[0:10]
        else:
            return str(date)[0:10]


def get_real_datelist(start, end):
    """
    explanation:
        取数据的真实区间，当start end中间没有交易日时返回None, None,
        同时返回的时候用 start,end=get_real_datelist

    params:
        * start->
            含义: 开始日期
            类型: date
            参数支持: []
        * end->
            含义: 截至日期
            类型: date
            参数支持: []
    """
    real_start = get_real_date(start, trade_date_sse, 1)
    real_end = get_real_date(end, trade_date_sse, -1)
    if trade_date_sse.index(real_start) > trade_date_sse.index(real_end):
        return None, None
    else:
        return (real_start, real_end)


def get_trade_gap(start, end):
    """
    explanation:
        返回start_day到end_day中间有多少个交易天 算首尾

    params:
        * start->
            含义: 开始日期
            类型: date
            参数支持: []
        * end->
            含义: 截至日期
            类型: date
            参数支持: []
   """
    start, end = get_real_datelist(start, end)
    if start is not None:
        return trade_date_sse.index(end) + 1 - trade_date_sse.index(start)
    else:
        return 0


def now_time():
    return (
        str(
            get_real_date(
                str(datetime.date.today() - datetime.timedelta(days=1)),
                trade_date_sse,
                -1,
            )
        )
        + " 17:00:00"
        if datetime.datetime.now().hour < 15
        else str(get_real_date(str(datetime.date.today()), trade_date_sse, -1))
        + " 15:00:00"
    )


def get_nth_trading_day(date, n=1, way=1):
    """
    得到上一个(n)交易日
    :param  date: 类型 str eg: 2018-11-11
    :param n:  整形
    :return: 字符串 str eg: 2018-11-10
    """
    date = str(date)[0:10]
    if way < 0:
        return get_date_gap(date, n, "lt")
    else:
        return get_date_gap(date, n, "gt")


def get_date_gap(date, gap, methods):
    """
    :param date: 字符串起始日 类型 str eg: 2018-11-11
    :param gap: 整数 间隔多数个交易日
    :param methods:  gt大于 ，gte 大于等于， 小于lt ，小于等于lte ， 等于===
    :return: 字符串 eg：2000-01-01
    """
    try:
        if methods in [">", "gt"]:
            return trade_date_sse[trade_date_sse.index(date) + gap]
        elif methods in [">=", "gte"]:
            return trade_date_sse[trade_date_sse.index(date) + gap - 1]
        elif methods in ["<", "lt"]:
            return trade_date_sse[trade_date_sse.index(date) - gap]
        elif methods in ["<=", "lte"]:
            return trade_date_sse[trade_date_sse.index(date) - gap + 1]
        elif methods in ["==", "=", "eq"]:
            return date
    except Exception:
        return "wrong date"


def get_last_day(date: str, n: int = 1) -> str:
    """
    explanation:
       得到上一个(n)交易日

    params:
        * date->
            含义: 日期
            类型: str
            参数支持: []
        * n->
            含义: 步长
            类型: int
            参数支持: [int]
    """
    date = str(date)[0:10]
    return get_date_gap(date, n, "lt")
