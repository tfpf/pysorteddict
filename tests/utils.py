import platform
import random
import sys

from pysorteddict import SortedDict
import pytest


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
        self.key_subtype = type("sub" + self.key_type.__name__, (self.key_type,), {})
        self.rg = random.Random(__name__)
        self.keys = [self.small_key() for _ in range(1000)]
        self.values = [self.small_key() for _ in self.keys]
        self.normal_dict = dict(zip(self.keys, self.values, strict=True))
        sorted_dict = SortedDict(self.key_type)
        for key, value in zip(self.keys, self.values, strict=True):
            sorted_dict[key] = value
        self.sorted_dicts = [sorted_dict, sorted_dict.copy()]

        # Store the reference count of an item in a list at the position at
        # which it appears in the normal dictionary. At this point, the
        # reference counts are all 4, but querying the reference count
        # increases it, so I store 5. Whenever a test changes the reference
        # count of any item, I set the new reference count at its index.
        # Remember that reference counting is specific to the CPython
        # implementation.
        if self.cpython:
            self.keys_refcounts = [5] * len(self.normal_dict)
            self.values_refcounts = [5] * len(self.normal_dict)

    @pytest.mark.parametrize("sorted_dict", self.sorted_dicts)
    def test_contains_wrong_type(self):
        assert self.key_subtype() not in sorted_dict

    def test_contains_false(self):
        key = self.large_key()
        self.assertTrue(key not in self.sorted_dict)

        if self.cpython:
            self.assertEqual(2, sys.getrefcount(key))

    def test_contains_true(self):
        key = self.rg.choice(self.keys)
        self.assertTrue(key in self.sorted_dict)

        if self.cpython:
            self.assertEqual(5, sys.getrefcount(key))

    def test_len(self):
        self.assertEqual(len(self.normal_dict), len(self.sorted_dict))

    def test_getitem_wrong_type(self):
        with self.assertRaises(TypeError) as ctx:
            self.sorted_dict[self.key_subtype()]
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
        value = self.small_key()
        with self.assertRaises(TypeError) as ctx:
            self.sorted_dict[self.key_subtype()] = value
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

    def test_clear(self):
        self.sorted_dict.clear()
        self.assertEqual("{}", str(self.sorted_dict))
        self.assertEqual(0, len(self.sorted_dict))

        if self.cpython:
            self.keys_refcounts = [3] * len(self.normal_dict)
            self.values_refcounts = [3] * len(self.normal_dict)

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

    def test_stress(self):
        self.normal_dict = {}
        self.sorted_dict = SortedDict(self.key_type)
        for method in self.rg.choices(["__contains__", "__delitem__", "__getitem__", "__setitem__", "clear"], k=10000):
            key, value = self.small_key(), self.small_key()
            match method:
                case "__contains__":
                    self.assertEqual(key in self.normal_dict, key in self.sorted_dict)

                case "__delitem__":
                    try:
                        del self.normal_dict[key]
                    except KeyError as exc:
                        with self.assertRaises(KeyError) as ctx:
                            del self.sorted_dict[key]
                        self.assertEqual(exc.args[0], ctx.exception.args[0])
                    else:
                        del self.sorted_dict[key]

                case "__getitem__":
                    try:
                        value = self.normal_dict[key]
                    except KeyError as exc:
                        with self.assertRaises(KeyError) as ctx:
                            self.sorted_dict[key]
                        self.assertEqual(exc.args[0], ctx.exception.args[0])
                    else:
                        self.assertEqual(value, self.sorted_dict[key])

                case "__setitem__":
                    self.normal_dict[key] = value
                    self.sorted_dict[key] = value
                    self.assertEqual(value, self.sorted_dict[key])

                case "clear":
                    self.normal_dict.clear()
                    self.sorted_dict.clear()

            # I know this is bad, but can't think of a better way to avoid
            # repeating code.
            self.test_len()
            self.test_str()
            self.test_items()
            self.test_keys()
            self.test_values()

        if self.cpython:
            self.keys_refcounts = [3] * len(self.normal_dict)
            self.values_refcounts = [3] * len(self.normal_dict)

    def tearDown(self):
        if self.cpython:
            for expected, observed in zip(self.keys_refcounts, map(sys.getrefcount, self.normal_dict), strict=True):
                self.assertEqual(expected, observed)
            for expected, observed in zip(
                self.values_refcounts, map(sys.getrefcount, self.normal_dict.values()), strict=True
            ):
                self.assertEqual(expected, observed)
