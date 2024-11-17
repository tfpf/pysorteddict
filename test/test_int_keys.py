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
        self.values = [rand() for _ in self.keys]
        self.regular_dict = dict(zip(self.keys, self.values, strict=True))
        self.sorted_dict = SortedDict(int)
        for key, value in zip(self.keys, self.values, strict=False):
            self.sorted_dict[key] = value

    def test_len(self):
        expected = len(self.regular_dict)
        observed = len(self.sorted_dict)
        self.assertEqual(expected, observed)

    def test_str(self):
        expected = str(dict(sorted(self.regular_dict.items())))
        observed = str(self.sorted_dict)
        self.assertEqual(expected, observed)

    def test_items(self):
        expected = sorted(self.regular_dict.items())
        observed = self.sorted_dict.items()
        self.assertEqual(expected, observed)

    def test_keys(self):
        expected = sorted(self.regular_dict.keys())
        observed = self.sorted_dict.keys()
        self.assertEqual(expected, observed)

    def test_values(self):
        expected = [item[1] for item in sorted(self.regular_dict.items())]
        observed = self.sorted_dict.values()
        self.assertEqual(expected, observed)
