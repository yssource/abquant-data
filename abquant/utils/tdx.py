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
