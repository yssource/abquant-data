# -*- coding: utf-8 -*-

from typing import Iterable, List, Optional, Union
import pandas as pd
import datetime
from functools import lru_cache
from abquant.helper import unnormalize_code
from abquant.utils.code import code_tolist
from abquant.utils.tdx import is_index_cn
from abquant.utils.logger import user_log as ulog
from abquant.helper import unnormalize_code
from pyabquant import PyAbquant, FQ_TYPE, INSTRUMENT_TYPE  # type: ignore


def get_price(
    order_book_ids: Union[str, List[str]],
    start_date: Union[datetime.date, datetime.datetime, str],
    end_date: Optional[Union[datetime.date, datetime.datetime, str]] = None,
    frequency: Optional[str] = "1d",
    fields: Optional[List[str]] = [],
    adjust_type: Optional[str] = "pre",
    skip_suspended: Optional[bool] = False,
    expect_df: Optional[bool] = False,
) -> pd.DataFrame:
    """
    :param order_book_ids: 合约代码，合约代码，可传入order_book_id, order_book_id list, symbol, symbol list
    :param start_date: 开始日期，用户必须指定
    :param end_date: 结束日期，默认为策略当前日期前一天
    :param frequency: 历史数据的频率。 现在支持日/分钟级别的历史数据，默认为'1d'。使用者可自由选取不同频率，例如'5m'代表5分钟线
    :param fields: 期望返回的字段名称，如 open，close 等
    :param adjust_type: 权息修复方案。前复权 - pre，后复权 - post，不复权 - none
    :param skip_suspended: 是否跳过停牌数据。默认为False，不跳过，用停牌前数据进行补齐。True则为跳过停牌期。注意，当设置为True时，函数order_book_id只支持单个合约传入
    :param expect_df: 是否期望始终返回 DataFrame。pandas 0.25.0 以上该参数应设为 True，以避免因试图构建 Panel 产生异常

    当 expect_df 为 False 时，返回值的类型如下

        *   传入一个order_book_id，多个fields，函数会返回一个pandas DataFrame
        *   传入一个order_book_id，一个field，函数会返回pandas Series
        *   传入多个order_book_id，一个field，函数会返回一个pandas DataFrame
        *   传入多个order_book_id，函数会返回一个pandas Panel


        =========================   =========================   ==============================================================================
        参数                         类型                        说明
        =========================   =========================   ==============================================================================
        open                        float                       开盘价
        close                       float                       收盘价
        high                        float                       最高价
        low                         float                       最低价
        limit_up                    float                       涨停价
        limit_down                  float                       跌停价
        total_turnover              float                       总成交额
        volume                      float                       总成交量
        acc_net_value               float                       累计净值（仅限基金日线数据）
        unit_net_value              float                       单位净值（仅限基金日线数据）
        discount_rate               float                       折价率（仅限基金日线数据）
        settlement                  float                       结算价 （仅限期货日线数据）
        prev_settlement             float                       昨日结算价（仅限期货日线数据）
        open_interest               float                       累计持仓量（期货专用）
        basis_spread                float                       基差点数（股指期货专用，股指期货收盘价-标的指数收盘价）
        trading_date                pandas.TimeStamp             交易日期（仅限期货分钟线数据），对应期货夜盘的情况
        =========================   =========================   ==============================================================================

    :example:

    获取单一股票历史日线行情:

    ..  code-block:: python3
        :linenos:

        get_price('000001.XSHE', start_date='2015-04-01', end_date='2015-04-12')
        #[Out]
        #open    close    high    low    total_turnover    volume    limit_up    limit_down
        #2015-04-01    10.7300    10.8249    10.9470    10.5469    2.608977e+09    236637563.0    11.7542    9.6177
        #2015-04-02    10.9131    10.7164    10.9470    10.5943    2.222671e+09    202440588.0    11.9102    9.7397
        #2015-04-03    10.6486    10.7503    10.8114    10.5876    2.262844e+09    206631550.0    11.7881    9.6448
        #2015-04-07    10.9538    11.4015    11.5032    10.9538    4.898119e+09    426308008.0    11.8288    9.6787
        #2015-04-08    11.4829    12.1543    12.2628    11.2929    5.784459e+09    485517069.0    12.5409    10.2620
        #2015-04-09    12.1747    12.2086    12.9208    12.0255    5.794632e+09    456921108.0    13.3684    10.9403
        #2015-04-10    12.2086    13.4294    13.4294    12.1069    6.339649e+09    480990210.0    13.4294    10.9877
        #...
    """

    if isinstance(start_date, (datetime.datetime,)):
        start_date = start_date.strftime("%Y-%m-%d %H:%M:%S")
    if isinstance(end_date, (datetime.datetime,)):
        end_date = end_date.strftime("%Y-%m-%d %H:%M:%S")

    if frequency in ["1d"]:
        start_date = start_date.split(" ")[0]  # type: ignore
        end_date = end_date.split(" ")[0]  # type: ignore

    if fields is None or not fields:
        fields = ["open", "close", "high", "low", "vol"]
    if isinstance(fields, (str,)):
        fields = [fields]
    for field in fields:
        if field in ["volume"]:
            field = "vol"
        assert field in [
            "open",
            "close",
            "high",
            "low",
            "vol",
            "amount",
            "date_stamp",
            "date",
            "datetime",
            "code",
            "...",
        ], f"invalid {field}"

    assert adjust_type in [
        "pre",
        None,
        "post",
    ], f"{adjust_type} must be one of 前复权 - pre，后复权 - post，不复权 - none"

    if adjust_type == "pre":
        fq = FQ_TYPE.PRE
    elif adjust_type == "post":
        fq = FQ_TYPE.POST
    else:
        fq = FQ_TYPE.NONE

    if isinstance(order_book_ids, (str,)):
        order_book_ids = order_book_ids.split()

    if not is_index_cn(order_book_ids[0]):
        order_book_ids = code_tolist(order_book_ids)
        if frequency in ["1d"]:
            from pyabqstockday import PyStockDay as stockday  # type: ignore

            sm = stockday(order_book_ids, start_date, end_date, fq)
        else:

            from pyabqstockmin import PyStockMin as stockmin  # type: ignore

            sm = stockmin(order_book_ids, start_date, end_date, frequency, fq)
    else:
        order_book_ids = code_tolist(order_book_ids)
        if frequency in ["1d"]:
            from pyabqindexday import PyIndexDay as indexday  # type: ignore

            sm = indexday(order_book_ids, start_date, end_date)
        else:

            from pyabqindexmin import PyIndexMin as indexmin  # type: ignore

            sm = indexmin(order_book_ids, start_date, end_date, frequency)

    code = sm.to_series_string("code")
    date = sm.to_series_string("date")
    df = pd.DataFrame()
    try:
        if frequency in ["1d"]:
            df = pd.DataFrame({"code": code, "date": date})
        else:
            datetime_ = sm.to_series_string("datetime")
            df = pd.DataFrame({"code": code, "datetime": datetime_})
    except Exception:
        raise RuntimeError("fail to get_price")

    for field in fields:
        if field in ["code", "date", "datetime"]:
            continue
        if field in ["vol"]:
            df["volume"] = sm.to_series(field)
        df[field] = sm.to_series(field)

    if frequency in ["1d"]:
        df.set_index(["code", "date"], inplace=True)
    else:
        df.set_index(["code", "datetime"], inplace=True)

    return df


