from decimal import Decimal

import pytest
from hypothesis import settings
from hypothesis import strategies as st
from hypothesis.stateful import Bundle, RuleBasedStateMachine, invariant, precondition, rule

from pysorteddict import SortedDict

settings.register_profile("default", max_examples=200, stateful_step_count=100)
supported_keys = (
    st.booleans()
    | st.binary()
    | st.floats(allow_nan=False)
    | st.integers()
    | st.text()
    | st.dates()
    | st.timedeltas()
    | st.decimals(allow_nan=False)
    | st.fractions()
    | st.ip_addresses(v=4)
    | st.ip_addresses(v=6)
    | st.uuids()
)
unsupported_keys = st.none() | st.lists(st.integers()) | st.tuples(st.integers())
unsupported_supported_keys = supported_keys | unsupported_keys


class SortedDictionaryChecker(RuleBasedStateMachine):
    def __init__(self):
        super().__init__()
        self.key_type = None
        self.normal_dict = {}
        self.sorted_dict = SortedDict()

    keys, values = Bundle("keys"), Bundle("values")

    @rule(target=keys, key=supported_keys)
    def add_key(self, key):
        return key

    @rule(target=values, value=st.integers())
    def add_value(self, value):
        return value

    @invariant()
    def always(self):
        sorted_normal_dict = dict(sorted(self.normal_dict.items()))
        assert repr(self.sorted_dict) == f"SortedDict({sorted_normal_dict})"
        assert len(self.sorted_dict) == len(sorted_normal_dict)
        assert all(a == b for a, b in zip(self.sorted_dict, sorted_normal_dict, strict=True))
        assert all(a == b for a, b in zip(reversed(self.sorted_dict), reversed(sorted_normal_dict), strict=True))

    @precondition(lambda self: self.key_type is None)
    @rule(key=unsupported_supported_keys)
    def contains_key_type_not_set(self, key):
        with pytest.raises(RuntimeError):
            _ = key in self.sorted_dict

    @precondition(lambda self: self.key_type is not None)
    @rule(key=unsupported_supported_keys | keys)
    def contains_wrong_type(self, key):
        if type(key) is not self.key_type:
            with pytest.raises(TypeError):
                _ = key in self.sorted_dict
        else:
            assert (key in self.sorted_dict) == (key in self.normal_dict)

    @precondition(lambda self: self.key_type is float or self.key_type is Decimal)
    @rule()
    def contains_nan(self):
        key = self.key_type("nan")
        with pytest.raises(ValueError, match=rf"^got bad key {key} of type {type(key)}$"):
            _ = key in self.sorted_dict

    @rule(key=st.floats(), value=st.integers())
    def setitem(self, key, value):
        self.key_type = self.key_type or type(key)
        self.normal_dict[key] = value
        self.sorted_dict[key] = value


TestSortedDictionary = SortedDictionaryChecker.TestCase
