from abquant.data.tdx_api import *
from abquant.utils.logger import user_log as ulog


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
