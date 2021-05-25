import pandas as pd
import numpy as np
from abquant.utils.logger import system_log as slog
from abquant.helper import to_json_from_pandas
from abquant.utils.qa import make_datestamp
from abquant.config import Setting
import json


def for_sz(code):
    """深市代码分类
    Arguments:
        code {[type]} -- [description]
    Returns:
        [type] -- [description]
    """

    if str(code)[0:2] in ["00", "30", "02"]:
        return "stock_cn"
    elif str(code)[0:2] in ["39"]:
        return "index_cn"
    elif str(code)[0:2] in ["15"]:
        return "etf_cn"
    elif str(code)[0:3] in [
        "101",
        "104",
        "105",
        "106",
        "107",
        "108",
        "109",
        "111",
        "112",
        "114",
        "115",
        "116",
        "117",
        "118",
        "119",
        "123",
        "127",
        "128",
        "131",
        "139",
    ]:
        # 10xxxx 国债现货
        # 11xxxx 债券
        # 12xxxx 可转换债券

        # 123
        # 127
        # 12xxxx 国债回购
        return "bond_cn"

    elif str(code)[0:2] in ["20"]:
        return "stockB_cn"
    else:
        return "undefined"


def for_sh(code):
    if str(code)[0] == "6":
        return "stock_cn"
    elif str(code)[0:3] in ["000", "880"]:
        return "index_cn"
    elif str(code)[0:2] == "51":
        return "etf_cn"
    # 110×××120×××企业债券；
    # 129×××100×××可转换债券；
    # 113A股对应可转债 132
    elif str(code)[0:3] in [
        "102",
        "110",
        "113",
        "120",
        "122",
        "124",
        "130",
        "132",
        "133",
        "134",
        "135",
        "136",
        "140",
        "141",
        "143",
        "144",
        "147",
        "148",
    ]:
        return "bond_cn"
    else:
        return "undefined"


headers = {
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8",
    "Accept-Encoding": "gzip, deflate",
    "Accept-Language": "zh-CN,zh;q=0.9",
    "Cache-Control": "max-age=0",
    "Connection": "keep-alive",
    "Upgrade-Insecure-Requests": "1",
    #   'If-Modified-Since': 'Thu, 11 Jan 2018 07:05:01 GMT',
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36",
}


def select_bond_market_code(code):

    if code[0:3] in [
        "101",
        "104",
        "105",
        "106",
        "107",
        "108",
        "109",
        "111",
        "112",
        "114",
        "115",
        "116",
        "117",
        "118",
        "119",
        "123",
        "127",
        "128",
        "131",
        "139",
    ]:
        return 0
    else:
        return 1


def select_market_code(code):
    """
    1- sh
    0 -sz
    """
    code = str(code)
    if code[0] in ["5", "6", "9"] or code[:3] in [
        "009",
        "126",
        "110",
        "201",
        "202",
        "203",
        "204",
    ]:
        return 1
    return 0


def select_index_code(code):
    """
    1 - sh
    0 - sz
    """
    code = str(code)
    if code[0] == "3":
        return 0
    return 1


def get_stock_market(code):
    return "SH" if select_market_code(code) == 1 else "SZ"


def select_type(frequence):
    if frequence in ["day", "d", "D", "DAY", "Day"]:
        frequence = 9
    elif frequence in ["w", "W", "Week", "week"]:
        frequence = 5
    elif frequence in ["month", "M", "m", "Month"]:
        frequence = 6
    elif frequence in ["Q", "Quarter", "q"]:
        frequence = 10
    elif frequence in ["y", "Y", "year", "Year"]:
        frequence = 11
    elif str(frequence) in ["5", "5m", "5min", "five"]:
        frequence, type_ = 0, "5min"
    elif str(frequence) in ["1", "1m", "1min", "one"]:
        frequence, type_ = 8, "1min"
    elif str(frequence) in ["15", "15m", "15min", "fifteen"]:
        frequence, type_ = 1, "15min"
    elif str(frequence) in ["30", "30m", "30min", "half"]:
        frequence, type_ = 2, "30min"
    elif str(frequence) in ["60", "60m", "60min", "1h"]:
        frequence, type_ = 3, "60min"

    return frequence


