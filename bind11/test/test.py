# -*- coding: utf-8 -*-

import unittest
# import libfoo as foo
import foo

class MainTest(unittest.TestCase):
    def testFoo(self):
        print(foo.foo2())
        self.assertEqual(foo.add(5, 5), 10)
        self.assertEqual(foo.foo2(), 600001)

if __name__ == '__main__':
    unittest.main()