@lru_cache(maxsize=1024)
def get_all_securities(
    type_: INSTRUMENT_TYPE = INSTRUMENT_TYPE.CS, date: Optional[str] = None
) -> pd.DataFrame:
    df = pd.DataFrame()
    from pyabqsecuritylist import PySecurityList as securitylist  # type: ignore

    sl = securitylist(type_)

    code = sl.to_series_string("code")
    volunit = sl.to_series("volunit")
    decimal_point = sl.to_series("decimal_point")
    name = sl.to_series_string("name")
    pre_close = sl.to_series("pre_close")
    sse = sl.to_series_string("sse")
    sec = sl.to_series_string("sec")
    # print(code[:10])

    df = pd.DataFrame(
        {
            "code": code,
            "volunit": volunit,
            "decimal_point": decimal_point,
            "name": name,
            "pre_close": pre_close,
            "sse": sse,
            "sec": sec,
        }
    )
    df.set_index(["code"], inplace=True)

    return df


@lru_cache(maxsize=1024)
def get_security_info(
    code: str, ins_type: INSTRUMENT_TYPE = INSTRUMENT_TYPE.CS
) -> pd.Series:
    from pyabqsecuritylist import PySecurityList as securitylist  # type: ignore

    sl = securitylist([unnormalize_code(code)], "", ins_type)

    code = sl.to_series_string("code")
    volunit = sl.to_series("volunit")
    decimal_point = sl.to_series("decimal_point")
    name = sl.to_series_string("name")
    pre_close = sl.to_series("pre_close")
    sse = sl.to_series_string("sse")
    sec = sl.to_series_string("sec")
    try:
        s = pd.Series(
            {
                "code": code[0],
                "volunit": volunit[0],
                "decimal_point": decimal_point[0],
                "display_name": name[0],
                "pre_close": pre_close[0],
                "sse": sse[0],
                "sec": sec[0],
            }
        )
    except:
        s = pd.Series()
    return s


def get_realtime_quotes(codes: Iterable[str]) -> pd.DataFrame:
    import abquant.data.ts_api as ts
    from abquant.utils.ts import order_book_id_2_tushare_code

    codes = [order_book_id_2_tushare_code(code) for code in codes]
    return ts.get_realtime_quotes(codes)
