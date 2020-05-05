# -*- coding: utf-8 -*-
import time
import json
import hashlib
from typing import Union, Optional, List, Tuple, Iterable, Callable
from functools import lru_cache


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


@lru_cache(maxsize=256)
def normalize_code_list(codes: Tuple[str], kind: str = "cs") -> Tuple[str]:
    return [normalize_code(code, kind) for code in codes]
