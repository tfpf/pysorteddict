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
        cls.wrong_argument = ""

    def setUp(self, key_type: type):
        self.rg = random.Random(__name__)
        self.keys = [self.small_key() for _ in range(1000)]
        self.values = [self.small_key() for _ in self.keys]
        self.normal_dict = dict(zip(self.keys, self.values, strict=True))
        self.sorted_dict = SortedDict(key_type)
        for key, value in zip(self.keys, self.values, strict=True):
            self.sorted_dict[key] = value

        # Store the reference count of an item in a list at the position at
        # which it appears in the normal dictionary. At this point, the
        # reference counts are all 3, but querying the reference count
        # increases it, so I store 4. Whenever a test changes the reference
        # count of any item, I set the new reference count at its index.
        self.cpython = platform.python_implementation() == "CPython"
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
