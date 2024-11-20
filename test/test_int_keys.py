import unittest

from pysorteddict import SortedDict

from .utils import Random


class TestIntKeys(unittest.TestCase):
    """Test a sorted dictionary with ``int`` keys."""

    @classmethod
    def setUpClass(cls):
        cls.wrong_argument = "key must be of type <class 'int'>"

    def setUp(self):
        self.rg = Random()
        self.keys = [self.rg.int() for _ in range(1000)]
        self.values = [self.rg.int() for _ in self.keys]
        self.normal_dict = dict(zip(self.keys, self.values, strict=True))
        self.sorted_dict = SortedDict(int)
        for key, value in zip(self.keys, self.values, strict=True):
            self.sorted_dict[key] = value

    def test_len(self):
        expected = len(self.normal_dict)
        observed = len(self.sorted_dict)
        self.assertEqual(expected, observed)

    def test_getitem_wrong_type(self):
        with self.assertRaises(ValueError) as ctx:
            self.sorted_dict[object()]
        self.assertEqual(self.wrong_argument, ctx.exception.args[0])

    def test_getitem_not_found(self):
        with self.assertRaises(KeyError) as ctx:
            self.sorted_dict[-1000]
        self.assertEqual(-1000, ctx.exception.args[0])

    def test_getitem(self):
        key = self.rg.choice(self.keys)
        expected = self.normal_dict[key]
        observed = self.sorted_dict[key]
        self.assertEqual(expected, observed)

    def test_str(self):
        expected = str(dict(sorted(self.normal_dict.items())))
        observed = str(self.sorted_dict)
        self.assertEqual(expected, observed)

    def test_items(self):
        expected = sorted(self.normal_dict.items())
        observed = self.sorted_dict.items()
        self.assertEqual(expected, observed)

    def test_keys(self):
        expected = sorted(self.normal_dict.keys())
        observed = self.sorted_dict.keys()
        self.assertEqual(expected, observed)

    def test_values(self):
        expected = [item[1] for item in sorted(self.normal_dict.items())]
        observed = self.sorted_dict.values()
        self.assertEqual(expected, observed)


if __name__ == "__main__":
    unittest.main()
