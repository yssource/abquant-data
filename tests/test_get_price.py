# -*- coding: utf-8 -*-
from foo import *
import pytest
import time


def get_price(code, start, end):
    # import trepan.api; trepan.api.debug(start_opts={'startup-profile': ['/home/jimmy/.config/trepanpy/profile']})
    try:
        ida = IndexDayAction.getInstance()
        idxs = ida.run("000001", "1989-01-01", "2019-01-01")
        if len(idxs) > 0:
            return idxs[0]
        return ""
    except Exception:
        return ""


def test_singleton():
    ida = IndexDayAction.getInstance()
    ida2 = IndexDayAction.getInstance()
    assert ida == ida2


def test_get_price():
    code = "000001"
    start = "1989-01-01"
    end = "2022-01-01"
    expected = get_price(code, start, end)
    assert "000001" in expected.code()
    # time.sleep(10)
