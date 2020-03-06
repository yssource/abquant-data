# -*- coding: utf-8 -*-
import unittest
from abqstockday import PyStockDay as stockday

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
        open_ = sd.toSeries("open")
        close = sd.toSeries("close")
        high = sd.toSeries("high")
        low = sd.toSeries("low")
        vol = sd.toSeries("vol")
        date_stamp = sd.toSeries("date_stamp")

        df = pd.DataFrame(
            {
                "open": open_,
                "close": close,
                "high": high,
                "low": low,
                "vol": vol,
                "date_stamp": date_stamp,
            }
        )
        print(df.head(20))

        fig = plt.figure()
        ax = fig.add_subplot(111)
        code = "000001"
        ax.set_title(code, fontsize='large')
        df.plot(kind="line", x="date_stamp", y="open", color="red", ax=ax)
        img = "/tmp/abqstockday.png"
        fig.savefig(img)
        self.assertTrue(len(open_) > 0)


if __name__ == "__main__":
    unittest.main()
