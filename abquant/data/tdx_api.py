# -*- coding: utf-8 -*-
import datetime
import pandas as pd
import json
from pytdx.exhq import TdxExHq_API
from pytdx.hq import TdxHq_API
from retrying import retry
from abquant.utils.logger import system_log as slog
from abquant.utils.cache import Cache
from abquant.utils.parallelism import Parallelism
from abquant.utils.tdx import *
from abquant.utils.tdx import for_sh, for_sz, select_market_code, select_type
from abquant.utils.datetime import *
from abquant.utils.datetime import get_trade_gap, make_datestamp, make_timestamp
from abquant.config import Setting

global best_ip
best_ip = {"stock": {"ip": None, "port": None}, "future": {"ip": None, "port": None}}


def ping(ip: str, port: int = 7709, type_: str = "stock"):
    api = TdxHq_API()
    apix = TdxExHq_API()
    now = datetime.datetime.now()
    try:
        if type_ in ["stock"]:
            with api.connect(ip, port, time_out=0.7):
                res = api.get_security_list(0, 1)

                if res is not None:
                    if len(api.get_security_list(0, 1)) > 800:
                        return datetime.datetime.now() - now
                    else:
                        slog.error("BAD RESPONSE {}".format(ip))
                        return datetime.timedelta(9, 9, 0)
                else:

                    slog.error("BAD RESPONSE {}".format(ip))
                    return datetime.timedelta(9, 9, 0)
        elif type_ in ["future"]:
            with apix.connect(ip, port, time_out=0.7):
                res = apix.get_instrument_count()
                if res is not None:
                    if res > 40000:
                        return datetime.datetime.now() - now
                    else:
                        slog.error("Bad FUTUREIP REPSONSE {}".format(ip))
                        return datetime.timedelta(9, 9, 0)
                else:
                    slog.error("Bad FUTUREIP REPSONSE {}".format(ip))
                    return datetime.timedelta(9, 9, 0)
    except Exception as e:
        if isinstance(e, TypeError):
            slog.error(e)
        else:
            slog.error("BAD RESPONSE {}".format(ip))
        return datetime.timedelta(9, 9, 0)


def select_best_ip():
    slog.debug("Selecting the Best Server IP of TDX.")

    # 删除exclude ip
    exclude_ip = {"ip": "1.1.1.1", "port": 7709}
    default_ip = {
        "stock": {"ip": None, "port": None},
        "future": {"ip": None, "port": None},
    }
    alist = []
    alist.append(exclude_ip)

    ipexclude = Setting.get_setting(Setting.NETWORK_IP_INI, "IPLIST", "exclude")

    Setting.exclude_from_stock_ip_list(ipexclude)

    ipdefault = Setting.get_setting(Setting.NETWORK_IP_INI, "IPLIST", "default")

    ipdefault = eval(ipdefault) if isinstance(ipdefault, str) else ipdefault
    assert isinstance(ipdefault, dict)

    stock_ip_list = Setting.make_stock_ip_list()
    future_ip_list = Setting.make_future_ip_list()

    if ipdefault["stock"]["ip"] is None:
        best_stock_ip = get_ip_list_by_ping(stock_ip_list)
    else:
        if ping(
            ipdefault["stock"]["ip"], ipdefault["stock"]["port"], "stock"
        ) < datetime.timedelta(0, 1):
            slog.debug("USING DEFAULT STOCK IP")
            best_stock_ip = ipdefault["stock"]
        else:
            slog.error("DEFAULT STOCK IP is BAD, RETESTING")
            best_stock_ip = get_ip_list_by_ping(stock_ip_list)
    if ipdefault["future"]["ip"] is None:
        best_future_ip = get_ip_list_by_ping(future_ip_list, _type="future")
    else:
        if ping(
            ipdefault["future"]["ip"], ipdefault["future"]["port"], "future"
        ) < datetime.timedelta(0, 1):
            slog.debug("USING DEFAULT FUTURE IP")
            best_future_ip = ipdefault["future"]
        else:
            slog.error("DEFAULT FUTURE IP {} is BAD, RETESTING".format(ipdefault))
            best_future_ip = get_ip_list_by_ping(future_ip_list, _type="future")
    ipbest = {"stock": best_stock_ip, "future": best_future_ip}
    Setting.update_setting(
        Setting.NETWORK_IP_INI, "IPLIST", "default", json.dumps(ipbest)
    )

    slog.debug(
        "=== The BEST SERVER === stock_ip: {} future_ip: {}".format(
            best_stock_ip["ip"], best_future_ip["ip"]
        )
    )
    return ipbest


