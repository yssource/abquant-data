# -*- coding: utf-8 -*-
import time

def time_counter(func):
    from abquant.utils.logger import system_log as slog

    def wrapper(*args, **kwargs):
        start = time.time()
        func(*args, **kwargs)
        end = time.time()
        duration = end - start
        slog.debug((u"{}({}, {}) has a duration: {}.").format(
            func, args, kwargs, duration))
    return wrapper
