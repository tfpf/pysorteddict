import re
import string
import time
from decimal import Decimal
from ipaddress import IPv4Interface, IPv4Network, IPv6Interface, IPv6Network
from pathlib import Path, PurePath

import pytest
from hypothesis import settings
from hypothesis import strategies as st
from hypothesis.stateful import Bundle, RuleBasedStateMachine, invariant, precondition, rule

from pysorteddict import SortedDict

settings.register_profile("default", max_examples=200, stateful_step_count=100)
supported_keys = st.one_of(
    st.booleans(),
    st.binary(),
    st.floats(allow_nan=False),
    st.integers(),
    st.text(),
    st.dates(),
    st.timedeltas(),
    st.decimals(allow_nan=False),
    st.fractions(),
    st.ip_addresses(v=4),
    st.from_type(IPv4Interface),
    st.from_type(IPv4Network),
    st.ip_addresses(v=6),
    st.from_type(IPv6Interface),
    st.from_type(IPv6Network),
    st.builds(Path, st.text(alphabet=string.ascii_lowercase + "/")),
    st.builds(PurePath, st.text(alphabet=string.ascii_lowercase + "/")),
    st.builds(time.localtime, st.integers(min_value=0, max_value=2**30)),
    st.uuids(),
)
unsupported_keys = st.lists(st.integers()) | st.tuples(st.integers())
unsupported_supported_keys = unsupported_keys | supported_keys


def prec_key_type_not_set(self):
    return self.key_type is None


def prec_key_type_set(self):
    return self.key_type is not None


def prec_key_type_admits_nan(self):
    return any(self.key_type is key_type for key_type in [float, Decimal])


class SortedDictionaryChecker(RuleBasedStateMachine):
    def __init__(self, sorted_dict_type=SortedDict):
        super().__init__()
        self.key_type = None
        self.normal_dict = {}
        self.sorted_dict = sorted_dict_type()

    keys = Bundle("keys")

    @rule(target=keys, key=supported_keys)
    def add_key(self, key):
        return key

    @invariant()
    def always(self):
        sorted_normal_dict = dict(sorted(self.normal_dict.items()))
        assert repr(self.sorted_dict) == f"SortedDict({sorted_normal_dict})"
        assert len(self.sorted_dict) == len(sorted_normal_dict)
        assert all(a == b for a, b in zip(self.sorted_dict, sorted_normal_dict, strict=True))
        assert all(a == b for a, b in zip(reversed(self.sorted_dict), reversed(sorted_normal_dict), strict=True))

    ###########################################################################
    # `contains` tests.
    ###########################################################################

    @precondition(prec_key_type_not_set)
    @rule(key=unsupported_supported_keys)
    def contains_key_type_not_set(self, key):
        with pytest.raises(RuntimeError, match="key type not set: insert at least one item first"):
            _ = key in self.sorted_dict

    @precondition(prec_key_type_set)
    @rule(key=unsupported_supported_keys | keys)
    def contains_wrong_type(self, key):
        if type(key) is not self.key_type:
            with pytest.raises(
                TypeError, match=re.escape(f"got key {key!r} of type {type(key)}, want key of type {self.key_type}")
            ):
                _ = key in self.sorted_dict
        else:
            assert (key in self.sorted_dict) == (key in self.normal_dict)

    @precondition(prec_key_type_admits_nan)
    @rule()
    def contains_nan(self):
        key = self.key_type("nan")
        with pytest.raises(ValueError, match=re.escape(f"got bad key {key!r} of type {type(key)}")):
            _ = key in self.sorted_dict

    ###########################################################################
    # `setitem` tests.
    ###########################################################################

    @precondition(prec_key_type_not_set)
    @rule(key=unsupported_keys)
    def setitem_key_unsupported(self, key):
        with pytest.raises(TypeError, match=re.escape(f"got key {key!r} of unsupported type {type(key)}")):
            self.sorted_dict[key] = None

    @precondition(prec_key_type_not_set)
    @rule(key=st.sampled_from([float("nan"), Decimal("nan")]))
    def setitem_nan_empty(self, key):
        with pytest.raises(ValueError, match=re.escape(f"got bad key {key!r} of type {type(key)}")):
            self.sorted_dict[key] = None

    @precondition(prec_key_type_set)
    @rule(key=unsupported_supported_keys | keys, value=st.integers())
    def setitem_wrong_type(self, key, value):
        if type(key) is not self.key_type:
            with pytest.raises(
                TypeError, match=re.escape(f"got key {key!r} of type {type(key)}, want key of type {self.key_type}")
            ):
                self.sorted_dict[key] = None
        else:
            self.normal_dict[key] = value
            self.sorted_dict[key] = value

    @precondition(prec_key_type_admits_nan)
    @rule()
    def setitem_nan(self):
        key = self.key_type("nan")
        with pytest.raises(ValueError, match=re.escape(f"got bad key {key!r} of type {type(key)}")):
            self.sorted_dict[key] = None

    @precondition(prec_key_type_not_set)
    @rule(key=supported_keys, value=st.integers())
    def setitem_empty(self, key, value):
        self.key_type = type(key)
        self.normal_dict[key] = value
        self.sorted_dict[key] = value


TestSortedDictionary = SortedDictionaryChecker.TestCase
