import pytest
from hypothesis import strategies as st
from hypothesis.stateful import Bundle, RuleBasedStateMachine, precondition, rule

from pysorteddict import SortedDict


class SortedDictionaryChecker(RuleBasedStateMachine):
    def __init__(self):
        super().__init__()
        self.key_type = None
        self.normal_dict = {}
        self.sorted_dict = SortedDict()

    keys, values = Bundle("keys"), Bundle("values")

    @rule(target=keys, k=st.integers())
    def add_key(self, k):
        return k

    @rule(target=values, v=st.integers())
    def add_value(self, v):
        return v

    @rule(k=keys, v=values)
    def setitem(self, k, v):
        self.key_type = type(k)
        self.normal_dict[k] = v
        self.sorted_dict[k] = v

    @precondition(lambda self: self.key_type is not None)
    @rule(k=keys)
    def contains(self, k):
        assert (k in self.normal_dict) == (k in self.sorted_dict)

    @precondition(lambda self: self.key_type is None)
    @rule(k=keys)
    def contains_error(self, k):
        with pytest.raises(RuntimeError):
            _ = k in self.sorted_dict


TestSortedDictionary = SortedDictionaryChecker.TestCase
