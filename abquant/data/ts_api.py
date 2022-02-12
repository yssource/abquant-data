from abquant.utils.ts import (
    code_to_symbol,
    LIVE_DATA_URL,
    P_TYPE,
    DOMAINS,
    random,
    US_LIVE_DATA_COLS,
    LIVE_DATA_COLS,
)
from abquant.utils.logger import user_log as ulog
import pandas as pd  # type: ignore
import re
import requests
from requests.exceptions import Timeout
from typing import Union, Iterable


def get_realtime_quotes(symbols: Union[str, Iterable[str]]) -> pd.DataFrame:
    """
       获取实时交易数据 getting real time quotes data
       用于跟踪交易情况（本次执行的结果-上一次执行的数据）
    Parameters
    ------
        symbols : string, array-like object (list, tuple, Series).
    return
    -------
        DataFrame 实时交易数据
              属性:0：name，股票名字
            1：open，今日开盘价
            2：pre_close，昨日收盘价
            3：price，当前价格
            4：high，今日最高价
            5：low，今日最低价
            6：bid，竞买价，即“买一”报价
            7：ask，竞卖价，即“卖一”报价
            8：volumn，成交量 maybe you need do volumn/100
            9：amount，成交金额（元 CNY）
            10：b1_v，委买一（笔数 bid volume）
            11：b1_p，委买一（价格 bid price）
            12：b2_v，“买二”
            13：b2_p，“买二”
            14：b3_v，“买三”
            15：b3_p，“买三”
            16：b4_v，“买四”
            17：b4_p，“买四”
            18：b5_v，“买五”
            19：b5_p，“买五”
            20：a1_v，委卖一（笔数 ask volume）
            21：a1_p，委卖一（价格 ask price）
            ...
            30：date，日期；
            31：time，时间；
    """
    symbols_list = ""
    if isinstance(symbols, Iterable):
        for code in symbols:
            symbols_list += code_to_symbol(code) + ","
    else:
        symbols_list = code_to_symbol(symbols)
    symbols_list = symbols_list[:-1] if len(symbols_list) > 8 else symbols_list
    url = LIVE_DATA_URL % (P_TYPE["http"], DOMAINS["sinahq"], random(), symbols_list)
    headers = {"host": "hq.sinajs.cn", "referer": "https://finance.sina.com.cn/"}

    text = ""
    try:
        resp = requests.get(url, headers=headers, timeout=10)
        text = resp.text
    except Timeout:
        # ulog.warning(f"URL: {url} \n Timeout has been raised.")
        return pd.DataFrame(columns=LIVE_DATA_COLS).drop("s", axis=1)  # type: ignore

    reg = re.compile(r'\="(.*?)\";')
    data = reg.findall(text)
    regSym = re.compile(r"(?:sh|sz|gb_)(.*?)\=")
    syms = regSym.findall(text)
    data_list = []
    syms_list = []
    for index, row in enumerate(data):
        if len(row) > 1:
            data_list.append([astr for astr in row.split(",")][: len(LIVE_DATA_COLS)])
            syms_list.append(syms[index])
    if len(syms_list) == 0:
        return pd.DataFrame(columns=LIVE_DATA_COLS).drop("s", axis=1)  # type: ignore
    if len(data_list[0]) == 28:
        df = pd.DataFrame(data_list, columns=US_LIVE_DATA_COLS)
    else:
        df = pd.DataFrame(data_list, columns=LIVE_DATA_COLS)
        df: pd.DataFrame = df.drop("s", axis=1)  # type: ignore
    df["code"] = syms_list
    ls = [cls for cls in df.columns if "_v" in cls]
    for txt in ls:
        df[txt] = df[txt].map(lambda x: x[:-2])
    return df
