# -*- coding: utf-8 -*-

import unittest
import math

from pyabqstockday import PyStockDay as stockday
from pyabquant import FQ_TYPE, PyAbquant

import pandas as pd

pd.set_option("display.max_rows", None)
pd.set_option("display.max_columns", None)
pd.set_option("display.width", None)


class MainTest(unittest.TestCase):
    def test_stockday(self):
        codes = ["000001"]
        start = "2019-01-01"
        end = "2019-12-01"
        sd = stockday(codes, start, end, FQ_TYPE.PRE)

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
        # self.assertEqual(open_[0], 9.39)
        self.assertAlmostEqual(open_[0], 9.39, delta=0.001)

    def test_ROC(self):
        codes = ["000001"]
        start = "2019-01-01"
        end = "2019-12-01"
        sd = stockday(codes, start, end, FQ_TYPE.PRE)

        rst = sd.ROC("close", 12, 6)
        # print(rst)
        self.assertTrue(len(rst["ROC"]) == 222)
        self.assertTrue(len(rst["ROCMA"]) == 222)
        self.assertAlmostEqual(rst["ROC"][0], -44.296, delta=0.01)
        self.assertTrue(math.isnan(rst["ROCMA"][0]))
        self.assertAlmostEqual(rst["ROC"][-1], -6.368, delta=0.01)
        self.assertAlmostEqual(rst["ROCMA"][-1], 15.071, delta=0.01)


if __name__ == "__main__":
    PyAbquant.start()
    unittest.main()
    PyAbquant.finish()
