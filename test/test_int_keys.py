import random
import unittest

from pysorteddict import SortedDict

engine = random.Random(__name__)


def rand(start: int = 1000, stop: int = 2000):
    return engine.randrange(start, stop)


class TestIntKeys(unittest.TestCase):
    """Test a sorted dictionary with ``int`` keys."""

    def setUp(self):
        self.keys = [rand() for _ in range(1000)]
        self.values = [rand() for _ in range(1000)]
        self.regular_dict = dict(zip(self.keys, self.values, strict=True))
        self.sorted_dict = SortedDict(int)
        for key, value in zip(self.keys, self.values):
            self.sorted_dict[key] = value

    def test_items(self):
        expected = sorted(self.regular_dict.items())
        observed = self.sorted_dict.items()
        self.assertEqual(expected, observed)

    def test_keys(self):
        expected = sorted(self.regular_dict.keys())
        observed = self.sorted_dict.keys()
        self.assertEqual(expected, observed)

    def test_values(self):
        expected = [*map(lambda item: item[1], sorted(self.regular_dict.items()))]
        observed = self.sorted_dict.values()
        self.assertEqual(expected, observed)

if __name__ == "__main__":
    unittest.main()
