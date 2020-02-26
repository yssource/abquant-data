import abc
import time


class IDataSource(object, metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def create(self):
        pass

    @abc.abstractmethod
    def read(self):
        pass

    @abc.abstractmethod
    def update(self):
        pass

    @abc.abstractmethod
    def delete(self):
        pass


def get_vendor(vendor="tdx"):
    """Factory"""
    if vendor in ["tdx"]:
        from . import tdx as TDX
    if vendor in ["ths"]:
        from . import ths as THS
    vendors = {
        "tdx": TDX,
    }
    return vendors[vendor]


def time_counter(func):
    from ..utils.logger import system_log

    def wrapper(*args, **kwargs):
        start = time.time()
        func(*args, **kwargs)
        end = time.time()
        duration = end - start
        system_log.debug((u"{}({}, {}) has a duration: {}.").format(
            func, args, kwargs, duration))
    return wrapper

@time_counter
def create_stock_day():
    vendor = get_vendor()
    vendor.Stock().create()


def create_stock_min():
    pass
