import datetime

import numpy as np
import pandas as pd
import json
from pytdx.exhq import TdxExHq_API
from pytdx.hq import TdxHq_API
from retrying import retry
from abquant.utils.logger import system_log as slog
from abquant.utils.logger import user_log as ulog
from abquant.utils.cache import Cache
from abquant.utils.parallelism import Parallelism
from abquant.utils.market import *
from abquant.config import Setting

global best_ip
best_ip = {
    'stock': {
        'ip': None, 'port': None
    },
    'future': {
        'ip': None, 'port': None
    }
}


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
    null = None
    exclude_ip = {"ip": "1.1.1.1", "port": 7709}
    default_ip = {
        "stock": {"ip": None, "port": None},
        "future": {"ip": None, "port": None},
    }
    alist = []
    alist.append(exclude_ip)

    ipexclude = Setting.get_setting(
        Setting.NETWORK_IP_INI, "IPLIST", "exclude")

    Setting.exclude_from_stock_ip_list(ipexclude)

    ipdefault = Setting.get_setting(
        Setting.NETWORK_IP_INI, "IPLIST", "default")

    ipdefault = eval(ipdefault) if isinstance(ipdefault, str) else ipdefault
    assert isinstance(ipdefault, dict)

    stock_ip_list = Setting.make_stock_ip_list()
    future_ip_list = Setting.make_future_ip_list()

    if ipdefault["stock"]["ip"] == None:
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
    if ipdefault["future"]["ip"] == None:
        best_future_ip = get_ip_list_by_ping(future_ip_list, _type="future")
    else:
        if ping(
            ipdefault["future"]["ip"], ipdefault["future"]["port"], "future"
        ) < datetime.timedelta(0, 1):
            slog.debug("USING DEFAULT FUTURE IP")
            best_future_ip = ipdefault["future"]
        else:
            slog.error(
                "DEFAULT FUTURE IP {} is BAD, RETESTING".format(ipdefault))
            best_future_ip = get_ip_list_by_ping(
                future_ip_list, _type="future")
    ipbest = {"stock": best_stock_ip, "future": best_future_ip}
    Setting.update_setting(Setting.NETWORK_IP_INI,
                           "IPLIST", "default", json.dumps(ipbest))

    slog.debug(
        "=== The BEST SERVER ===\n stock_ip {} future_ip {}".format(
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
            slog.debug("saving ip list to {} cache {}".format(
                _type, len(results)))
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
        ip {[type]} -- [description]
        port {[type]} -- [description]
    Returns:
        [type] -- [description]
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


@retry(stop_max_attempt_number=3, wait_random_min=50, wait_random_max=100)
def get_stock_list(type_="stock", ip=None, port=None):
    ip, port = get_mainmarket_ip(ip, port)
    api = TdxHq_API()
    with api.connect(ip, port):
        data = pd.concat(
            [
                pd.concat(
                    [
                        api.to_df(api.get_security_list(j, i * 1000))
                        .assign(sse="sz" if j == 0 else "sh")
                        .set_index(["code", "sse"], drop=False)
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
        # data.code = data.code.apply(int)
        sz = data.query('sse=="sz"')
        sh = data.query('sse=="sh"')

        sz = sz.assign(sec=sz.code.apply(for_sz))
        sh = sh.assign(sec=sh.code.apply(for_sh))

        if type_ in ["stock", "gp"]:

            return (
                pd.concat([sz, sh], sort=False)
                .query('sec=="stock_cn"')
                .sort_index()
                .assign(name=data["name"].apply(lambda x: str(x)[0:6]))
            )

        elif type_ in ["index", "zs"]:

            return (
                pd.concat([sz, sh], sort=False)
                .query('sec=="index_cn"')
                .sort_index()
                .assign(name=data["name"].apply(lambda x: str(x)[0:6]))
            )
            # .assign(szm=data['name'].apply(lambda x: ''.join([y[0] for y in lazy_pinyin(x)])))\
            # .assign(quanpin=data['name'].apply(lambda x: ''.join(lazy_pinyin(x))))
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
        # .assign(szm=data['name'].apply(lambda x: ''.join([y[0] for y in lazy_pinyin(x)])))\
            #    .assign(quanpin=data['name'].apply(lambda x: ''.join(lazy_pinyin(x))))
