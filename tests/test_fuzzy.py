import pytest
from pysorteddict import SortedDict
from sys import getrefcount
import random

rg = random.Random(__name__)

class TestFuzzy:

    def _set_up(self):
        self.key_type = str

    def test_fuzzy(self):
        self._set_up()

        for attr in rg.choices(dir(SortedDict)):
            match attr:
                case "__contains__":
                    self._test_contains()
                case attr:
                    raise RuntimeError(attr)

    def _test_contains(self):
        pass

