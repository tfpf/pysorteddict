import unittest

from pysorteddict import SortedDict

class TestMisc(unittest.TestCase):

    def test_construct_without_argument(self):
        with self.assertRaises(TypeError):
            SortedDict()

    def test_construct_with_bool_instance(self):
        with self.assertRaises(ValueError):
            SortedDict(False)

    def test_construct_with_bytes_instance(self):
        with self.assertRaises(ValueError):
            SortedDict(b"")

    def test_construct_with_bytearray_instance(self):
        with self.assertRaises(ValueError):
            SortedDict(bytearray())

    def test_construct_with_complex_instance(self):
        with self.assertRaises(ValueError):
            SortedDict(0j)

    def test_construct_with_float_instance(self):
        with self.assertRaises(ValueError):
            SortedDict(0.0)

    def test_construct_with_int_instance(self):
        with self.assertRaises(ValueError):
            SortedDict(0)

    def test_construct_with_list_instance(self):
        with self.assertRaises(ValueError):
            SortedDict([])

    def test_construct_with_range_instance(self):
        with self.assertRaises(ValueError):
            SortedDict(range(0))

    def test_construct_with_str_instance(self):
        with self.assertRaises(ValueError):
            SortedDict("")

    def test_construct_with_tuple_instance(self):
        with self.assertRaises(ValueError):
            SortedDict(())
