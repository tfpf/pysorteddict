import random
import unittest

from pysorteddict import SortedDict


class Random(random.Random):
    def __init__(self, x: str = __name__):
        super().__init__(x)

    def randrange(self, start: int = 1000, stop: int = 2000):
        return super().randrange(start, stop)


r = Random()


class TestIntKeys(unittest.TestCase):
    """Test a sorted dictionary with ``int`` keys."""

    def setUp(self):
        self.keys = [r.randrange() for _ in range(1000)]
        self.values = [r.randrange() for _ in self.keys]
        self.normal_dict = dict(zip(self.keys, self.values, strict=True))
        self.sorted_dict = SortedDict(int)
        for key, value in zip(self.keys, self.values, strict=True):
            self.sorted_dict[key] = value

    @unittest.skip
    def test_len(self):
        expected = len(self.normal_dict)
        observed = len(self.sorted_dict)
        self.assertEqual(expected, observed)

    def test_getitem_wrong_type(self):
        with self.assertRaises(ValueError) as ctx:
            self.sorted_dict[object()]
        self.assertEqual("key must be of type <class 'int'>", ctx.exception.args[0])

    @unittest.skip
    def test_getitem_not_found(self):
        with self.assertRaises(KeyError) as ctx:
            self.sorted_dict[-1000]
        self.assertEqual(-1000, ctx.exception.args[0])

    def test_getitem(self):
        key = r.choice(self.keys)
        expected = self.normal_dict[key]
        observed = self.sorted_dict[key]
        self.assertEqual(expected, observed)

    @unittest.skip
    def test_str(self):
        expected = str(dict(sorted(self.normal_dict.items())))
        observed = str(self.sorted_dict)
        self.assertEqual(expected, observed)

    @unittest.skip
    def test_items(self):
        expected = sorted(self.normal_dict.items())
        observed = self.sorted_dict.items()
        self.assertEqual(expected, observed)

    @unittest.skip
    def test_keys(self):
        expected = sorted(self.normal_dict.keys())
        observed = self.sorted_dict.keys()
        self.assertEqual(expected, observed)

    @unittest.skip
    def test_values(self):
        expected = [item[1] for item in sorted(self.normal_dict.items())]
        observed = self.sorted_dict.values()
        self.assertEqual(expected, observed)
