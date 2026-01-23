import pytest
from hypothesis import settings
from hypothesis import strategies as st
from hypothesis.stateful import Bundle, RuleBasedStateMachine, invariant, precondition, rule

from pysorteddict import SortedDict

settings.register_profile("default", max_examples=200, stateful_step_count=100)


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

    @invariant()
    def always(self):
        sorted_normal_dict = dict(sorted(self.normal_dict.items()))
        assert repr(self.sorted_dict) == f"SortedDict({sorted_normal_dict})"
        assert len(self.sorted_dict) == len(sorted_normal_dict)
        assert all(a == b for a, b in zip(self.sorted_dict, sorted_normal_dict, strict=True))
        assert all(a == b for a, b in zip(reversed(self.sorted_dict), reversed(sorted_normal_dict), strict=True))

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