def get_ip_list_by_ping(ip_list=[], _type="stock"):
    best_ip = get_ip_list_by_multi_process_ping(ip_list, 1, _type)
    return best_ip[0]


def get_ip_list_by_multi_process_ping(ip_list=[], n=0, _type="stock", cache_age=86400):
    """ 根据ping排序返回可用的ip列表
    2019 04 09  增加_type缓存时间cache_age
    2019 03 31 取消参数filename
    :param ip_list: ip列表
    :param n: 最多返回的ip数量， 当可用ip数量小于n，返回所有可用的ip；n=0时，返回所有可用ip
    :param _type: ip类型
    :param cache_age: ip类型缓存时间（秒），默认为一天（86400秒）
    :return: 可以ping通的ip列表
    """
    cache = Cache()
    results = cache.get(_type)
    if results:
        # read the data from cache
        slog.debug("loading ip list from {} cache.".format(_type))
    else:
        ips = [(x["ip"], x["port"], _type) for x in ip_list]
        ps = Parallelism()
        ps.run(ping, ips)
        data = list(ps.get_results())
        results = []
        for i in range(len(data)):
            # 删除ping不通的数据
            if data[i] < datetime.timedelta(0, 9, 0):
                results.append((data[i], ip_list[i]))
        # 按照ping值从小大大排序
        results = [x[1] for x in sorted(results, key=lambda x: x[0])]
        if _type:
            # store the data as binary data stream
            cache.set(_type, results, age=cache_age)
            slog.debug("saving ip list to {} cache {}".format(_type, len(results)))
    if len(results) > 0:
        if n == 0 and len(results) > 0:
            return results
        else:
            return results[:n]
    else:
        slog.debug("ALL IP PING TIMEOUT!")
        return [{"ip": None, "port": None}]


def get_mainmarket_ip(ip, port):
    """[summary]
    Arguments:
        ip {[type_]} -- [description]
        port {[type_]} -- [description]
    Returns:
        [type_] -- [description]
    """

    global best_ip
    if (
        ip is None
        and port is None
        and best_ip["stock"]["ip"] is None
        and best_ip["stock"]["port"] is None
    ):
        best_ip = select_best_ip()
        ip = best_ip["stock"]["ip"]
        port = best_ip["stock"]["port"]
    elif (
        ip is None
        and port is None
        and best_ip["stock"]["ip"] is not None
        and best_ip["stock"]["port"] is not None
    ):
        ip = best_ip["stock"]["ip"]
        port = best_ip["stock"]["port"]
    else:
        pass
    return ip, port


