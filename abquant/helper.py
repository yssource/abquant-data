# -*- coding: utf-8 -*-
import time
import json
import hashlib
from typing import Union, Optional, List, Tuple, Iterable, Callable
from functools import lru_cache
from enum import Enum, EnumMeta

def time_counter(func):
    from abquant.utils.logger import system_log as slog

    def wrapper(*args, **kwargs):
        start = time.time()
        func(*args, **kwargs)
        end = time.time()
        duration = end - start
        slog.debug(
            ("{}({}, {}) has a duration: {}.").format(func, args, kwargs, duration)
        )

    return wrapper


def to_json_from_pandas(data):
    """需要对于datetime 和date 进行转换, 以免直接被变成了时间戳"""
    if "datetime" in data.columns:
        data.datetime = data.datetime.apply(str)
    if "date" in data.columns:
        data.date = data.date.apply(str)
    return json.loads(data.to_json(orient="records"))


def util_file_md5(filename):
    """
    explanation:
        获取文件的MD5值

    params:
        * filename ->:
            meaning: 文件路径
            type: null
            optional: [null]

    return:
        str

    demonstrate:
        Not described

    output:
        Not described
    """

    with open(filename, mode="rb") as f:
        d = hashlib.md5()
        while True:
            # 128 is smaller than the typical filesystem block
            buf = f.read(4096)
            if not buf:
                break
            d.update(buf)
        return d.hexdigest()


@lru_cache(maxsize=256)
def normalize_code(code: str, kind: str = "cs") -> str:
    if ".XSHG" in code or ".XSHE" in code:
        return code
    if code.startswith("399"):
        return f"{code}.XSHE"
    if kind in ["cs", "CS"]:
        if code.startswith("6"):
            return f"{code}.XSHG"
        elif code[0] in ["3", "0"]:
            return f"{code}.XSHE"
        else:
            raise RuntimeError("Unknown code")
    if kind in ["indx", "INDX", "index", "INDEX"]:
        if code.startswith("0"):
            return f"{code}.XSHG"
        if code.startswith("3"):
            return f"{code}.XSHE"
    return code


@lru_cache(maxsize=256)
def normalize_code_list(codes: Tuple[str], kind: str = "cs") -> Iterable[str]:
    return [normalize_code(code, kind) for code in codes]


class CustomEnumMeta(EnumMeta):
    def __new__(metacls, cls, bases, classdict):
        enum_class = super(CustomEnumMeta, metacls).__new__(metacls, cls, bases, classdict)
        enum_class._member_reverse_map = {v.value: v for v in enum_class.__members__.values()}
        return enum_class

    def __contains__(cls, member):
        if super(CustomEnumMeta, cls).__contains__(member):
            return True
        if isinstance(member, str):
            return member in cls._member_reverse_map
        return False

    def __getitem__(self, item):
        try:
            return super(CustomEnumMeta, self).__getitem__(item)
        except KeyError:
            return self._member_reverse_map[item]

# noinspection PyUnresolvedReferences
class CustomEnumCore(str, Enum, metaclass=CustomEnumMeta): pass

# noinspection PyUnresolvedReferences
class CustomEnum(CustomEnumCore):
    def __repr__(self):
        return "%s.%s" % (
            self.__class__.__name__, self._name_)

# noinspection PyPep8Naming
class INSTRUMENT_TYPE(CustomEnum):
    CS = "CS"
    FUTURE = "Future"
    OPTION = "Option"
    ETF = "ETF"
    LOF = "LOF"
    INDX = "INDX"
    FENJI_MU = "FenjiMu"
    FENJI_A = "FenjiA"
    FENJI_B = "FenjiB"
    PUBLIC_FUND = 'PublicFund'
    BOND = "Bond"
    CONVERTIBLE = "Convertible"
    SPOT = "Spot"
    REPO = "Repo"
