# -*- coding: utf-8 -*-

import unittest
from abqstockday import PyStockDay as stockday


class MainTest(unittest.TestCase):
    def testDay(self):
        codes = ["000001"]
        start = "2019-01-01"
        end = "2019-12-01"
        sm = stockday(codes, start, end)
        series = sm.toSeries("open")
        self.assertTrue(len(series) > 0)
        self.assertEqual(series[0], 9.39)

if __name__ == "__main__":
    unittest.main()
