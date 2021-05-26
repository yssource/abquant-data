# -*- coding: utf-8 -*-
import unittest
from pyabquant import PyAbquant
from pyabqstockday import PyStockDay as stockday

import matplotlib

matplotlib.use("Agg")

from matplotlib import rcParams, gridspec, ticker, image as mpimg, pyplot as plt
from matplotlib import font_manager
import seaborn as sns
import pandas as pd
import numpy as np
import platform


class MainTest(unittest.TestCase):
    def testPlot(self):
        codes = ["000001"]
        start = "2019-01-01"
        end = "2019-12-01"
        sd = stockday(codes, start, end)
        open_ = sd.to_series("open")
        close = sd.to_series("close")
        high = sd.to_series("high")
        low = sd.to_series("low")
        vol = sd.to_series("vol")
        amount = sd.to_series("amount")
        date = sd.to_series_string("date")
        code = sd.to_series_string("code")
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

        print(df.head(20))
        fig = plt.figure()
        ax = fig.add_subplot(111)
        code = "000001"
        ax.set_title(code, fontsize="large")
        df.plot(kind="line", x="date_stamp", y="open", color="red", ax=ax)
        img = "/tmp/pyabqstockday.png"
        fig.savefig(img)
        self.assertTrue(len(open_) > 0)


if __name__ == "__main__":
    PyAbquant.start()
    unittest.main()
    PyAbquant.finish()
