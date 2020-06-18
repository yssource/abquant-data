# -*- coding: utf-8 -*-

import unittest
from pyabquant import PyAbquant
from abqstockmin import PyStockMin as stockmin


class MainTest(unittest.TestCase):
    def testMin(self):
        codes = ["000001"]
        start = "2019-01-01"
        end = "2019-12-01"
        freq = "5min"
        sm = stockmin(codes, start, end, freq)
        self.assertTrue(sm.toQfq() > 0)


if __name__ == "__main__":
    PyAbquant.start()
    unittest.main()
    PyAbquant.finish()
