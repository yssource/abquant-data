# -*- coding: utf-8 -*-

import unittest
from pyabquant import PyAbquant, INSTRUMENT_TYPE
from pyabqsecuritylist import PySecurityList as securitylist

import pandas as pd

pd.set_option("display.max_rows", None)
pd.set_option("display.max_columns", None)
pd.set_option("display.width", None)


class MainTest(unittest.TestCase):
    def test_indexlist(self):
        codes = ["000001", "399001", "399006"]
        end = "2019-12-01"
        sd = securitylist(codes, end, INSTRUMENT_TYPE.INDX)

        code = sd.to_series_string("code")
        volunit = sd.to_series("volunit")
        decimal_point = sd.to_series("decimal_point")
        name = sd.to_series_string("name")
        pre_close = sd.to_series("pre_close")
        sse = sd.to_series_string("sse")
        sec = sd.to_series_string("sec")
        print(code[:10])

        df = pd.DataFrame(
            {
                "code": code,
                "volunit": volunit,
                "decimal_point": decimal_point,
                "display_name": name,
                "pre_close": pre_close,
                "sse": sse,
                "sec": sec,
            }
        )
        df.set_index(["code"], inplace=True)

        print(df[:10])

        self.assertTrue(len(volunit) > 0)
        self.assertAlmostEqual(volunit[0], 100.0, delta=0.001)
        self.assertEqual(df.loc["399006"].display_name, "创业板指")

    def test_stocklist(self):
        codes = ["000001", "300001", "600000"]
        end = "2019-12-01"
        sd = securitylist(codes, end, INSTRUMENT_TYPE.CS)

        code = sd.to_series_string("code")
        volunit = sd.to_series("volunit")
        decimal_point = sd.to_series("decimal_point")
        name = sd.to_series_string("name")
        pre_close = sd.to_series("pre_close")
        sse = sd.to_series_string("sse")
        sec = sd.to_series_string("sec")
        print(code[:10])

        df = pd.DataFrame(
            {
                "code": code,
                "volunit": volunit,
                "decimal_point": decimal_point,
                "display_name": name,
                "pre_close": pre_close,
                "sse": sse,
                "sec": sec,
            }
        )
        df.set_index(["code"], inplace=True)

        print(df[:10])

        self.assertTrue(len(volunit) > 0)
        self.assertAlmostEqual(volunit[0], 100.0, delta=0.001)
        self.assertEqual(df.loc["600000"].display_name, "浦发银行")


if __name__ == "__main__":
    PyAbquant.start()
    unittest.main()
    PyAbquant.finish()
