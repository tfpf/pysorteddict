import builtins
import math
import random
import string

import pytest

from pysorteddict import SortedDict

supported_types = {bytes, int, float, str}
unsupported_types = {bool, bytearray, complex, dict, frozenset, list, set, tuple}


class TestFuzzy:
    _rg = random.Random(__name__)

    def _set_up(self):
        self.key_type = self._rg.choice([*supported_types])
        self.normal_dict = {}
        self.sorted_dict = SortedDict()

    def _gen(self):
        match self.key_type:
            case builtins.bytes:
                return self._rg.randbytes(self._rg.randrange(16, 32))
            case builtins.int:
                return self._rg.randrange(1_000, 2_000)
            case builtins.float:
                return self._rg.choice([f / 1_000 for f in range(1_000)] + [-math.inf, math.inf])
            case builtins.str:
                return "".join(self._rg.choices(string.ascii_lowercase, k=self._rg.randrange(20, 30)))
            case key_type:
                raise RuntimeError(key_type)

    def test_fuzzy(self):
        self._set_up()

        for attr in self._rg.choices(dir(SortedDict), k=1_000_000):
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
