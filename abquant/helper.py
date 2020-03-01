# -*- coding: utf-8 -*-
import time
import csv
import json
import numpy as np
import pandas as pd
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
