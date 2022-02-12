# -*- coding: utf-8 -*-

from __future__ import annotations
import abc
from typing import List, TYPE_CHECKING
from abquant.helper import time_counter, INSTRUMENT_TYPE

if TYPE_CHECKING:
    from abquant.data.tdx import Stock, Future, Etf


class ISecurity(object, metaclass=abc.ABCMeta):
    """
    The ISecurity interface declares an `accept` method that should take the
    base ISecurityVisitor interface as an argument.
    """

    @abc.abstractmethod
    def accept(self, visitor: ISecurityVisitor) -> None:
        ...

    @abc.abstractmethod
    def create_list(self, iSecurity: ISecurity) -> None:
        ...

    @abc.abstractmethod
    def create_day(self, iSecurity: ISecurity) -> None:
        ...

    @abc.abstractmethod
    def create_min(self, iSecurity: ISecurity) -> None:
        ...

    def getClassName(self) -> str:
        return self.__class__.__name__


class ISecurityVisitor(object, metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def visit_stock(self, stock: Stock) -> None:
        ...

    @abc.abstractmethod
    def visit_future(self, future: Future) -> None:
        ...

    @abc.abstractmethod
    def visit_etf(self, stock: Etf) -> None:
        ...


class SecurityVisitor(ISecurityVisitor):
    def __init__(self, *args, **kwargs):
        "docstring"
        ...

    def visit_stock(self, stock: Stock):
        if getattr(stock, "create_list"):
            stock.create_list(ins_type=INSTRUMENT_TYPE.INDX)
            stock.create_list(ins_type=INSTRUMENT_TYPE.CS)

        if getattr(stock, "create_day"):
            stock.create_day(ins_type=INSTRUMENT_TYPE.INDX)
            stock.create_day(ins_type=INSTRUMENT_TYPE.CS)

        if getattr(stock, "create_min"):
            stock.create_min(ins_type=INSTRUMENT_TYPE.INDX)
            stock.create_min(ins_type=INSTRUMENT_TYPE.CS)

        if getattr(stock, "create_xdxr"):
            stock.create_xdxr(ins_type=INSTRUMENT_TYPE.INDX)
            stock.create_xdxr(ins_type=INSTRUMENT_TYPE.CS)

    def visit_future(self, future: Future):
        if getattr(future, "create_list"):
            future.create_list(ins_type=INSTRUMENT_TYPE.FUTURE)

        if getattr(future, "create_day"):
            future.create_day(ins_type=INSTRUMENT_TYPE.FUTURE)

        if getattr(future, "create_min"):
            future.create_min(ins_type=INSTRUMENT_TYPE.FUTURE)

    def visit_etf(self, etf: Etf):
        if getattr(etf, "create_list"):
            etf.create_list(ins_type=INSTRUMENT_TYPE.ETF)

        if getattr(etf, "create_day"):
            etf.create_day(ins_type=INSTRUMENT_TYPE.ETF)

        if getattr(etf, "create_min"):
            etf.create_min(ins_type=INSTRUMENT_TYPE.ETF)


def get_broker(broker="tdx"):
    """Factory"""
    from . import tdx as TDX
    from . import ths as THS
    from . import qa as QA

    brokers = {
        "tdx": TDX,
        "ths": THS,
        "qa": QA,
    }
    return brokers[broker]


def get_broker_api(broker="tdx"):
    """Factory"""
    from . import tdx_api as TDX_api
    from . import ths_api as THS_api
    from . import qa_api as QA_api

    brokers = {
        "tdx": TDX_api,
        "ths": THS_api,
        "qa": QA_api,
    }
    return brokers[broker]


@time_counter
def create_base():
    def regist_securities(
        securities: List[ISecurity], visitor: ISecurityVisitor
    ) -> None:
        for sec in securities:
            sec.accept(visitor)

    broker = get_broker()
    securities = [broker.Stock(), broker.Future(), broker.Etf()]
    regist_securities(securities, SecurityVisitor())


@time_counter
def create_stock_list():
    broker = get_broker("tdx")
    brokers_api = [get_broker_api(b) for b in ["tdx", "ths", "qa"]]
    e = broker.Stock()
    e.create_list(brokers_api=brokers_api, ins_type=INSTRUMENT_TYPE.INDX)
    e.create_list(brokers_api=brokers_api, ins_type=INSTRUMENT_TYPE.CS)


@time_counter
def create_stock_day(codes):
    broker = get_broker()
    s = broker.Stock(codes=codes)
    s.create_day(ins_type=INSTRUMENT_TYPE.INDX)
    s.create_day(ins_type=INSTRUMENT_TYPE.CS)


@time_counter
def create_stock_min(codes, freqs):
    broker = get_broker()
    s = broker.Stock(codes=codes, freqs=freqs)
    s.create_min(ins_type=INSTRUMENT_TYPE.INDX)
    s.create_min(ins_type=INSTRUMENT_TYPE.CS)


@time_counter
def create_stock_xdxr(codes):
    broker = get_broker()
    s = broker.Stock(codes=codes)
    s.create_xdxr()


@time_counter
def create_stock_block():
    broker = get_broker("tdx")
    brokers_api = [get_broker_api(b) for b in ["tdx", "ths", "qa"]]
    s = broker.Stock()
    s.create_block(brokers_api=brokers_api)


@time_counter
def create_stock_info(codes):
    broker = get_broker()
    s = broker.Stock(codes=codes)
    s.create_info()


@time_counter
def create_stock_financial():
    broker = get_broker()
    s = broker.Stock()
    s.create_financial()


@time_counter
def create_etf_list():
    broker = get_broker("tdx")
    brokers_api = [get_broker_api(b) for b in ["tdx", "ths", "qa"]]
    e = broker.Etf()
    e.create_list(brokers_api=brokers_api)


@time_counter
def create_etf_day(codes):
    broker = get_broker()
    s = broker.Etf(codes=codes)
    s.create_day(ins_type=INSTRUMENT_TYPE.ETF)


@time_counter
def create_etf_min(codes, freqs):
    broker = get_broker()
    s = broker.Etf(codes=codes, freqs=freqs)
    s.create_min(ins_type=INSTRUMENT_TYPE.ETF)
