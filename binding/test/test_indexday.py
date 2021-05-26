# -*- coding: utf-8 -*-

import unittest

import pandas as pd

from pyabqindexday import PyIndexDay as indexday
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

        open_ = sd.to_series("open")
        close = sd.to_series("close")
        high = sd.to_series("high")
        low = sd.to_series("low")
        vol = sd.to_series("vol")
        amount = sd.to_series("amount")
        date = sd.to_series_string("date")
        code = sd.to_series_string("code")
        print(date[:10])
        print(code[:10])
        date_stamp = sd.to_series("date_stamp")

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