def query_stock_day(
    code: str, start, end, collections, format="numpy", frequence="day",
):
    """'获取股票日线'

    Returns:
        [type] -- [description]

        感谢@几何大佬的提示
        https://docs.mongodb.com/manual/tutorial/project-fields-from-query-results/#return-the-specified-fields-and-the-id-field-only

    """

    start = str(start)[0:10]
    end = str(end)[0:10]
    # code= [code] if isinstance(code,str) else code

    # code checking
    code = code.split()
    cursor = collections.find(
        {
            "code": {"$in": code},
            "date_stamp": {"$lte": make_datestamp(end), "$gte": make_datestamp(start)},
        },
        {"_id": 0},
        batch_size=10000,
    )
    # res=[QA_util_dict_remove_key(data, '_id') for data in cursor]

    res = pd.DataFrame([item for item in cursor])
    try:
        res = (
            res.assign(volume=res.vol, date=pd.to_datetime(res.date))
            .drop_duplicates((["date", "code"]))
            .query("volume>1")
            .set_index("date", drop=False)
        )
        res = res.loc[
            :, ["code", "open", "high", "low", "close", "volume", "amount", "date"]
        ]
    except:
        res = None
    if format in ["P", "p", "pandas", "pd"]:
        return res
    elif format in ["json", "dict"]:
        return to_json_from_pandas(res)
    # 多种数据格式
    elif format in ["n", "N", "numpy"]:
        return np.asarray(res)
    elif format in ["list", "l", "L"]:
        return np.asarray(res).tolist()
    else:
        slog.error(
            'QA Error QA_fetch_stock_day format parameter %s is none of  "P, p, pandas, pd , json, dict , n, N, numpy, list, l, L, !" '
            % format
        )
        return None


def stock_to_fq(bfq_data, xdxr_data, fqtype):
    "使用数据库数据进行复权"
    info = xdxr_data.query("category==1")
    bfq_data = bfq_data.assign(if_trade=1)

    if len(info) > 0:
        data = pd.concat(
            [bfq_data, info.loc[bfq_data.index[0] : bfq_data.index[-1], ["category"]]],
            axis=1,
        )

        data["if_trade"].fillna(value=0, inplace=True)
        data = data.fillna(method="ffill")

        data = pd.concat(
            [
                data,
                info.loc[
                    bfq_data.index[0] : bfq_data.index[-1],
                    ["fenhong", "peigu", "peigujia", "songzhuangu"],
                ],
            ],
            axis=1,
        )
    else:
        data = pd.concat(
            [
                bfq_data,
                info.loc[
                    :, ["category", "fenhong", "peigu", "peigujia", "songzhuangu"]
                ],
            ],
            axis=1,
        )
    data = data.fillna(0)
    data["preclose"] = (
        data["close"].shift(1) * 10 - data["fenhong"] + data["peigu"] * data["peigujia"]
    ) / (10 + data["peigu"] + data["songzhuangu"])

    if fqtype in ["01", "qfq"]:
        data["adj"] = (
            (data["preclose"].shift(-1) / data["close"]).fillna(1)[::-1].cumprod()
        )
    else:
        data["adj"] = (
            (data["close"] / data["preclose"].shift(-1)).cumprod().shift(1).fillna(1)
        )

    for col in ["open", "high", "low", "close", "preclose"]:
        data[col] = data[col] * data["adj"]
    data["volume"] = (
        data["volume"] / data["adj"]
        if "volume" in data.columns
        else data["vol"] / data["adj"]
    )
    try:
        data["high_limit"] = data["high_limit"] * data["adj"]
        data["low_limit"] = data["high_limit"] * data["adj"]
    except:
        pass
    return data.query("if_trade==1 and open != 0").drop(
        ["fenhong", "peigu", "peigujia", "songzhuangu", "if_trade", "category"],
        axis=1,
        errors="ignore",
    )


def save_error_log(err: list, key: str):
    if len(err) < 1:
        slog.info("SUCCESS")
    else:
        slog.info(" ERROR CODE \n ")
        slog.info(err)
        errs = dict()
        if Setting.ERROR_CODES_JSON.exists():
            with Setting.ERROR_CODES_JSON.open(mode="r") as f:
                try:
                    errs = json.load(f)
                except Exception:
                    pass
        errs[key] = list(set(err))
        with Setting.ERROR_CODES_JSON.open(mode="w") as f:
            json.dump(errs, f)
