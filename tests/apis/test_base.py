from abquant.apis.base import get_price, get_all_securities
import pandas as pd
from abquant.utils.logger import user_log as ulog


def test_get_price_day_with_single_field():
    code = "000001.XSHE"
    start = "2020-01-01"
    end = "2020-02-01"
    fields = ["open"]
    actual = get_price(code, start, end, fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, (pd.Series))


def test_get_price_day_with_multi_fields():
    code = "000001.XSHE"
    start = "2020-01-01"
    end = "2020-02-01"
    fields = ["open", "close"]
    actual = get_price(code, start, end, fields=fields)
    ulog.debug(actual)
    assert "close" in actual.columns
    assert isinstance(actual, (pd.DataFrame))


def test_get_price_min_with_single_field():
    code = "000001.XSHE"
    start = "2020-01-01 00:00:00"
    end = "2020-02-01 00:00:00"
    fields = ["open"]
    actual = get_price(code, start, end, frequency="1min", fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, (pd.Series))


def test_get_price_min_with_multi_field():
    code = "000001.XSHE"
    start = "2020-01-01 00:00:00"
    end = "2020-02-01 00:00:00"
    fields = ["open", "close"]
    actual = get_price(code, start, end, frequency="5min", fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, (pd.DataFrame))


def test_get_price_min_with_multi_field_without_data():
    code = "000001.XSHE"
    start = "2020-01-01 00:00:00"
    end = "2020-01-01 23:55:00"
    fields = ["open", "close"]
    actual = get_price(
        code, start, end, frequency="5min", fields=fields, adjust_type="pre"
    )
    ulog.debug(actual)
    assert isinstance(actual, (pd.DataFrame))
    assert actual.empty is True


def test_get_all_securities_cs_withno_date():
    actual = get_all_securities(["cs"])
    ulog.debug(actual)
    assert isinstance(actual, (pd.DataFrame))
