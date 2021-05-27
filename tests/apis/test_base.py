from pyabquant import PyAbquant, FQ_TYPE, INSTRUMENT_TYPE  # type: ignore
from abquant.apis.base import get_price, get_all_securities, get_security_info
import pandas as pd
from abquant.utils.logger import user_log as ulog


def test_get_price_day_stock_date_with_single_field():
    code = "000001.XSHE"
    start = "2021-02-01"
    end = "2021-02-01"
    fields = ["open"]
    actual = get_price(code, start, end, fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, pd.Series)


def test_get_price_day_stock_datetime_with_single_field():
    code = "000001.XSHE"
    start = "2021-01-22 14:55:00"
    end = "2021-01-22 15:00:00"
    fields = ["open"]
    actual = get_price(code, start, end, fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, pd.Series)


def test_get_price_day_index_date_with_single_field():
    code = "000001.XSHG"
    start = "2021-02-01"
    end = "2021-02-01"
    fields = ["open"]
    actual = get_price(code, start, end, fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, pd.Series)


def test_get_price_day_index_datetime_with_single_field():
    code = "000001.XSHG"
    start = "2021-01-22 14:55:00"
    end = "2021-01-22 15:00:00"
    fields = ["open"]
    actual = get_price(code, start, end, fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, pd.Series)


def test_get_price_day_stock_date_with_multi_field():
    code = "000001.XSHE"
    start = "2021-02-01"
    end = "2021-02-01"
    fields = ["open", "close"]
    actual = get_price(code, start, end, fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, pd.DataFrame)


def test_get_price_day_stock_datetime_with_multi_field():
    code = "000001.XSHE"
    start = "2021-01-22 14:55:00"
    end = "2021-01-22 15:00:00"
    fields = ["open", "close"]
    actual = get_price(code, start, end, fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, pd.DataFrame)


def test_get_price_day_index_date_with_multi_field():
    code = "000001.XSHG"
    start = "2021-02-01"
    end = "2021-02-01"
    fields = ["open", "close"]
    actual = get_price(code, start, end, fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, pd.DataFrame)


def test_get_price_day_index_datetime_with_multi_field():
    code = "000001.XSHG"
    start = "2021-01-22 14:55:00"
    end = "2021-01-22 15:00:00"
    fields = ["open", "close"]
    actual = get_price(code, start, end, fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, pd.DataFrame)


def test_get_price_min_stock_with_single_field():
    code = "000001.XSHE"
    start = "2021-02-01 00:00:00"
    end = "2021-02-04 00:00:00"
    fields = ["open"]
    actual = get_price(code, start, end, frequency="1min", fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, pd.Series)


def test_get_price_min_stock_with_multi_field():
    code = "000001.XSHE"
    start = "2021-01-01 00:00:00"
    end = "2021-02-01 00:00:00"
    fields = ["open", "close"]
    actual = get_price(code, start, end, frequency="5min", fields=fields)
    ulog.debug(actual)
    assert isinstance(actual, pd.DataFrame)


def test_get_price_min_stock_with_multi_field_without_data():
    code = "000001.XSHE"
    start = "2021-01-01 00:00:00"
    end = "2021-01-01 23:55:00"
    fields = ["open", "close"]
    actual = get_price(
        code, start, end, frequency="5min", fields=fields, adjust_type="pre"
    )
    ulog.debug(actual)
    assert isinstance(actual, pd.DataFrame)
    assert actual.empty is True


def test_get_all_securities_cs_without_date():
    actual = get_all_securities(INSTRUMENT_TYPE.CS)
    ulog.debug(actual)
    assert isinstance(actual, pd.DataFrame)


def test_get_all_securities_index_without_date():
    actual = get_all_securities(INSTRUMENT_TYPE.INDX)
    ulog.debug(actual)
    assert isinstance(actual, pd.DataFrame)


def test_get_security_info_stock_type():
    code = "000001.XSHE"
    actual = get_security_info(code, INSTRUMENT_TYPE.CS)
    ulog.debug(actual)
    assert isinstance(actual, pd.Series)
    assert actual.display_name == "平安银行"


def test_get_security_info_index_type():
    code = "399006.XSHE"
    actual = get_security_info(code, INSTRUMENT_TYPE.INDX)
    ulog.debug(actual)
    assert isinstance(actual, pd.Series)
    assert actual.display_name == "创业板指"
