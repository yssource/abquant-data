# -*- coding: utf-8 -*-

import unittest

import pandas as pd

from pyabqindexmin import PyIndexMin as indexmin
from pyabquant import PyAbquant

pd.set_option("display.max_rows", None)
pd.set_option("display.max_columns", None)
pd.set_option("display.width", None)


class MainTest(unittest.TestCase):
    def testMin(self):
        codes = ["000001"]
        start = "2019-01-01"
        end = "2019-12-01"
        freq = "5min"
        sm = indexmin(codes, start, end, freq)

        open_ = sm.to_series("open")
        close = sm.to_series("close")
        high = sm.to_series("high")
        low = sm.to_series("low")
        vol = sm.to_series("vol")
        amount = sm.to_series("amount")
        date = sm.to_series_string("date")
        datetime = sm.to_series_string("datetime")
        code = sm.to_series_string("code")
        date_stamp = sm.to_series("date_stamp")
        df = pd.DataFrame(
            {
                "open": open_,
                "close": close,
                "high": high,
                "low": low,
                "vol": vol,
                "amount": amount,
                "date": date,
                "datetime": datetime,
                "code": code,
                "date_stamp": date_stamp,
            }
        )
        df.set_index(["code", "datetime"], inplace=True)
        print(df[:10])
        self.assertTrue(len(open_) > 0)
        # self.assertEqual(open_[0], 2497.88)
        self.assertAlmostEqual(open_[0], 2497.88, delta=0.001)


if __name__ == "__main__":
    PyAbquant.start()
    unittest.main()
    PyAbquant.finish()
