from typing import Iterable


INDEX_LABELS = ["sh", "sz", "hs300", "sz50", "cyb", "zxb", "zx300", "zh500"]
INDEX_LIST = {
    "sh": "sh000001",
    "sz": "sz399001",
    "hs300": "sh000300",
    "sz50": "sh000016",
    "zxb": "sz399005",
    "cyb": "sz399006",
    "zx300": "sz399008",
    "zh500": "sh000905",
}
LIVE_DATA_URL = "%shq.%s/rn=%s&list=%s"
P_TYPE = {"http": "http://", "ftp": "ftp://"}
DOMAINS = {
    "sina": "sina.com.cn",
    "sinahq": "sinajs.cn",
    "ifeng": "ifeng.com",
    "sf": "finance.sina.com.cn",
    "vsf": "vip.stock.finance.sina.com.cn",
    "idx": "www.csindex.com.cn",
    "163": "money.163.com",
    "em": "eastmoney.com",
    "sseq": "query.sse.com.cn",
    "sse": "www.sse.com.cn",
    "szse": "www.szse.cn",
    "oss": "file.tushare.org",
    "idxip": "115.29.204.48",
    "shibor": "www.shibor.org",
    "mbox": "www.cbooo.cn",
    "tt": "gtimg.cn",
    "gw": "gw.com.cn",
    "v500": "value500.com",
    "sstar": "stock.stockstar.com",
    "dfcf": "nufm.dfcfw.com",
}
US_LIVE_DATA_COLS = [
    "name",
    "price",
    "change_percent",
    "time",
    "change",
    "open",
    "high",
    "low",
    "high_52week",
    "low_52week",
    "volume",
    "volume_average",
    "mktcap",
    "eps",
    "pe",
    "fpe",
    "beta",
    "dividend",
    "earnings_yield",
    "totals",
    "instown",
    "extended_price",
    "extended_change_percent",
    "extended_change",
    "extended_time",
    "time_est",
    "pre_close",
    "extended_volume",
]
FOR_CLASSIFY_COLS = ["code", "name"]
LIVE_DATA_COLS = [
    "name",
    "open",
    "pre_close",
    "price",
    "high",
    "low",
    "bid",
    "ask",
    "volume",
    "amount",
    "b1_v",
    "b1_p",
    "b2_v",
    "b2_p",
    "b3_v",
    "b3_p",
    "b4_v",
    "b4_p",
    "b5_v",
    "b5_p",
    "a1_v",
    "a1_p",
    "a2_v",
    "a2_p",
    "a3_v",
    "a3_p",
    "a4_v",
    "a4_p",
    "a5_v",
    "a5_p",
    "date",
    "time",
    "s",
]


def code_to_symbol(code: str) -> str:
    """
    生成symbol代码标志
    """
    if code in INDEX_LABELS:
        return INDEX_LIST[code]
    elif code[:3] == "gb_":
        return code
    else:
        if len(code) != 6:
            return code
        else:
            return (
                f"sh{code}"
                if code[:1] in ["5", "6", "9"] or code[:2] in ["11", "13"]
                else f"sz{code}"
            )


def random(n: int = 13) -> str:
    from random import randint

    start = 10 ** (n - 1)
    end = (10 ** n) - 1
    return str(randint(start, end))
