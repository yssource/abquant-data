from abquant.helper import normalize_code_list


def test_normalize_code_list_stock():
    codes = tuple(list(set(["000001", "300001", "600001", "688001"])))
    actual = normalize_code_list(codes, kind="cs")
    expected = ["000001.XSHE", "300001.XSHE", "600001.XSHG", "688001.XSHG"]
    assert set(actual) == set(expected)


def test_normalize_code_list_index():
    codes = tuple(list(set(["000001", "399001"])))
    actual = normalize_code_list(codes, kind="index")
    expected = ["000001.XSHG", "399001.XSHE"]
    assert set(actual) == set(expected)
