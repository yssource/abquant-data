import abc
from abquant.helper import time_counter


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


@time_counter
def create_stock_day():
    vendor = get_vendor()
    vendor.Stock().create()

@time_counter
def create_stock_min():
    pass
