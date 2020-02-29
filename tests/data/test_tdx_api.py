from abquant.data.tdx_api import *
from abquant.utils.logger import user_log as ulog
import time


def test_ping_ok():
    ip = "180.153.18.170"
    td = ping(ip)
    ulog.debug(td)
    expected = 1
    assert td.total_seconds() < expected


def test_ping_fail():
    ip = "1.1.1.1"
    td = ping(ip)
    ulog.debug(td)
    expected = 1
    assert td.total_seconds() > expected


def test_select_best_ip():
    actual = select_best_ip()
    ulog.debug(actual)
    expected = 7709
    assert actual.get("stock").get("port") == expected


def test_get_mainmarket_ip():
    ip, port = None, None
    ip_, port_ = get_mainmarket_ip(ip, port)
    ulog.debug(ip_)
    ulog.debug(port_)
    expected = 7709
    assert port_ == expected


def test_get_extensionmarket_ip():
    ip, port = None, None
    ip_, port_ = get_extensionmarket_ip(ip, port)
    ulog.debug(ip_)
    ulog.debug(port_)
    expected = 7727
    assert port_ == expected


###############################################################################
## 以下测试会频繁访问通达信的服务器，在实际测试时请分步测试，1) 测试其中一个 ##
## 方法时，先注释其他方法 或者2) 加上 time.sleep(interval)                  ##
###############################################################################

# TODO:  在网络测试时，正确的做法是应该mock数据。目的是测试方法，而不是测试网络。


def test_get_stock_list():
    # time.sleep(5)
    actual = get_stock_list()
    ulog.debug(actual)
    expected = 3803
    assert actual.size >= expected


# def test_get_security_bars():
#     code = "000001"
#     actual = get_security_bars(code)
#     ulog.debug(actual)
#     expected = 3
#     assert actual.size >= expected


def test_get_stock_day():
    # time.sleep(5)
    code = "000001"
    start = "2020-01-01"
    end = "2020-02-01"
    actual = get_stock_day(code, start, end)
    ulog.debug(actual)
    ulog.debug(actual.size)
    expected = 144
    assert actual.size == expected
