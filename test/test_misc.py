import unittest

from pysorteddict import SortedDict


class TestMisc(unittest.TestCase):
    @classmethod
    def setUpClass(self):
        self.missing_argument = "function missing required argument 'key_type' (pos 1)"
        self.wrong_argument = "constructor argument must be a supported type"

    def test_construct_without_argument(self):
        with self.assertRaises(TypeError) as ctx:
            SortedDict()
        self.assertEqual(ctx.exception.args[0], self.missing_argument)

    def test_construct_with_bool_instance(self):
        with self.assertRaises(ValueError) as ctx:
            SortedDict(False)
        self.assertEqual(ctx.exception.args[0], self.wrong_argument)

    def test_construct_with_bytes_instance(self):
        with self.assertRaises(ValueError) as ctx:
            SortedDict(b"")
        self.assertEqual(ctx.exception.args[0], self.wrong_argument)

    def test_construct_with_bytearray_instance(self):
        with self.assertRaises(ValueError) as ctx:
            SortedDict(bytearray())
        self.assertEqual(ctx.exception.args[0], self.wrong_argument)

    def test_construct_with_complex_instance(self):
        with self.assertRaises(ValueError) as ctx:
            SortedDict(0j)
        self.assertEqual(ctx.exception.args[0], self.wrong_argument)

    def test_construct_with_float_instance(self):
        with self.assertRaises(ValueError) as ctx:
            SortedDict(0.0)
        self.assertEqual(ctx.exception.args[0], self.wrong_argument)

    def test_construct_with_int_instance(self):
        with self.assertRaises(ValueError) as ctx:
            SortedDict(0)
        self.assertEqual(ctx.exception.args[0], self.wrong_argument)

    def test_construct_with_list_instance(self):
        with self.assertRaises(ValueError) as ctx:
            SortedDict([])
        self.assertEqual(ctx.exception.args[0], self.wrong_argument)

    def test_construct_with_memoryview_instance(self):
        with self.assertRaises(ValueError) as ctx:
            SortedDict(memoryview(b""))
        self.assertEqual(ctx.exception.args[0], self.wrong_argument)

    def test_construct_with_range_instance(self):
        with self.assertRaises(ValueError) as ctx:
            SortedDict(range(0))
        self.assertEqual(ctx.exception.args[0], self.wrong_argument)

    def test_construct_with_str_instance(self):
        with self.assertRaises(ValueError) as ctx:
            SortedDict("")
        self.assertEqual(ctx.exception.args[0], self.wrong_argument)

    def test_construct_with_tuple_instance(self):
        with self.assertRaises(ValueError) as ctx:
            SortedDict(())
        self.assertEqual(ctx.exception.args[0], self.wrong_argument)
