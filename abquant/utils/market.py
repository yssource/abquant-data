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
