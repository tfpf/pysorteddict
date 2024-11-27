import platform
import random
import sys

from pysorteddict import SortedDict


class TestGenericKeys:
    """
    Subclass this class to produce similar test cases for each key type. Name
    it first in the inheritance list to ensure correct method resolution.
    """

    def small_key(self):
        """
        Override this method to generate a random key. The set of possible
        outputs of this function should be disjoint with that of ``large_key``.
        The key should be a new object rather than an interned one.
        """

    def large_key(self):
        """
        Override this method to generate a random key. The set of possible
        outputs of this function should be disjoint with that of ``small_key``.
        The key should be a new object rather than an interned one.
        """

    @classmethod
    def setUpClass(cls):
        cls.cpython = platform.python_implementation() == "CPython"
        cls.wrong_argument = ""

    def setUp(self, key_type: type):
        self.key_type = key_type
        self.rg = random.Random(__name__)
        self.keys = [self.small_key() for _ in range(1000)]
        self.values = [self.small_key() for _ in self.keys]
        self.normal_dict = dict(zip(self.keys, self.values, strict=True))
        self.sorted_dict = SortedDict(self.key_type)
        for key, value in zip(self.keys, self.values, strict=True):
            self.sorted_dict[key] = value

        # Store the reference count of an item in a list at the position at
        # which it appears in the normal dictionary. At this point, the
        # reference counts are all 3, but querying the reference count
        # increases it, so I store 4. Whenever a test changes the reference
        # count of any item, I set the new reference count at its index.
        # Remember that reference counting is specific to the CPython
        # implementation.
        if self.cpython:
            self.keys_refcounts = [4] * len(self.normal_dict)
            self.values_refcounts = [4] * len(self.normal_dict)

    def test_len(self):
        self.assertEqual(len(self.normal_dict), len(self.sorted_dict))

    def test_getitem_wrong_type(self):
        with self.assertRaises(TypeError) as ctx:
            self.sorted_dict[object()]
        self.assertEqual(self.wrong_argument, ctx.exception.args[0])

    def test_getitem_not_found(self):
        key = self.large_key()
        with self.assertRaises(KeyError) as ctx:
            self.sorted_dict[key]
        self.assertEqual(key, ctx.exception.args[0])

        if self.cpython:
            self.assertEqual(3, sys.getrefcount(key))

    def test_getitem(self):
        key = self.rg.choice(self.keys)
        value = self.sorted_dict[key]
        self.assertEqual(self.normal_dict[key], value)

        if self.cpython:
            self.assertEqual(5, sys.getrefcount(key))
            self.assertEqual(5, sys.getrefcount(value))

    def test_setitem_wrong_type(self):
        key = object()
        value = self.small_key()
        with self.assertRaises(TypeError) as ctx:
            self.sorted_dict[key] = value
        self.assertEqual(self.wrong_argument, ctx.exception.args[0])

        if self.cpython:
            self.assertEqual(2, sys.getrefcount(value))

    def test_setitem_existing(self):
        idx, key = self.rg.choice([*enumerate(self.normal_dict)])
        value = self.small_key()
        self.sorted_dict[key] = value
        self.assertEqual(value, self.sorted_dict[key])

        if self.cpython:
            self.assertEqual(5, sys.getrefcount(key))
            self.assertEqual(3, sys.getrefcount(value))
            self.values_refcounts[idx] -= 1

    def test_setitem_new(self):
        key = self.large_key()
        value = self.small_key()
        self.sorted_dict[key] = value
        self.assertEqual(value, self.sorted_dict[key])

        if self.cpython:
            self.assertEqual(3, sys.getrefcount(key))
            self.assertEqual(3, sys.getrefcount(value))

    def test_setitem_remove_not_found(self):
        key = self.large_key()
        with self.assertRaises(KeyError) as ctx:
            del self.sorted_dict[key]
        self.assertEqual(key, ctx.exception.args[0])

        if self.cpython:
            self.assertEqual(3, sys.getrefcount(key))

    def test_setitem_remove_existing(self):
        idx, key = self.rg.choice([*enumerate(self.normal_dict)])
        del self.sorted_dict[key]
        with self.assertRaises(KeyError) as ctx:
            self.sorted_dict[key]
        self.assertEqual(key, ctx.exception.args[0])

        if self.cpython:
            self.assertEqual(5, sys.getrefcount(key))
            self.keys_refcounts[idx] -= 1
            self.values_refcounts[idx] -= 1

    def test_str(self):
        self.assertEqual(str(dict(sorted(self.normal_dict.items()))), str(self.sorted_dict))

    def test_str_preserved(self):
        self._str = str(self.sorted_dict)
        self.assertEqual(str(dict(sorted(self.normal_dict.items()))), self._str)

        if self.cpython:
            self.assertEqual(2, sys.getrefcount(self._str))

    def test_items(self):
        self.assertEqual(sorted(self.normal_dict.items()), self.sorted_dict.items())

    def test_items_preserved(self):
        self._items = self.sorted_dict.items()
        self.assertEqual(sorted(self.normal_dict.items()), self._items)

        if self.cpython:
            self.assertEqual(2, sys.getrefcount(self._items))
            self.keys_refcounts = [5] * len(self.normal_dict)
            self.values_refcounts = [5] * len(self.normal_dict)

    def test_keys(self):
        self.assertEqual(sorted(self.normal_dict.keys()), self.sorted_dict.keys())

    def test_keys_preserved(self):
        self._keys = self.sorted_dict.keys()
        self.assertEqual(sorted(self.normal_dict.keys()), self._keys)

        if self.cpython:
            self.assertEqual(2, sys.getrefcount(self._keys))
            self.keys_refcounts = [5] * len(self.normal_dict)

    def test_values(self):
        self.assertEqual([item[1] for item in sorted(self.normal_dict.items())], self.sorted_dict.values())

    def test_values_preserved(self):
        self._values = self.sorted_dict.values()
        self.assertEqual([item[1] for item in sorted(self.normal_dict.items())], self._values)

        if self.cpython:
            self.assertEqual(2, sys.getrefcount(self._values))
            self.values_refcounts = [5] * len(self.normal_dict)

    def test_del(self):
        del self.sorted_dict

        if self.cpython:
            self.keys_refcounts = [3] * len(self.normal_dict)
            self.values_refcounts = [3] * len(self.normal_dict)

    def test_empty(self):
        sorted_dict = SortedDict(self.key_type)
        self.assertEqual("{}", str(sorted_dict))
        self.assertEqual(0, len(sorted_dict))

    def tearDown(self):
        if self.cpython:
            for expected, observed in zip(self.keys_refcounts, map(sys.getrefcount, self.normal_dict), strict=True):
                self.assertEqual(expected, observed)
            for expected, observed in zip(
                self.values_refcounts, map(sys.getrefcount, self.normal_dict.values()), strict=True
            ):
                self.assertEqual(expected, observed)
