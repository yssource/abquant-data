# -*- coding: utf-8 -*-
import sys
import time

import numpy as np
import pandas as pd


pd.set_option('display.max_rows', None)
pd.set_option('display.max_columns', None)
pd.set_option('display.width', None)

from foo import *


def to_np(stocks):
    # for var in stocks:
    #     print(var.open())

    """转化为numpy结构数组"""
    t_type = np.dtype(
        {
            "names": ["date", "证券代码", "open", "high", "close", "low"],
            "formats": ["U10", "U10", "d", "d", "d", "d"],
        }
    )
    return np.array(
        [(s.date(), s.code(), s.open(), s.high(), s.close(), s.low()) for s in stocks],
        dtype=t_type,
    )


def to_df(stocks):
    """转化为pandas的DataFrame"""
    return pd.DataFrame.from_records(to_np(stocks), index="date")


def main():
    ida = IndexDayAction.getInstance()
    print(type(ida))
    ida2 = IndexDayAction.getInstance()
    print(type(ida2))
    if ida == ida2:
        print("====================")
    idxs = ida.run("000001", "1989-01-01", "2019-01-01")
    print("---m-----")
    return idxs


def xdxr():
    x = Xdxr()
    # df = x.getXdxr()
    # dfx = x.getIdxs()
    # df = x.get_columns()
    df = x.getData()
    # df = x.hello()
    # df = x.getVec()
    return df

def stockmin():
    sa = StockMinAction.getInstance()
    ss = sa.toPython()
    # print(ss)
    return ss


if __name__ == "__main__":
    # ida = IndexDayAction.getInstance()
    # # try:
    # #     ida = IndexDayAction().getInstance()
    # # except Exception as e:
    # #     print(e)

    # idxs = main()
    # print('--------')
    # for idx in idxs:
    #     print("{} {}".format(idx.id(), idx.code()))
    # print('----2----')
    # time.sleep(3)
    # sys.exit(0)

    # df = xdxr()
    # print("-----rrrrrrrrrr------")
    # dnp = to_df(df)
    # # dnp = to_np(df)
    # print("--2222222---rrrrrrrrrr------")
    # print(dnp)

    # print("-----rrrrrrrrrr------")
    # for e in df:
    #     print("code: {}, open: {}, high: {}".format(e.code(), e.open(), e.high()))
    # print("--2222222---rrrrrrrrrr------")
    # print(df)
    s3 = stockmin()
    for e in s3:
        print("datetime: {} code: {}, open: {}, high: {}".format(e.datetime(), e.code(), e.open(), e.high()))
