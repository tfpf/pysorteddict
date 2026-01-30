import re
import string
from datetime import date, timedelta
from decimal import Decimal
from fractions import Fraction
from ipaddress import IPv4Address, IPv4Interface, IPv4Network, IPv6Address, IPv6Interface, IPv6Network
from pathlib import Path, PurePath
from uuid import UUID

import pytest
from hypothesis import settings
from hypothesis import strategies as st
from hypothesis.stateful import RuleBasedStateMachine, invariant, precondition, rule

from pysorteddict import SortedDict

settings.register_profile("default", max_examples=200, stateful_step_count=100)

strategy_mapping = {
    bool: st.booleans(),
    bytes: st.binary(),
    float: st.floats(allow_nan=False),
    int: st.integers(),
    str: st.text(),
    date: st.dates(),
    timedelta: st.timedeltas(),
    Decimal: st.decimals(allow_nan=False),
    Fraction: st.fractions(),
    IPv4Address: st.ip_addresses(v=4),
    IPv4Interface: st.from_type(IPv4Interface),
    IPv4Network: st.from_type(IPv4Network),
    IPv6Address: st.ip_addresses(v=6),
    IPv6Interface: st.from_type(IPv6Interface),
    IPv6Network: st.from_type(IPv6Network),
    type(Path()): st.builds(Path, st.text(alphabet=string.ascii_lowercase + "/")),
    type(PurePath()): st.builds(PurePath, st.text(alphabet=string.ascii_lowercase + "/")),
    # The representation of objects of this type in Python is different from
    # that when using the C++ API. I must resolve that difference first.
    # st.builds(time.localtime, st.integers(min_value=0, max_value=2**30)),
    UUID: st.uuids(),
}
strategy_mapping_complement = {
    tp: st.one_of(other_strat for other_tp, other_strat in strategy_mapping.items() if other_tp is not tp)
    for tp in strategy_mapping
}
supported_keys = st.one_of(strategy_mapping.values())
unsupported_keys = st.lists(st.integers())
all_keys = st.one_of(supported_keys, unsupported_keys)


def prec_key_type_not_set(self):
    return self.key_type is None


def prec_key_type_set(self):
    return self.key_type is not None


def prec_key_type_admits_nan(self):
    return any(self.key_type is key_type for key_type in [float, Decimal])


def rule_key_wrong_type():
    return st.runner().flatmap(lambda self: strategy_mapping_complement[self.key_type])


def rule_key_right_type():
    return st.runner().flatmap(lambda self: strategy_mapping[self.key_type])


def rule_key_exists():
    return st.runner().flatmap(lambda self: st.sampled_from(self.keys))


def rule_key_is_nan():
    return st.runner().flatmap(
        lambda self: st.sampled_from([float("nan"), Decimal("nan")])
        if self.key_type is None
        else st.just(self.key_type("nan"))
    )


