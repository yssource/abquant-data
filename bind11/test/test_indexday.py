# -*- coding: utf-8 -*-

import unittest

import pandas as pd

from abqindexday import PyIndexDay as indexday
from pyabquant import PyAbquant

pd.set_option("display.max_rows", None)
pd.set_option("display.max_columns", None)
pd.set_option("display.width", None)


class MainTest(unittest.TestCase):
    def testDay(self):
        codes = ["000001"]
        start = "2019-01-01"
        end = "2019-12-01"
        sd = indexday(codes, start, end)

        open_ = sd.toSeries("open")
        close = sd.toSeries("close")
        high = sd.toSeries("high")
        low = sd.toSeries("low")
        vol = sd.toSeries("vol")
        amount = sd.toSeries("amount")
        date = sd.toSeries_string("date")
        code = sd.toSeries_string("code")
        print(date[:10])
        print(code[:10])
        date_stamp = sd.toSeries("date_stamp")

        df = pd.DataFrame(
            {
                "open": open_,
                "close": close,
                "high": high,
                "low": low,
                "vol": vol,
                "amount": amount,
                "date": date,
                "code": code,
                "date_stamp": date_stamp,
            }
        )
        df.set_index(["code", "date"], inplace=True)

        print(df[:10])

        self.assertTrue(len(open_) > 0)
        self.assertAlmostEqual(open_[0], 2497.88, delta=0.001)


if __name__ == "__main__":
    PyAbquant.start()
    unittest.main()
    PyAbquant.finish()
