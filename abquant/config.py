# -*- coding: utf-8 -*-
from __future__ import annotations

import codecs
import configparser
import os
import platform
from pathlib import Path
from typing import Dict, List, Union

import yaml

import simplejson as json
from abquant.utils.logger import user_log as ulog

xdg_config_home = (
    Path(os.getenv("XDG_CONFIG_HOME", ""))
    if "Windows" not in platform.platform()
    else Path("C:")
)

xdg_data_home = (
    Path(os.getenv("XDG_DATA_HOME", ""))
    if "Windows" not in platform.platform()
    else Path("C:")
)

abquant_home = (
    xdg_config_home / "abquant"
    if xdg_config_home.exists()
    else Path.home() / ".abquant"
)

abquant_data_home = (
    xdg_data_home / "abquant" if xdg_data_home.exists() else Path.home() / ".abquant"
)


class Setting(object):
    INFO_IP_JSON = abquant_home / "config/info_ip.json"
    MONGODB_INI = abquant_home / "config/mongodb.ini"
    STOCK_IP_JSON = abquant_home / "config/stock_ip.json"
    FUTURE_IP_JSON = abquant_home / "config/future_ip.json"
    NETWORK_IP_INI = abquant_home / "config/network.ini"
    ERROR_CODES_JSON = abquant_home / "log/error_codes.json"
    DOWNLOAD_PATH = abquant_data_home / "download"

    config = configparser.ConfigParser()
    info_ip_list: List[Dict[str, Union[str, int]]] = []
    stock_ip_list: List[Dict[str, Union[str, int]]] = []
    future_ip_list: List[Dict[str, Union[str, int]]] = []

    DBNAME = ""

    def __init__(self, uri=None):
        self.mongo_uri = uri or Setting.get_mongo()
        self.username = None
        self.password = None

    @staticmethod
    def make_download_path():
        if not Setting.DOWNLOAD_PATH.exists():
            Path.mkdir(Setting.DOWNLOAD_PATH)

    @staticmethod
    def get_mongo():
        res = ""
        if not Setting.MONGODB_INI.exists():
            ulog.error("{} does not exists.".format(Setting.MONGODB_INI))
            return
        try:
            Setting.config = Setting.get_config(Setting.MONGODB_INI)
            Setting.DBNAME = Setting.config.get("product", "DatabaseName")
            host = Setting.config.get("product", "HostName")
            host = host if host else "localhost"
            port = Setting.config.get("product", "Port")
            port = port if port else "27017"
            res = "mongodb://{}:{}".format(host, port)
        except:
            res = "mongodb://localhost:27017"
        return res

    @staticmethod
    def get_config(ini):
        """
        Returns the config object
        """
        if not ini.exists():
            ulog.error("{} does not exists.".format(ini))
            return
        Setting.config.read(ini)
        return Setting.config

    @staticmethod
    def get_setting(ini, section, key):
        """
        Print out a key
        """
        if not ini.exists():
            ulog.error("{} does not exists.".format(ini))
            return
        Setting.config = Setting.get_config(ini)
        value = Setting.config.get(section, key)
        msg = "{section} {key} is {value}".format(section=section, key=key, value=value)
        # ulog.info(msg)
        return value

    @staticmethod
    def update_setting(ini, section, key, value):
        """
        Update a key
        """
        if not ini.exists():
            ulog.error("{} does not exists.".format(ini))
            return
        Setting.config = Setting.get_config(ini)
        Setting.config.set(section, key, value)
        with open(ini, "w") as config_file:
            Setting.config.write(config_file)

    @staticmethod
    def env_config():
        return os.environ.get("MONGOURI", None)

    @staticmethod
    def make_info_ip_list():
        if Setting.INFO_IP_JSON.exists():
            with open(Setting.INFO_IP_JSON, "r") as f:
                Setting.info_ip_list = json.load(f)
                return Setting.info_ip_list
        else:
            Setting.info_ip_list = [
                {"ip": "101.227.73.20", "port": 7709},
                {"ip": "101.227.77.254", "port": 7709},
                {"ip": "114.80.63.12", "port": 7709},
                {"ip": "114.80.63.35", "port": 7709},
                {"ip": "115.238.56.198", "port": 7709},
                {"ip": "115.238.90.165", "port": 7709},
                {"ip": "124.160.88.183", "port": 7709},
                {"ip": "14.215.128.18", "port": 7709},
                {"ip": "180.153.18.170", "port": 7709},
                {"ip": "180.153.18.171", "port": 7709},
                {"ip": "180.153.39.51", "port": 7709},
                {"ip": "202.108.253.130", "port": 7709},
                {"ip": "202.108.253.131", "port": 7709},
                {"ip": "218.108.47.69", "port": 7709},
                {"ip": "218.108.98.244", "port": 7709},
                {"ip": "218.75.126.9", "port": 7709},
                {"ip": "221.231.141.60", "port": 7709},
                {"ip": "59.173.18.140", "port": 7709},
                {"ip": "60.12.136.250", "port": 7709},
                {"ip": "60.28.23.80", "port": 7709},
                # added 2022-11-28 from tdx
                {"ip": "124.71.187.122", "port": 7709, "name": "上海双线主站14"},
                {"ip": "119.97.185.59", "port": 7709, "name": "武汉电信主站1"},
                {"ip": "47.107.64.168", "port": 7709, "name": "深圳双线主站7"},
                {"ip": "124.70.75.113", "port": 7709, "name": "北京双线主站4"},
                {"ip": "124.71.9.153", "port": 7709, "name": "广州双线主站4"},
                {"ip": "123.60.84.66", "port": 7709, "name": "上海双线主站15"},
                {"ip": "47.107.228.47", "port": 7719, "name": "深圳双线主站8"},
                {"ip": "120.46.186.223", "port": 7709, "name": "北京双线主站5"},
                {"ip": "124.70.22.210", "port": 7709, "name": "北京双线主站6"},
                {"ip": "139.9.133.247", "port": 7709, "name": "北京双线主站7"},
                {"ip": "116.205.163.254", "port": 7709, "name": "广州双线主站5"},
                {"ip": "116.205.171.132", "port": 7709, "name": "广州双线主站6"},
                {"ip": "116.205.183.150", "port": 7709, "name": "广州双线主站7"},
                # added 20190222 from tdx
                {"ip": "106.14.95.149", "port": 7711, "name": "上海双线资讯主站"},
                {"ip": "112.74.214.43", "port": 7711, "name": "深圳双线资讯主站1"},
                {"ip": "113.105.142.162", "port": 7711, "name": "东莞电信资讯主站"},
                {"ip": "119.97.185.10", "port": 7711, "name": "武汉电信资讯主站2"},
                {"ip": "119.97.185.6", "port": 7711, "name": "武汉电信资讯主站1"},
                {"ip": "120.24.0.77", "port": 7711, "name": "深圳双线资讯主站2"},
                {"ip": "120.79.212.229", "port": 7711, "name": "深圳双线资讯主站3"},
                {"ip": "47.107.75.159", "port": 7711, "name": "深圳双线资讯主站4"},
                {"ip": "47.92.127.181", "port": 7711, "name": "北京双线资讯主站"},
                {"ip": "113.105.142.162", "port": 7721},
                {"ip": "23.129.245.199", "port": 7721},
            ]
            with open(Setting.INFO_IP_JSON, "w") as f:
                json.dump(Setting.info_ip_list, f)

    @staticmethod
    def make_stock_ip_list():
        if Path(Setting.STOCK_IP_JSON).exists():
            with open(Setting.STOCK_IP_JSON, "r") as f:
                Setting.stock_ip_list = json.load(f)
                return Setting.stock_ip_list
        else:
            Setting.stock_ip_list = [
                # added 20190222 from tdx
                {"ip": "124.71.187.122", "port": 7709, "name": "上海双线主站14"},
                {"ip": "106.120.74.86", "port": 7711, "name": "北京行情主站1"},
                {"ip": "113.105.73.88", "port": 7709, "name": "深圳行情主站"},
                {"ip": "113.105.73.88", "port": 7711, "name": "深圳行情主站"},
                {"ip": "114.80.80.222", "port": 7711, "name": "上海行情主站"},
                {"ip": "117.184.140.156", "port": 7711, "name": "移动行情主站"},
                {"ip": "119.147.171.206", "port": 443, "name": "广州行情主站"},
                {"ip": "119.147.171.206", "port": 80, "name": "广州行情主站"},
                {"ip": "218.108.50.178", "port": 7711, "name": "杭州行情主站"},
                {"ip": "221.194.181.176", "port": 7711, "name": "北京行情主站2"},
                # origin
                {"ip": "106.120.74.86", "port": 7709},  # 北京
                {"ip": "112.95.140.74", "port": 7709},
                {"ip": "112.95.140.92", "port": 7709},
                {"ip": "112.95.140.93", "port": 7709},
                {"ip": "113.05.73.88", "port": 7709},  # 深圳
                {"ip": "114.67.61.70", "port": 7709},
                {"ip": "114.80.149.19", "port": 7709},
                {"ip": "114.80.149.22", "port": 7709},
                {"ip": "114.80.149.84", "port": 7709},
                {"ip": "114.80.80.222", "port": 7709},  # 上海
                {"ip": "115.238.56.198", "port": 7709},
                {"ip": "115.238.90.165", "port": 7709},
                {"ip": "117.184.140.156", "port": 7709},  # 移动
                {"ip": "119.147.164.60", "port": 7709},  # 广州
                {"ip": "119.147.171.206", "port": 7709},  # 广州
                {"ip": "119.29.51.30", "port": 7709},
                {"ip": "121.14.104.70", "port": 7709},
                {"ip": "121.14.104.72", "port": 7709},
                {"ip": "121.14.110.194", "port": 7709},  # 深圳
                {"ip": "121.14.2.7", "port": 7709},
                {"ip": "123.125.108.23", "port": 7709},
                {"ip": "123.125.108.24", "port": 7709},
                {"ip": "124.160.88.183", "port": 7709},
                {"ip": "180.153.18.17", "port": 7709},
                {"ip": "180.153.18.170", "port": 7709},
                {"ip": "180.153.18.171", "port": 7709},
                {"ip": "180.153.39.51", "port": 7709},
                {"ip": "218.108.47.69", "port": 7709},
                {"ip": "218.108.50.178", "port": 7709},  # 杭州
                {"ip": "218.108.98.244", "port": 7709},
                {"ip": "218.75.126.9", "port": 7709},
                {"ip": "218.9.148.108", "port": 7709},
                {"ip": "221.194.181.176", "port": 7709},  # 北京
                {"ip": "59.173.18.69", "port": 7709},
                {"ip": "60.12.136.250", "port": 7709},
                {"ip": "60.191.117.167", "port": 7709},
                {"ip": "60.28.29.69", "port": 7709},
                {"ip": "61.135.142.73", "port": 7709},
                {"ip": "61.135.142.88", "port": 7709},  # 北京
                {"ip": "61.152.107.168", "port": 7721},
                {"ip": "61.152.249.56", "port": 7709},  # 上海
                {"ip": "61.153.144.179", "port": 7709},
                {"ip": "61.153.209.138", "port": 7709},
                {"ip": "61.153.209.139", "port": 7709},
                {"ip": "hq.cjis.cn", "port": 7709},
                {"ip": "hq1.daton.com.cn", "port": 7709},
                {"ip": "jstdx.gtjas.com", "port": 7709},
                {"ip": "shtdx.gtjas.com", "port": 7709},
                {"ip": "sztdx.gtjas.com", "port": 7709},
                {"ip": "113.105.142.162", "port": 7721},
                {"ip": "23.129.245.199", "port": 7721},
            ]
            with open(Setting.STOCK_IP_JSON, "w") as f:
                json.dump(Setting.stock_ip_list, f)

    @staticmethod
    def make_future_ip_list():
        if os.path.exists(Setting.FUTURE_IP_JSON):
            with open(Setting.FUTURE_IP_JSON, "r") as f:
                Setting.future_ip_list = json.load(f)
                return Setting.future_ip_list
        else:
            Setting.future_ip_list = [
                # origin
                {"ip": "106.14.95.149", "port": 7727, "name": "扩展市场上海双线"},
                {"ip": "112.74.214.43", "port": 7727, "name": "扩展市场深圳双线1"},
                # {"ip": "113.105.142.136", "port": 443, "name": "扩展市场东莞主站"},
                {"ip": "119.147.86.171", "port": 7727, "name": "扩展市场深圳主站"},
                {"ip": "119.97.185.5", "port": 7727, "name": "扩展市场武汉主站1"},
                {"ip": "120.24.0.77", "port": 7727, "name": "扩展市场深圳双线2"},
                {"ip": "124.74.236.94", "port": 7721},
                {"ip": "14.215.128.18", "port": 7721, "name": "华泰证券深圳电信"},
                {"ip": "139.219.103.190", "port": 7721, "name": "华泰证券云行情"},
                {"ip": "202.103.36.71", "port": 443, "name": "扩展市场武汉主站2"},
                {"ip": "47.92.127.181", "port": 7727, "name": "扩展市场北京主站"},
                {"ip": "59.175.238.38", "port": 7727, "name": "扩展市场武汉主站3"},
                {"ip": "61.152.107.141", "port": 7727, "name": "扩展市场上海主站1"},
                {"ip": "61.152.107.171", "port": 7727, "name": "扩展市场上海主站2"},
                # {"ip": "124.74.236.94","port": 7721},
                # {"ip": "218.80.248.229", "port": 7721},
                # {"ip": "58.246.109.27", "port": 7721},
                # added 20190222 from tdx
                {"ip": "119.147.86.171", "port": 7721, "name": "扩展市场深圳主站"},
                {"ip": "47.107.75.159", "port": 7727, "name": "扩展市场深圳双线3"},
            ]
            with open(Setting.FUTURE_IP_JSON, "w") as f:
                json.dump(Setting.future_ip_list, f)

    @staticmethod
    def exclude_from_stock_ip_list(exclude_ip_list):
        """从stock_ip_list删除列表exclude_ip_list中的ip
        从stock_ip_list删除列表future_ip_list中的ip

        :param exclude_ip_list:  需要删除的ip_list
        :return: None
        """
        Setting.stock_ip_list = Setting.make_stock_ip_list()
        Setting.future_ip_list = Setting.make_future_ip_list()
        for exc in exclude_ip_list:
            if exc in Setting.stock_ip_list:
                Setting.stock_ip_list.remove(exc)

        # 扩展市场
        for exc in exclude_ip_list:
            if exc in Setting.future_ip_list:
                Setting.future_ip_list.remove(exc)

    @staticmethod
    def load_yaml(path):
        with codecs.open(path, encoding="utf-8") as f:
            return yaml.load(f)

    @staticmethod
    def load_json(path):
        with codecs.open(path, encoding="utf-8") as f:
            return json.loads(f.read())


"""
["121.14.110.210", "119.147.212.76", "113.105.73.86", "119.147.171.211", "119.147.164.57", "119.147.164.58", "61.49.50.180", "61.49.50.181",
"61.135.142.85", "61.135.149.181", "114.80.80.210", "222.73.49.15", "221.194.181.176"]
"""
