import pytest
from pysorteddict import SortedDict
from sys import getrefcount
import random

rg = random.Random(__name__)

class TestFuzzy:

    def set_up(self):
        self.key_type = str

    def _test_contains(self):
        pass

    def test_fuzzy(self):
        self.set_up()

        attrs = dir(SortedDict)
        for _ in range(10):
            match rg.choice(attrs):
                case "__contains__":
                    self._test_contains()
                case attr:
                    raise RuntimeError(attr)
