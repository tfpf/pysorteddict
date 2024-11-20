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
        self.assertEqual(len(self.normal_dict), len(self.sorted_dict))

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
        self.assertEqual(self.normal_dict[key], self.sorted_dict[key])

    def test_setitem_wrong_type(self):
        with self.assertRaises(ValueError) as ctx:
            self.sorted_dict[object()] = -1000
        self.assertEqual(self.wrong_argument, ctx.exception.args[0])

    def test_setitem_existing(self):
        key = self.rg.choice(self.keys)
        value = self.rg.int()
        self.sorted_dict[key] = value
        self.assertEqual(value, self.sorted_dict[key])

    def test_setitem_new(self):
        value = self.rg.int()
        self.sorted_dict[-1000] = value
        self.assertEqual(value, self.sorted_dict[-1000])

    def test_setitem_remove_not_found(self):
        with self.assertRaises(KeyError) as ctx:
            del self.sorted_dict[-1000]
        self.assertEqual(-1000, ctx.exception.args[0])

    def test_setitem_remove(self):
        key = self.rg.choice(self.keys)
        del self.sorted_dict[key]
        with self.assertRaises(KeyError) as ctx:
            self.sorted_dict[key]
        self.assertEqual(key, ctx.exception.args[0])

    def test_str(self):
        self.assertEqual(str(dict(sorted(self.normal_dict.items()))), str(self.sorted_dict))

    def test_items(self):
        self.assertEqual(sorted(self.normal_dict.items()), self.sorted_dict.items())

    def test_keys(self):
        self.assertEqual(sorted(self.normal_dict.keys()), self.sorted_dict.keys())

    def test_values(self):
        self.assertEqual([item[1] for item in sorted(self.normal_dict.items())], self.sorted_dict.values())

    def test_empty(self):
        sorted_dict = SortedDict(int)
        self.assertEqual("{}", str(sorted_dict))
        self.assertEqual(0, len(sorted_dict))


if __name__ == "__main__":
    unittest.main()
