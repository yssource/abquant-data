# -*- coding: utf-8 -*-

import unittest
import foo

class MathTest(unittest.TestCase):
    def testMath(self):
        '''Test case for Math class from foo module.'''
        # val = 5
        # counter = foo.Counter(600001)
        # print(counter.getCounter())
        # self.assertEqual(counter.value(), 600001)
        # counter.setValue(55)
        # print(counter.getCounter())
        # self.assertEqual(counter.value(), 55)
        # print(counter.getCounter())
        # self.assertEqual(counter.squared(5), 5 * 5)

        d = foo.Doo("66666")
        ss3 = d.init3("bbb00000000")
        ss4 = d.init5("aa11100000000")
        print("ssssss 55553333333 -> {}".format(ss3))
        print("ssssss 55555554444443333333 -> {}".format(ss4))
        s = d.mmain("aaaaaaaaa", "bbbbbbbbbbbb")
        s2 = d.mmain("ccccccccc", "dddddddddd")
        print('ssssss {}'.format(s))
        print('s2222sssss {}'.format(s2))


        print('-----------------------22222---------------------------')
        d2 = foo.Doo("88888866666")
        s = d2.mmain2("aaaaa22aaaa", "bbbbbbbbbbbb")
        s2 = d2.mmain2("ccc222cccccc", "dddddddddd")
        print('-----ssssss {}'.format(s))
        print('------s2222sssss {}'.format(s2))
if __name__ == '__main__':
    unittest.main()
