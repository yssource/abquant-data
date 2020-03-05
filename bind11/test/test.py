# -*- coding: utf-8 -*-

import unittest
import abqstockmin

class MainTest(unittest.TestCase):
    def testFoo(self):
        print(abqstockmin.stock())
        self.assertEqual(abqstockmin.add(5, 5), 10)
        self.assertTrue(abqstockmin.stock() > 0)

if __name__ == '__main__':
    unittest.main()
