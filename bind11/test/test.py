# -*- coding: utf-8 -*-

import unittest
# import libfoo as abqstockmin
# import libabqactions as abqstockmin
import abqstockmin

class MainTest(unittest.TestCase):
    def testFoo(self):
        print(abqstockmin.stock())
        self.assertEqual(abqstockmin.add(5, 5), 10)
        self.assertEqual(abqstockmin.stock(), 60)

if __name__ == '__main__':
    unittest.main()