class SortedDictionaryChecker(RuleBasedStateMachine):
    def __init__(self, sorted_dict_type=SortedDict):
        super().__init__()
        self.key_type = None
        self.keys = []
        self.normal_dict = {}
        self.sorted_dict = sorted_dict_type()

    @invariant()
    def always(self):
        sorted_normal_dict = dict(sorted(self.normal_dict.items()))
        assert repr(self.sorted_dict) == f"SortedDict({sorted_normal_dict})"
        assert len(self.sorted_dict) == len(sorted_normal_dict)
        assert all(a == b for a, b in zip(self.sorted_dict, sorted_normal_dict, strict=True))
        assert all(a == b for a, b in zip(reversed(self.sorted_dict), reversed(sorted_normal_dict), strict=True))

    ###########################################################################
    # `contains`.
    ###########################################################################

    @precondition(prec_key_type_not_set)
    @rule(key=all_keys)
    def contains_key_type_not_set(self, key):
        with pytest.raises(RuntimeError, match="key type not set: insert at least one item first"):
            _ = key in self.sorted_dict

    @precondition(prec_key_type_set)
    @rule(key=rule_key_wrong_type())
    def contains_wrong_type(self, key):
        with pytest.raises(
            TypeError, match=re.escape(f"got key {key!r} of type {type(key)}, want key of type {self.key_type}")
        ):
            _ = key in self.sorted_dict

    @precondition(prec_key_type_admits_nan)
    @rule(key=rule_key_is_nan())
    def contains_nan(self, key):
        with pytest.raises(ValueError, match=re.escape(f"got bad key {key!r} of type {type(key)}")):
            _ = key in self.sorted_dict

    @precondition(prec_key_type_set)
    @rule(key=rule_key_right_type())
    def contains_probably_false(self, key):
        assert (key in self.sorted_dict) == (key in self.normal_dict)

    @precondition(prec_key_type_set)
    @rule(key=rule_key_exists())
    def contains_true(self, key):
        assert key in self.sorted_dict

    ###########################################################################
    # `getitem`.
    ###########################################################################

    @precondition(prec_key_type_not_set)
    @rule(key=all_keys)
    def getitem_key_type_not_set(self, key):
        with pytest.raises(RuntimeError, match="key type not set: insert at least one item first"):
            self.sorted_dict[key]

    @precondition(prec_key_type_set)
    @rule(key=rule_key_wrong_type())
    def getitem_wrong_type(self, key):
        with pytest.raises(
            TypeError, match=re.escape(f"got key {key!r} of type {type(key)}, want key of type {self.key_type}")
        ):
            self.sorted_dict[key]

    @precondition(prec_key_type_admits_nan)
    @rule(key=rule_key_is_nan())
    def getitem_nan(self, key):
        with pytest.raises(ValueError, match=re.escape(f"got bad key {key!r} of type {type(key)}")):
            self.sorted_dict[key]

    @precondition(prec_key_type_set)
    @rule(key=rule_key_right_type())
    def getitem_probably_key_error(self, key):
        if key not in self.normal_dict:
            with pytest.raises(KeyError, match=re.escape(f"{key!r}")):
                self.sorted_dict[key]
        else:
            assert self.sorted_dict[key] == self.normal_dict[key]

    @precondition(prec_key_type_set)
    @rule(key=rule_key_exists())
    def getitem(self, key):
        assert self.sorted_dict[key] == self.normal_dict[key]

    ###########################################################################
    # `setitem`.
    ###########################################################################

    @precondition(prec_key_type_not_set)
    @rule(key=unsupported_keys)
    def setitem_key_unsupported(self, key):
        with pytest.raises(TypeError, match=re.escape(f"got key {key!r} of unsupported type {type(key)}")):
            self.sorted_dict[key] = None

    @precondition(prec_key_type_not_set)
    @rule(key=rule_key_is_nan())
    def setitem_nan_empty(self, key):
        with pytest.raises(ValueError, match=re.escape(f"got bad key {key!r} of type {type(key)}")):
            self.sorted_dict[key] = None

    @precondition(prec_key_type_set)
    @rule(key=rule_key_wrong_type())
    def setitem_wrong_type(self, key):
        with pytest.raises(
            TypeError, match=re.escape(f"got key {key!r} of type {type(key)}, want key of type {self.key_type}")
        ):
            self.sorted_dict[key] = None

    @precondition(prec_key_type_admits_nan)
    @rule(key=rule_key_is_nan())
    def setitem_nan(self, key):
        with pytest.raises(ValueError, match=re.escape(f"got bad key {key!r} of type {type(key)}")):
            self.sorted_dict[key] = None

    @precondition(prec_key_type_not_set)
    @rule(key=supported_keys, value=st.integers())
    def setitem_empty(self, key, value):
        self.key_type = type(key)
        self.keys.append(key)
        self.normal_dict[key] = value
        self.sorted_dict[key] = value

    @precondition(prec_key_type_set)
    @rule(key=rule_key_right_type(), value=st.integers())
    def setitem_probably_new(self, key, value):
        if key not in self.normal_dict:
            self.keys.append(key)
        self.normal_dict[key] = value
        self.sorted_dict[key] = value

    @precondition(prec_key_type_set)
    @rule(key=rule_key_exists(), value=st.integers())
    def setitem_existing(self, key, value):
        self.normal_dict[key] = value
        self.sorted_dict[key] = value

    ###########################################################################
    # `delitem`.
    ###########################################################################


TestSortedDictionary = SortedDictionaryChecker.TestCase
