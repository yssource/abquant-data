# -*- coding: utf-8 -*-
import sys
import time

import pprint
import numpy as np
import pandas as pd


pd.set_option('display.max_rows', None)
pd.set_option('display.max_columns', None)
pd.set_option('display.width', None)

from pytdx.hq import TdxHq_API
api = TdxHq_API(heartbeat=True)


if __name__ == "__main__":
    with api.connect('119.147.212.81', 7709):
        # import pudb; pudb.set_trace()
        # dt=api.get_minute_time_data(0, '002154')
        # dt=api.get_minute_time_data(0, '002773')
        # dt=api.get_minute_time_data(1, '600300')
        # dt = api.to_df(dt) # 返回DataFrame
        # print(dt)
        # pprint.pprint(dt)


        # log.info("获取股票列表")
        # import pudb; pudb.set_trace()
        stocks = api.get_security_list(0, 1000)
        pprint.pprint(stocks)

        # log.info("获取k线")
        # data = api.get_security_bars(9, 0, '000001', 4, 3)
        # pprint.pprint(data)