def get_extensionmarket_ip(ip, port):
    global best_ip
    if (
        ip is None
        and port is None
        and best_ip["future"]["ip"] is None
        and best_ip["future"]["port"] is None
    ):
        best_ip = select_best_ip()
        ip = best_ip["future"]["ip"]
        port = best_ip["future"]["port"]
    elif (
        ip is None
        and port is None
        and best_ip["future"]["ip"] is not None
        and best_ip["future"]["port"] is not None
    ):
        ip = best_ip["future"]["ip"]
        port = best_ip["future"]["port"]
    else:
        pass
    return ip, port


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_stock_list(type_="stock", ip=None, port=None):
    ip, port = get_mainmarket_ip(ip, port)
    api = TdxHq_API()
    with api.connect(ip, port):
        data = pd.concat(
            [
                pd.concat(
                    [
                        api.to_df(api.get_security_list(j, i * 1000)).assign(
                            sse="sz" if j == 0 else "sh"
                        )
                        for i in range(int(api.get_security_count(j) / 1000) + 1)
                    ],
                    axis=0,
                    sort=False,
                )
                for j in range(2)
            ],
            axis=0,
            sort=False,
        )
        data = data.loc[
            :, ["code", "volunit", "decimal_point", "name", "pre_close", "sse"]
        ].set_index(["code", "sse"], drop=False)

        sz = data.query('sse=="sz"')
        sh = data.query('sse=="sh"')

        sz = sz.assign(sec=sz.code.apply(for_sz))
        sh = sh.assign(sec=sh.code.apply(for_sh))

        if type_ in ["stock", "gp", "CS"]:
            return (
                pd.concat([sz, sh], sort=False)
                .query('sec=="stock_cn"')
                .sort_index()
                .assign(name=data["name"].apply(lambda x: str(x)[0:6]))
            )
        elif type_ in ["index", "zs", "INDX"]:
            return (
                pd.concat([sz, sh], sort=False)
                .query('sec=="index_cn"')
                .sort_index()
                .assign(name=data["name"].apply(lambda x: str(x)[0:6]))
            )
        elif type_ in ["etf", "ETF"]:
            return (
                pd.concat([sz, sh], sort=False)
                .query('sec=="etf_cn"')
                .sort_index()
                .assign(name=data["name"].apply(lambda x: str(x)[0:6]))
            )
        else:
            return data.assign(code=data["code"].apply(lambda x: str(x))).assign(
                name=data["name"].apply(lambda x: str(x)[0:6])
            )


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_security_bars(code, _type, lens, ip=None, port=None):
    """按bar长度推算数据
    Arguments:
        code {[type_]} -- [description]
        _type {[type_]} -- [description]
        lens {[type_]} -- [description]
    Keyword Arguments:
        ip {[type_]} -- [description] (default: {best_ip})
        port {[type_]} -- [description] (default: {7709})
    Returns:
        [type_] -- [description]
    """
    ip, port = get_mainmarket_ip(ip, port)
    api = TdxHq_API()
    with api.connect(ip, port):
        data = pd.concat(
            [
                api.to_df(
                    api.get_security_bars(
                        select_type(_type),
                        select_market_code(code),
                        code,
                        (i - 1) * 800,
                        800,
                    )
                )
                for i in range(1, int(lens / 800) + 2)
            ],
            axis=0,
            sort=False,
        )
        data = (
            data.drop(["year", "month", "day", "hour", "minute"], axis=1, inplace=False)
            .assign(
                datetime=pd.to_datetime(data["datetime"]),
                date=data["datetime"].apply(lambda x: str(x)[0:10]),
                date_stamp=data["datetime"].apply(lambda x: make_datestamp(x)),
                time_stamp=data["datetime"].apply(lambda x: make_timestamp(x)),
                type=_type,
                code=str(code),
            )
            .set_index("datetime", drop=False, inplace=False)
            .tail(lens)
        )
        if not data.dropna().empty:
            return data
        else:
            return None


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_stock_day(
    code, start_date, end_date, if_fq="00", frequence="day", ip=None, port=None
):
    """获取日线及以上级别的数据
    Arguments:
        code {str:6} -- code 是一个单独的code 6位长度的str
        start_date {str:10} -- 10位长度的日期 比如'2017-01-01'
        end_date {str:10} -- 10位长度的日期 比如'2018-01-01'
    Keyword Arguments:
        if_fq {str} -- '00'/'bfq' -- 不复权 '01'/'qfq' -- 前复权 '02'/'hfq' -- 后复权 '03'/'ddqfq' -- 定点前复权 '04'/'ddhfq' --定点后复权
        frequency {str} -- day/week/month/quarter/year 也可以是简写 D/W/M/Q/Y
        ip {str} -- [description] (default: None) ip可以通过select_best_ip()函数重新获取
        port {int} -- [description] (default: {None})
    Returns:
        pd.DataFrame/None -- 返回的是dataframe,如果出错比如只获取了一天,而当天停牌,返回None
    Exception:
        如果出现网络问题/服务器拒绝, 会出现socket:time out 尝试再次获取/更换ip即可, 本函数不做处理
    """
    ip, port = get_mainmarket_ip(ip, port)
    api = TdxHq_API()
    try:
        with api.connect(ip, port, time_out=0.7):

            if frequence in ["day", "d", "D", "DAY", "Day"]:
                frequence = 9
            elif frequence in ["w", "W", "Week", "week"]:
                frequence = 5
            elif frequence in ["month", "M", "m", "Month"]:
                frequence = 6
            elif frequence in ["quarter", "Q", "Quarter", "q"]:
                frequence = 10
            elif frequence in ["y", "Y", "year", "Year"]:
                frequence = 11
            start_date = str(start_date)[0:10]
            today_ = datetime.date.today()
            lens = get_trade_gap(start_date, today_)

            data = pd.concat(
                [
                    api.to_df(
                        api.get_security_bars(
                            frequence,
                            select_market_code(code),
                            code,
                            (int(lens / 800) - i) * 800,
                            800,
                        )
                    )
                    for i in range(int(lens / 800) + 1)
                ],
                axis=0,
                sort=False,
            )
            # 这里的问题是: 如果只取了一天的股票,而当天停牌, 那么就直接返回None了
            if len(data) < 1:
                return None
            data = data[data["open"] != 0]

            data = data.assign(
                date=data["datetime"].apply(lambda x: str(x[0:10])),
                code=str(code),
                date_stamp=data["datetime"].apply(
                    lambda x: make_datestamp(str(x)[0:10])
                ),
            ).set_index("date", drop=False, inplace=False)

            end_date = str(end_date)[0:10]
            data = data.drop(
                ["year", "month", "day", "hour", "minute", "datetime"], axis=1
            )[start_date:end_date]
            if if_fq in ["00", "bfq"]:
                return data
            else:
                slog.warn("CURRENTLY NOT SUPPORT REALTIME FUQUAN")
                return None
    except Exception as e:
        slog.error(e)


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_index_day(code, start_date, end_date, frequence="day", ip=None, port=None):
    """指数日线
    1- sh
    0 -sz
    Arguments:
        code {[type_]} -- [description]
        start_date {[type_]} -- [description]
        end_date {[type_]} -- [description]
    Keyword Arguments:
        frequence {str} -- [description] (default: {'day'})
        ip {[type_]} -- [description] (default: {None})
        port {[type_]} -- [description] (default: {None})
    Returns:
        [type_] -- [description]
    """

    ip, port = get_mainmarket_ip(ip, port)
    api = TdxHq_API()
    if frequence in ["day", "d", "D", "DAY", "Day"]:
        frequence = 9
    elif frequence in ["w", "W", "Week", "week"]:
        frequence = 5
    elif frequence in ["month", "M", "m", "Month"]:
        frequence = 6
    elif frequence in ["Q", "Quarter", "q"]:
        frequence = 10
    elif frequence in ["y", "Y", "year", "Year"]:
        frequence = 11

    with api.connect(ip, port):
        start_date = str(start_date)[0:10]
        today_ = datetime.date.today()
        lens = get_trade_gap(start_date, today_)

        if str(code)[0] in ["5", "1"]:  # ETF
            data = pd.concat(
                [
                    api.to_df(
                        api.get_security_bars(
                            frequence,
                            1 if str(code)[0] in ["0", "8", "9", "5"] else 0,
                            code,
                            (int(lens / 800) - i) * 800,
                            800,
                        )
                    )
                    for i in range(int(lens / 800) + 1)
                ],
                axis=0,
                sort=False,
            )
        else:
            data = pd.concat(
                [
                    api.to_df(
                        api.get_index_bars(
                            frequence,
                            1 if str(code)[0] in ["0", "8", "9", "5"] else 0,
                            code,
                            (int(lens / 800) - i) * 800,
                            800,
                        )
                    )
                    for i in range(int(lens / 800) + 1)
                ],
                axis=0,
                sort=False,
            )
        data = (
            data.assign(date=data["datetime"].apply(lambda x: str(x[0:10])))
            .assign(code=str(code))
            .assign(
                date_stamp=data["datetime"].apply(
                    lambda x: make_datestamp(str(x)[0:10])
                )
            )
            .set_index("date", drop=False, inplace=False)
            .assign(code=code)
            .drop(["year", "month", "day", "hour", "minute", "datetime"], axis=1)
        )
        data = data.loc[start_date:end_date]
        return data.assign(date=data["date"].apply(lambda x: str(x)[0:10]))


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_stock_min(code, start, end, frequence="1min", ip=None, port=None):
    ip, port = get_mainmarket_ip(ip, port)
    api = TdxHq_API()
    type_ = ""
    start_date = str(start)[0:10]
    today_ = datetime.date.today()
    lens = get_trade_gap(start_date, today_)
    if str(frequence) in ["5", "5m", "5min", "five"]:
        frequence, type_ = 0, "5min"
        lens = 48 * lens
    elif str(frequence) in ["1", "1m", "1min", "one"]:
        frequence, type_ = 8, "1min"
        lens = 240 * lens
    elif str(frequence) in ["15", "15m", "15min", "fifteen"]:
        frequence, type_ = 1, "15min"
        lens = 16 * lens
    elif str(frequence) in ["30", "30m", "30min", "half"]:
        frequence, type_ = 2, "30min"
        lens = 8 * lens
    elif str(frequence) in ["60", "60m", "60min", "1h"]:
        frequence, type_ = 3, "60min"
        lens = 4 * lens
    if lens > 20800:
        lens = 20800
    with api.connect(ip, port):

        data = pd.concat(
            [
                api.to_df(
                    api.get_security_bars(
                        frequence,
                        select_market_code(str(code)),
                        str(code),
                        (int(lens / 800) - i) * 800,
                        800,
                    )
                )
                for i in range(int(lens / 800) + 1)
            ],
            axis=0,
            sort=False,
        )
        data = (
            data.drop(["year", "month", "day", "hour", "minute"], axis=1, inplace=False)
            .assign(
                datetime=pd.to_datetime(data["datetime"]),
                code=str(code),
                date=data["datetime"].apply(lambda x: str(x)[0:10]),
                date_stamp=data["datetime"].apply(lambda x: make_datestamp(x)),
                time_stamp=data["datetime"].apply(lambda x: make_timestamp(x)),
                type=type_,
            )
            .set_index("datetime", drop=False, inplace=False)[start:end]
        )
        return data.assign(datetime=data["datetime"].apply(lambda x: str(x)))


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_stock_latest(code, frequence="day", ip=None, port=None):
    ip, port = get_mainmarket_ip(ip, port)
    code = [code] if isinstance(code, str) else code
    api = TdxHq_API(multithread=True)

    if frequence in ["w", "W", "Week", "week"]:
        frequence = 5
    elif frequence in ["month", "M", "m", "Month"]:
        frequence = 6
    elif frequence in ["Q", "Quarter", "q"]:
        frequence = 10
    elif frequence in ["y", "Y", "year", "Year"]:
        frequence = 11
    elif frequence in ["5", "5m", "5min", "five"]:
        frequence = 0
    elif frequence in ["1", "1m", "1min", "one"]:
        frequence = 8
    elif frequence in ["15", "15m", "15min", "fifteen"]:
        frequence = 1
    elif frequence in ["30", "30m", "30min", "half"]:
        frequence = 2
    elif frequence in ["60", "60m", "60min", "1h"]:
        frequence = 3
    else:
        frequence = 9

    with api.connect(ip, port):
        data = pd.concat(
            [
                api.to_df(
                    api.get_security_bars(
                        frequence, select_market_code(item), item, 0, 1
                    )
                ).assign(code=item)
                for item in code
            ],
            axis=0,
            sort=False,
        )
        return (
            data.assign(
                date=pd.to_datetime(data["datetime"].apply(lambda x: x[0:10])),
                date_stamp=data["datetime"].apply(
                    lambda x: make_datestamp(str(x[0:10]))
                ),
            )
            .set_index("date", drop=False)
            .drop(["year", "month", "day", "hour", "minute", "datetime"], axis=1)
        )


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_stock_realtime(code=["000001", "000002"], ip=None, port=None):
    ip, port = get_mainmarket_ip(ip, port)
    # reversed_bytes9 --> 涨速
    # active1,active2 --> 活跃度
    # reversed_bytes1 --> -价格*100
    # vol 总量 cur_vol 现量
    # amount 总金额
    # s_vol 内盘 b_vol 外盘
    # reversed_bytes2 市场
    # # reversed_bytes0 时间

    api = TdxHq_API()
    __data = pd.DataFrame()
    with api.connect(ip, port):
        code = [code] if isinstance(code, str) else code
        for id_ in range(int(len(code) / 80) + 1):
            __data = __data.append(
                api.to_df(
                    api.get_security_quotes(
                        [
                            (select_market_code(x), x)
                            for x in code[80 * id_ : 80 * (id_ + 1)]
                        ]
                    )
                )
            )
            __data = __data.assign(
                datetime=datetime.datetime.now(),
                servertime=__data["reversed_bytes0"].apply(QA_util_tdxtimestamp),
            )
            # __data['rev']
        data = __data[
            [
                "datetime",
                "servertime",
                "active1",
                "active2",
                "last_close",
                "code",
                "open",
                "high",
                "low",
                "price",
                "cur_vol",
                "s_vol",
                "b_vol",
                "vol",
                "ask1",
                "ask_vol1",
                "bid1",
                "bid_vol1",
                "ask2",
                "ask_vol2",
                "bid2",
                "bid_vol2",
                "ask3",
                "ask_vol3",
                "bid3",
                "bid_vol3",
                "ask4",
                "ask_vol4",
                "bid4",
                "bid_vol4",
                "ask5",
                "ask_vol5",
                "bid5",
                "bid_vol5",
            ]
        ]
        return data.set_index(["datetime", "code"])


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_index_realtime(code=["000001"], ip=None, port=None):
    ip, port = get_mainmarket_ip(ip, port)
    # reversed_bytes9 --> 涨速
    # active1,active2 --> 活跃度
    # reversed_bytes1 --> -价格*100
    # vol 总量 cur_vol 现量
    # amount 总金额
    # s_vol 内盘 b_vol 外盘
    # reversed_bytes2 市场
    # # reversed_bytes0 时间

    api = TdxHq_API()
    __data = pd.DataFrame()
    with api.connect(ip, port):
        code = [code] if isinstance(code, str) else code
        for id_ in range(int(len(code) / 80) + 1):
            __data = __data.append(
                api.to_df(
                    api.get_security_quotes(
                        [
                            (_select_index_code(x), x)
                            for x in code[80 * id_ : 80 * (id_ + 1)]
                        ]
                    )
                )
            )
            __data = __data.assign(
                datetime=datetime.datetime.now(),
                servertime=__data["reversed_bytes0"].apply(QA_util_tdxtimestamp),
            )
            # __data['rev']
        data = __data[
            [
                "datetime",
                "servertime",
                "active1",
                "active2",
                "last_close",
                "code",
                "open",
                "high",
                "low",
                "price",
                "cur_vol",
                "s_vol",
                "b_vol",
                "vol",
                "ask1",
                "ask_vol1",
                "bid1",
                "bid_vol1",
                "ask2",
                "ask_vol2",
                "bid2",
                "bid_vol2",
                "ask3",
                "ask_vol3",
                "bid3",
                "bid_vol3",
                "ask4",
                "ask_vol4",
                "bid4",
                "bid_vol4",
                "ask5",
                "ask_vol5",
                "bid5",
                "bid_vol5",
            ]
        ]
        return data.set_index(["datetime", "code"])


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_index_min(code, start, end, frequence="1min", ip=None, port=None):
    "指数分钟线"
    ip, port = get_mainmarket_ip(ip, port)
    api = TdxHq_API()
    type_ = ""

    start_date = str(start)[0:10]
    today_ = datetime.date.today()
    lens = get_trade_gap(start_date, today_)
    if str(frequence) in ["5", "5m", "5min", "five"]:
        frequence, type_ = 0, "5min"
        lens = 48 * lens
    elif str(frequence) in ["1", "1m", "1min", "one"]:
        frequence, type_ = 8, "1min"
        lens = 240 * lens
    elif str(frequence) in ["15", "15m", "15min", "fifteen"]:
        frequence, type_ = 1, "15min"
        lens = 16 * lens
    elif str(frequence) in ["30", "30m", "30min", "half"]:
        frequence, type_ = 2, "30min"
        lens = 8 * lens
    elif str(frequence) in ["60", "60m", "60min", "1h"]:
        frequence, type_ = 3, "60min"
        lens = 4 * lens

    if lens > 20800:
        lens = 20800
    with api.connect(ip, port):

        if str(code)[0] in ["5", "1"]:  # ETF
            data = pd.concat(
                [
                    api.to_df(
                        api.get_security_bars(
                            frequence,
                            1 if str(code)[0] in ["0", "8", "9", "5"] else 0,
                            code,
                            (int(lens / 800) - i) * 800,
                            800,
                        )
                    )
                    for i in range(int(lens / 800) + 1)
                ],
                axis=0,
                sort=False,
            )
        else:
            data = pd.concat(
                [
                    api.to_df(
                        api.get_index_bars(
                            frequence,
                            1 if str(code)[0] in ["0", "8", "9", "5"] else 0,
                            code,
                            (int(lens / 800) - i) * 800,
                            800,
                        )
                    )
                    for i in range(int(lens / 800) + 1)
                ],
                axis=0,
                sort=False,
            )
        data = (
            data.assign(datetime=pd.to_datetime(data["datetime"]), code=str(code))
            .drop(["year", "month", "day", "hour", "minute"], axis=1, inplace=False)
            .assign(
                code=code,
                date=data["datetime"].apply(lambda x: str(x)[0:10]),
                date_stamp=data["datetime"].apply(lambda x: make_datestamp(x)),
                time_stamp=data["datetime"].apply(lambda x: make_timestamp(x)),
                type=type_,
            )
            .set_index("datetime", drop=False, inplace=False)[start:end]
        )
        # data
        return data.assign(datetime=data["datetime"].apply(lambda x: str(x)))


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_index_list(ip=None, port=None):
    """获取指数列表
    Keyword Arguments:
        ip {[type_]} -- [description] (default: {None})
        port {[type_]} -- [description] (default: {None})
    Returns:
        [type_] -- [description]
    """

    ip, port = get_mainmarket_ip(ip, port)
    api = TdxHq_API()
    with api.connect(ip, port):
        data = pd.concat(
            [
                pd.concat(
                    [
                        api.to_df(api.get_security_list(j, i * 1000)).assign(
                            sse="sz" if j == 0 else "sh"
                        )
                        for i in range(int(api.get_security_count(j) / 1000) + 1)
                    ],
                    axis=0,
                    sort=False,
                )
                for j in range(2)
            ],
            axis=0,
            sort=False,
        )
        data = data.loc[
            :, ["code", "volunit", "decimal_point", "name", "pre_close", "sse"]
        ].set_index(["code", "sse"], drop=False)

        sz = data.query('sse=="sz"')
        sh = data.query('sse=="sh"')

        sz = sz.assign(sec=sz.code.apply(for_sz))
        sh = sh.assign(sec=sh.code.apply(for_sh))
        return (
            pd.concat([sz, sh], sort=False)
            .query('sec=="index_cn"')
            .sort_index()
            .assign(name=data["name"].apply(lambda x: str(x)[0:6]))
        )


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_stock_xdxr(code, ip=None, port=None):
    "除权除息"
    ip, port = get_mainmarket_ip(ip, port)
    api = TdxHq_API()
    market_code = select_market_code(code)
    with api.connect(ip, port):
        category = {
            "1": "除权除息",
            "2": "送配股上市",
            "3": "非流通股上市",
            "4": "未知股本变动",
            "5": "股本变化",
            "6": "增发新股",
            "7": "股份回购",
            "8": "增发新股上市",
            "9": "转配股上市",
            "10": "可转债上市",
            "11": "扩缩股",
            "12": "非流通股缩股",
            "13": "送认购权证",
            "14": "送认沽权证",
        }
        data = api.to_df(api.get_xdxr_info(market_code, code))
        if len(data) >= 1:
            data = (
                data.assign(date=pd.to_datetime(data[["year", "month", "day"]]))
                .drop(["year", "month", "day"], axis=1)
                .assign(
                    category_meaning=data["category"].apply(lambda x: category[str(x)])
                )
                .assign(code=str(code))
                .rename(
                    index=str,
                    columns={
                        "panhouliutong": "liquidity_after",
                        "panqianliutong": "liquidity_before",
                        "houzongguben": "shares_after",
                        "qianzongguben": "shares_before",
                    },
                )
                .set_index("date", drop=False, inplace=False)
            )
            return data.assign(date=data["date"].apply(lambda x: str(x)[0:10]))
        else:
            return None


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_stock_info(code, ip=None, port=None):
    "股票基本信息"
    ip, port = get_mainmarket_ip(ip, port)
    api = TdxHq_API()
    market_code = select_market_code(code)
    with api.connect(ip, port):
        return api.to_df(api.get_finance_info(market_code, code))


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_stock_block(ip=None, port=None):
    "板块数据"
    ip, port = get_mainmarket_ip(ip, port)
    api = TdxHq_API()
    with api.connect(ip, port):

        data = pd.concat(
            [
                api.to_df(api.get_and_parse_block_info("block_gn.dat")).assign(
                    type_="gn"
                ),
                api.to_df(api.get_and_parse_block_info("block.dat")).assign(type_="yb"),
                api.to_df(api.get_and_parse_block_info("block_zs.dat")).assign(
                    type_="zs"
                ),
                api.to_df(api.get_and_parse_block_info("block_fg.dat")).assign(
                    type_="fg"
                ),
            ],
            sort=False,
        )

        if len(data) > 10:
            return (
                data.assign(source="tdx")
                .drop(["block_type", "code_index"], axis=1)
                .set_index("code", drop=False, inplace=False)
                .drop_duplicates()
            )
        else:
            slog.debug("Wrong with fetch block ")


def my_name():
    return "TDX"
