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

    def test_construct_with_object_instance(self):
        with self.assertRaises(ValueError) as ctx:
            SortedDict(object())
        self.assertEqual(ctx.exception.args[0], self.wrong_argument)
