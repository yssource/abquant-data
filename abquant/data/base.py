# -*- coding: utf-8 -*-

from __future__ import annotations
import abc
from typing import List
from abquant.helper import time_counter
from abquant.config import Setting
from abquant.data.tdx_api import get_stock_day
from abquant.utils.logger import system_log as slog
import pymongo


class ISecurity(object, metaclass=abc.ABCMeta):
    """
    The ISecurity interface declares an `accept` method that should take the
    base ISecurityVisitor interface as an argument.
    """

    @abc.abstractmethod
    def accept(self, visitor: ISecurityVisitor) -> None:
        pass


class ISecurityVisitor(object, metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def create_day(self, iSecurity: ISecurity):
        pass

    @abc.abstractmethod
    def create_min(self, iSecurity: ISecurity, *args, **kwargs):
        pass


class SecurityVisitor(ISecurityVisitor):
    def __init__(self, *args, **kwargs):
        "docstring"
        pass

    def create_day(self, iSecurity: ISecurity):
        if getattr(iSecurity, "create_index_day"):
            iSecurity.create_index_day()
        if getattr(iSecurity, "create_day"):
            iSecurity.create_day()

    def create_min(self, iSecurity: ISecurity, *args, **kwargs):
        # freq = kwargs.get("freq", "x")
        if getattr(iSecurity, "create_index_min"):
            iSecurity.create_index_min(args, kwargs)
        if getattr(iSecurity, "create_min"):
            iSecurity.create_min(args, kwargs)

class Security(object):
    def __init__(self, *args, **kwargs):
        "docstring"
        self._db = pymongo.MongoClient(Setting.get_mongo())
        self.kind = kwargs.get("kind", "x")
        if self.kind in ["min"]:
            self.freq = kwargs.get("freq", "1min")

    @property
    def db(self):
        return self._db

    def create(self):
        if self.kind in ["x"]:
            slog.debug("create ... stock")
            self.create_day()
            self.create_min()
        if self.kind in ["day"]:
            self.create_day()
        if self.kind in ["min"]:
            if self.freq in ["x", "xmin"]:
                self.create_min(freq=self.freq)
            if self.freq in ["1min"]:
                pass

    def create_day(self, iSecurity: ISecurity):
        if getattr(iSecurity, "create_index_day"):
            iSecurity.create_index_day()
        if getattr(iSecurity, "create_day"):
            iSecurity.create_day()
        # code = "000001"
        # start = "2020-01-01"
        # end = "2020-02-01"
        # actual = get_stock_day(code, start, end)
        # # actual = get_index_day(code, start, end)
        # slog.debug(actual)

    def create_min(self, iSecurity: ISecurity, *args, **kwargs):
        # freq = kwargs.get("freq", "x")
        if getattr(iSecurity, "create_index_min"):
            iSecurity.create_index_min(args, kwargs)
        if getattr(iSecurity, "create_min"):
            iSecurity.create_min(args, kwargs)

    def delete(self):
        slog.debug("delete ... stock")

    def update(self):
        slog.debug("update ... stock")

    def read(self):
        slog.debug("read ... stock")


def get_broker(broker="tdx"):
    """Factory"""
    if broker in ["tdx"]:
        from . import tdx as TDX
    if broker in ["ths"]:
        from . import ths as THS
    brokers = {
        "tdx": TDX,
    }
    return brokers[broker]


@time_counter
def create_all():
    def registe_securities(
        securities: List[ISecurity], visitor: ISecurityVisitor
    ) -> None:
        for sec in securities:
            sec.accept(visitor)

    broker = get_broker()
    securities = [broker.Stock(), broker.Future()]
    registe_securities(securities, SecurityVisitor())


@time_counter
def create_stock_day():
    broker = get_broker()
    broker.Stock().create_index_day()
    broker.Stock().create_day()


@time_counter
def create_stock_min(freqs):
    broker = get_broker()
    f = freqs.split() if freqs else ""
    if f:
        s = broker.Stock(freqs=f)
        s.create_min(stockOrIndex="stock")
        s.create_min(stockOrIndex="index")
    else:
        s = broker.Stock()
        s.create_min(stockOrIndex="stock")
        s.create_min(stockOrIndex="index")
