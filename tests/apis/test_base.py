from abquant.apis.base import get_price
import pandas as pd
from abquant.utils.logger import user_log as ulog
import time


def test_get_price_day_with_single_field():
    code = "000001"
    start = "2020-01-01"
    end = "2020-02-01"
    fields = ["open"]
    actual = get_price(code, start, end, fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, (pd.Series))


def test_get_price_day_with_multi_fields():
    code = "000001"
    start = "2020-01-01"
    end = "2020-02-01"
    fields = ["open", "close"]
    actual = get_price(code, start, end, fields=fields)
    ulog.debug(actual)
    expected = 144
    assert "close" in actual.columns
    assert isinstance(actual, (pd.DataFrame))


def test_get_price_min_with_single_field():
    # FIXME: .Fatal ASSERT failure in QCoreApplication: "there should be only one application object",
    code = "000001"
    start = "2020-01-01"
    end = "2020-02-01"
    fields = ["open"]
    actual = get_price(code, start, end, frequency="1min", fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, (pd.Series))
