import random

import pytest

from pysorteddict import SortedDict

rg = random.Random(__name__)


class TestFuzzy:
    def _set_up(self):
        self.key_type = str
        self.normal_dict = {}
        self.sorted_dict = SortedDict()

    def test_fuzzy(self):
        self._set_up()

        for attr in rg.choices(dir(SortedDict), k=1000000):
            match attr:
                case "__delattr__":
                    self._test_delattr()
                case "__contains__":
                    self._test_contains()

    def _test_delattr(self):
        with pytest.raises(
            AttributeError, match="^attribute 'key_type' of 'pysorteddict.SortedDict' objects is not writable$"
        ):
            delattr(self.sorted_dict, "key_type")
        for attr in ("clear", "copy", "items", "keys", "values"):
            with pytest.raises(
                AttributeError, match=f"^'pysorteddict.SortedDict' object attribute {attr!r} is read-only$"
            ):
                delattr(self.sorted_dict, attr)

    def _test_contains(self):
        pass


if __name__ == "__main__":
    pytest.main()
