import random
import unittest

from pysorteddict import SortedDict


def rand():
    return random.randint(-99999, 99999)


class TestIntKeys(unittest.TestCase):
    def setUp(self):
        self.sd = SortedDict(int)
        for _ in range(100):
            self.sd[rand()] = rand()

if __name__ == "__main__":
    unittest.main()
