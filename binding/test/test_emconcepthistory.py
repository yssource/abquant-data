# -*- coding: utf-8 -*-

import unittest

import pandas as pd

from pyabqemconcepthistory import PyEmConceptHistory as emconcepthistory
from pyabquant import PyAbquant

pd.set_option("display.max_rows", None)
pd.set_option("display.max_columns", None)
pd.set_option("display.width", None)


class MainTest(unittest.TestCase):
    def testDay(self):
        codes = ["90.bk0980", "90.bk0990"]
        start = "2019-01-01"
        end = "2019-12-01"
        sd = emconcepthistory(codes, start, end)

        date = sd.toSeries_string("date")
        open_ = sd.toSeries("open")
        close = sd.toSeries("close")
        high = sd.toSeries("high")
        low = sd.toSeries("low")
        volume = sd.toSeries("volume")
        amount = sd.toSeries("amount")
        amplitude = sd.toSeries("amplitude")
        rise_fall_pct = sd.toSeries("rise_fall_pct")
        rise_fall_amt = sd.toSeries("rise_fall_amt")
        turnover = sd.toSeries("turnover")
        mkt_code = sd.toSeries_string("mkt_code")
        print(date[:10])
        print(mkt_code[:10])
        date_stamp = sd.toSeries("date_stamp")

        df = pd.DataFrame(
            {
                "date": date,
                "open": open_,
                "close": close,
                "high": high,
                "low": low,
                "volume": volume,
                "amount": amount,
                "amplitude": amplitude,
                "rise_fall_pct": rise_fall_pct,
                "rise_fall_amt": rise_fall_amt,
                "turnover": turnover,
                "mkt_code": mkt_code,
                "date_stamp": date_stamp,
            }
        )
        df.set_index(["mkt_code", "date"], inplace=True)

        print(df[:10])
        print(df[-10:])

        self.assertTrue(len(open_) > 0)
        self.assertAlmostEqual(open_[0], 559.62, delta=0.001)


if __name__ == "__main__":
    PyAbquant.start()
    unittest.main()
    PyAbquant.finish()
