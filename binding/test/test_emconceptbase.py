# -*- coding: utf-8 -*-
import unittest
import pandas as pd
from pyabqemconceptbase import PyEmConceptBase as emconceptbase
from pyabquant import PyAbquant

pd.set_option("display.max_rows", None)
pd.set_option("display.max_columns", None)
pd.set_option("display.width", None)


class MainTest(unittest.TestCase):
    def testDay(self):
        codes = ["bk0980", "bk0990"]
        sd = emconceptbase(codes)

        f104_bk_rise_cnt = sd.toSeries_int("f104_bk_rise_cnt")
        f105_bk_fall_cnt = sd.toSeries_int("f105_bk_fall_cnt")
        f12_code = sd.toSeries_string("f12_code")
        f13_market = sd.toSeries_int("f13_market")
        f14_name = sd.toSeries_string("f14_name")
        f3_bk_rise_fall_pct = sd.toSeries("f3_bk_rise_fall_pct")
        f8_turnover = sd.toSeries("f8_turnover")

        df = pd.DataFrame(
            {
                "f104_bk_rise_cnt": f104_bk_rise_cnt,
                "f105_bk_fall_cnt": f105_bk_fall_cnt,
                "f12_code": f12_code,
                "f13_market": f13_market,
                "f14_name": f14_name,
                "f3_bk_rise_fall_pct": f3_bk_rise_fall_pct,
                "f8_turnover": f8_turnover,
            }
        )
        df.set_index(["f12_code"], inplace=True)

        print(df[:10])

        self.assertEqual(f12_code[0], "BK0980")
        self.assertEqual(f14_name[0], "债转股")
        self.assertEqual(f104_bk_rise_cnt[0], 44)
        self.assertAlmostEqual(f8_turnover[0], 1.06, delta=0.001)


if __name__ == "__main__":
    PyAbquant.start()
    unittest.main()
    PyAbquant.finish()
