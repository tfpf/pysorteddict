import sys
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

        # Store the reference count of an item in a list at the position at
        # which it appears in the normal dictionary. At this point, the
        # reference counts are all 3, but querying the reference count
        # increases it, so I store 4. Whenever a test changes the reference
        # count of any item, I set the new reference count at its index.
        self.keys_refcounts = [4] * len(self.normal_dict)
        self.values_refcounts = [4] * len(self.normal_dict)

    def test_len(self):
        self.assertEqual(len(self.normal_dict), len(self.sorted_dict))

    def test_getitem_wrong_type(self):
        with self.assertRaises(TypeError) as ctx:
            self.sorted_dict[object()]
        self.assertEqual(self.wrong_argument, ctx.exception.args[0])

    def test_getitem_not_found(self):
        key = -self.rg.int()
        with self.assertRaises(KeyError) as ctx:
            self.sorted_dict[key]
        self.assertEqual(key, ctx.exception.args[0])

        self.assertEqual(3, sys.getrefcount(key))

    def test_getitem(self):
        key = self.rg.choice(self.keys)
        value = self.sorted_dict[key]
        self.assertEqual(self.normal_dict[key], value)

        self.assertEqual(5, sys.getrefcount(key))
        self.assertEqual(5, sys.getrefcount(value))

    def test_setitem_wrong_type(self):
        value = self.rg.int()
        with self.assertRaises(TypeError) as ctx:
            self.sorted_dict[object()] = value
        self.assertEqual(self.wrong_argument, ctx.exception.args[0])

        self.assertEqual(2, sys.getrefcount(value))

    def test_setitem_existing(self):
        idx, key = self.rg.choice([*enumerate(self.normal_dict)])
        value = self.rg.int()
        self.sorted_dict[key] = value
        self.assertEqual(value, self.sorted_dict[key])

        self.assertEqual(5, sys.getrefcount(key))
        self.assertEqual(3, sys.getrefcount(value))
        self.values_refcounts[idx] -= 1

    def test_setitem_new(self):
        key = -self.rg.int()
        value = self.rg.int()
        self.sorted_dict[key] = value
        self.assertEqual(value, self.sorted_dict[key])

        self.assertEqual(3, sys.getrefcount(key))
        self.assertEqual(3, sys.getrefcount(value))

    def test_setitem_remove_not_found(self):
        key = -self.rg.int()
        with self.assertRaises(KeyError) as ctx:
            del self.sorted_dict[key]
        self.assertEqual(key, ctx.exception.args[0])

        self.assertEqual(3, sys.getrefcount(key))

    def test_setitem_remove_existing(self):
        idx, key = self.rg.choice([*enumerate(self.normal_dict)])
        del self.sorted_dict[key]
        with self.assertRaises(KeyError) as ctx:
            self.sorted_dict[key]
        self.assertEqual(key, ctx.exception.args[0])

        self.assertEqual(5, sys.getrefcount(key))
        self.keys_refcounts[idx] -= 1
        self.values_refcounts[idx] -= 1

    def test_str(self):
        s = str(self.sorted_dict)
        self.assertEqual(str(dict(sorted(self.normal_dict.items()))), s)

        self.assertEqual(2, sys.getrefcount(s))

    def test_items(self):
        self.assertEqual(sorted(self.normal_dict.items()), self.sorted_dict.items())

    def test_items_preserved(self):
        self._items = self.sorted_dict.items()
        self.assertEqual(sorted(self.normal_dict.items()), self._items)

        self.assertEqual(2, sys.getrefcount(self._items))
        self.keys_refcounts = [5] * len(self.normal_dict)
        self.values_refcounts = [5] * len(self.normal_dict)

    def test_keys(self):
        self.assertEqual(sorted(self.normal_dict.keys()), self.sorted_dict.keys())

    def test_keys_preserved(self):
        self._keys = self.sorted_dict.keys()
        self.assertEqual(sorted(self.normal_dict.keys()), self._keys)

        self.assertEqual(2, sys.getrefcount(self._keys))
        self.keys_refcounts = [5] * len(self.normal_dict)

    def test_values(self):
        self.assertEqual([item[1] for item in sorted(self.normal_dict.items())], self.sorted_dict.values())

    def test_values_preserved(self):
        self._values = self.sorted_dict.values()
        self.assertEqual([item[1] for item in sorted(self.normal_dict.items())], self._values)

        self.assertEqual(2, sys.getrefcount(self._values))
        self.values_refcounts = [5] * len(self.normal_dict)

    def test_del(self):
        del self.sorted_dict

        self.keys_refcounts = [3] * len(self.normal_dict)
        self.values_refcounts = [3] * len(self.normal_dict)

    def test_empty(self):
        sorted_dict = SortedDict(int)
        self.assertEqual("{}", str(sorted_dict))
        self.assertEqual(0, len(sorted_dict))

    def tearDown(self):
        for expected, observed in zip(self.keys_refcounts, map(sys.getrefcount, self.normal_dict), strict=False):
            self.assertEqual(expected, observed)
        for expected, observed in zip(
            self.values_refcounts, map(sys.getrefcount, self.normal_dict.values()), strict=False
        ):
            self.assertEqual(expected, observed)


if __name__ == "__main__":
    unittest.main()
