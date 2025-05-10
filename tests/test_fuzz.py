import builtins
import math
import os
import random
import string

import pytest

from pysorteddict import SortedDict

unsupported_types = (bool, bytearray, complex, dict, frozenset, list, set, tuple)
supported_types = (bytes, int, float, str)
all_types = unsupported_types + supported_types


class TestFuzz:
    def _gen(self, key_type: type | None = None):
        match key_type or self.key_type:
            case builtins.bool:
                return bool(self._rg.getrandbits(1))
            case builtins.bytearray | builtins.frozenset | builtins.list | builtins.set | builtins.tuple:
                return key_type(self._gen(bytes))
            case builtins.complex:
                return self._gen(float) + self._gen(float) * 1j
            case builtins.bytes:
                return self._rg.randbytes(self._rg.randrange(16, 32))
            case builtins.dict:
                return {b: b for b in self._gen(bytes)}
            case builtins.int:
                return self._rg.randrange(1_000, 2_000)
            case builtins.float:
                # I want a non-negligible repetition chance. Hence the kludge.
                return self._rg.choices([*range(1_000)] + [-math.inf, math.inf, math.nan], weights=[1] * 1_002 + [100])[0] / 1_000
            case builtins.str:
                return "".join(self._rg.choices(string.ascii_lowercase, k=self._rg.randrange(20, 30)))
            case key_type:
                raise RuntimeError(key_type)

    @pytest.mark.parametrize("key_type", supported_types)
    def test_fuzz(self, key_type: type):
        self._rg = random.Random(f"{__name__}-{key_type.__name__}")
        self.key_type = key_type
        self.normal_dict = {}
        self.sorted_dict = SortedDict()

        for attr in self._rg.choices(dir(SortedDict), k=100_000):
            match attr:
                case "__delattr__":
                    self._test_delattr()
                case "__contains__":
                    self._test_contains()
                case "__delitem__":
                    self._test_delitem()

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
        for key_type in all_types:
            key = self._gen(key_type)
            if key_type is not self.key_type:
                assert key not in self.sorted_dict
                continue
            if self.normal_dict:
                assert self._rg.choice([*self.normal_dict]) in self.sorted_dict
            assert (key in self.sorted_dict) == (key in self.normal_dict)

    def _test_delitem(self):
        for key_type in all_types:
            key = self._gen(key_type)
            if not self.normal_dict:
                with pytest.raises(ValueError, match="^key type not set: insert at least one item first$"):
                    del self.sorted_dict[key]
                continue
            if key_type is not self.key_type:
                with pytest.raises(TypeError, match=f"^wrong key type: want {self.key_type!r}, got {key_type!r}$"):
                    del self.sorted_dict[key]
                continue
            if key not in self.normal_dict:
                with pytest.raises(KeyError, match=str(key)):
                    del self.sorted_dict[key]
                continue
            del self.normal_dict[key]
            del self.sorted_dict[key]


if __name__ == "__main__":
    pytest.main()
