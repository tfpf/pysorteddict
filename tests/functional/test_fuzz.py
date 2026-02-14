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


def prec_keys_not_empty(self):
    return bool(self.keys)


def rule_key_wrong_type():
    return st.runner().flatmap(lambda self: strategy_mapping_complement[self.key_type])


def rule_key_right_type():
    return st.runner().flatmap(lambda self: strategy_mapping[self.key_type])


def rule_key_exists():
    return st.runner().flatmap(lambda self: st.sampled_from(self.keys))


def rule_key_is_nan():
    return st.runner().flatmap(
        lambda self: st.just(self.key_type("nan"))
        if self.key_type is not None
        else st.sampled_from((float("nan"), Decimal("nan")))
    )


def rule_sorted_dict_or_sorted_dict_keys():
    return st.runner().flatmap(lambda self: st.sampled_from((self.sorted_dict, self.sorted_dict_keys)))


class SortedDictChecker(RuleBasedStateMachine):
    def __init__(self):
        super().__init__()
        self.reinitialise()

    def reinitialise(self):
        self.key_type = None
        self.keys = []
        self.normal_dict = {}
        self.sorted_dict = SortedDict()
        self.sorted_dict_items = self.sorted_dict.items()
        self.sorted_dict_keys = self.sorted_dict.keys()
        self.sorted_dict_values = self.sorted_dict.values()

    @invariant()
    def always(self):
        sorted_normal_dict = dict(sorted(self.normal_dict.items()))
        sorted_normal_dict_items_list = [*sorted_normal_dict.items()]
        sorted_normal_dict_keys_list = [*sorted_normal_dict.keys()]
        sorted_normal_dict_values_list = [*sorted_normal_dict.values()]
        assert repr(self.sorted_dict) == f"SortedDict({sorted_normal_dict})"
        assert repr(self.sorted_dict_items) == f"SortedDictItems({sorted_normal_dict_items_list})"
        assert repr(self.sorted_dict_keys) == f"SortedDictKeys({sorted_normal_dict_keys_list})"
        assert repr(self.sorted_dict_values) == f"SortedDictValues({sorted_normal_dict_values_list})"

        sorted_normal_dict_len = len(sorted_normal_dict)
        assert len(self.sorted_dict) == sorted_normal_dict_len
        assert len(self.sorted_dict_items) == sorted_normal_dict_len
        assert len(self.sorted_dict_keys) == sorted_normal_dict_len
        assert len(self.sorted_dict_values) == sorted_normal_dict_len

        assert [*self.sorted_dict] == [*sorted_normal_dict]
        assert [*reversed(self.sorted_dict)] == [*reversed(sorted_normal_dict)]
        assert [*self.sorted_dict_items] == sorted_normal_dict_items_list
        assert [*reversed(self.sorted_dict_items)] == sorted_normal_dict_items_list[::-1]
        assert [*self.sorted_dict_keys] == sorted_normal_dict_keys_list
        assert [*reversed(self.sorted_dict_keys)] == sorted_normal_dict_keys_list[::-1]
        assert [*self.sorted_dict_values] == sorted_normal_dict_values_list
        assert [*reversed(self.sorted_dict_values)] == sorted_normal_dict_values_list[::-1]

        assert self.sorted_dict.key_type is self.key_type

    ###########################################################################
    # `contains` for the sorted dictionary and its keys.
    ###########################################################################

    @precondition(prec_key_type_not_set)
    @rule(instance=rule_sorted_dict_or_sorted_dict_keys(), key=all_keys)
    def contains_key_type_not_set(self, instance, key):
        with pytest.raises(RuntimeError, match="key type not set: insert at least one item first"):
            _ = key in instance

    @precondition(prec_key_type_set)
    @rule(instance=rule_sorted_dict_or_sorted_dict_keys(), key=rule_key_wrong_type())
    def contains_wrong_type(self, instance, key):
        with pytest.raises(
            TypeError, match=re.escape(f"got key {key!r} of type {type(key)}, want key of type {self.key_type}")
        ):
            _ = key in instance

    @precondition(prec_key_type_admits_nan)
    @rule(instance=rule_sorted_dict_or_sorted_dict_keys(), key=rule_key_is_nan())
    def contains_nan(self, instance, key):
        with pytest.raises(ValueError, match=re.escape(f"got bad key {key!r} of type {type(key)}")):
            _ = key in instance

    @precondition(prec_key_type_set)
    @rule(instance=rule_sorted_dict_or_sorted_dict_keys(), key=rule_key_right_type())
    def contains_probably_false(self, instance, key):
        assert (key in instance) == (key in self.normal_dict)

    @precondition(prec_keys_not_empty)
    @rule(instance=rule_sorted_dict_or_sorted_dict_keys(), key=rule_key_exists())
    def contains_true(self, instance, key):
        assert key in instance

    ###########################################################################
    # `contains` the sorted dictionary items.
    ###########################################################################

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

    @precondition(prec_keys_not_empty)
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

    @precondition(prec_keys_not_empty)
    @rule(key=rule_key_exists(), value=st.integers())
    def setitem_existing(self, key, value):
        self.normal_dict[key] = value
        self.sorted_dict[key] = value

    ###########################################################################
    # `delitem`.
    ###########################################################################

    @precondition(prec_key_type_not_set)
    @rule(key=all_keys)
    def delitem_key_type_not_set(self, key):
        with pytest.raises(RuntimeError, match="key type not set: insert at least one item first"):
            del self.sorted_dict[key]

    @precondition(prec_key_type_set)
    @rule(key=rule_key_wrong_type())
    def delitem_wrong_type(self, key):
        with pytest.raises(
            TypeError, match=re.escape(f"got key {key!r} of type {type(key)}, want key of type {self.key_type}")
        ):
            del self.sorted_dict[key]

    @precondition(prec_key_type_admits_nan)
    @rule(key=rule_key_is_nan())
    def delitem_nan(self, key):
        with pytest.raises(ValueError, match=re.escape(f"got bad key {key!r} of type {type(key)}")):
            del self.sorted_dict[key]

    @precondition(prec_key_type_set)
    @rule(key=rule_key_right_type())
    def delitem_probably_key_error(self, key):
        if key not in self.normal_dict:
            with pytest.raises(KeyError, match=re.escape(f"{key!r}")):
                del self.sorted_dict[key]
        else:
            self.keys.remove(key)
            del self.normal_dict[key]
            del self.sorted_dict[key]

    @precondition(prec_keys_not_empty)
    @rule(key=rule_key_exists())
    def delitem(self, key):
        self.keys.remove(key)
        del self.normal_dict[key]
        del self.sorted_dict[key]

    ###########################################################################
    # `clear`.
    ###########################################################################

    @rule()
    def clear(self):
        self.keys.clear()
        self.normal_dict.clear()
        self.sorted_dict.clear()

    ###########################################################################
    # `copy`.
    ###########################################################################

    @rule()
    def copy(self):
        self.sorted_dict = self.sorted_dict.copy()
        self.sorted_dict_items = self.sorted_dict.items()
        self.sorted_dict_keys = self.sorted_dict.keys()
        self.sorted_dict_values = self.sorted_dict.values()

    ###########################################################################
    # `get`.
    ###########################################################################

    @rule()
    def get_wrong_call(self):
        with pytest.raises(TypeError, match=re.escape("get() takes at most 2 arguments (3 given)")):
            self.sorted_dict.get(object, object, object)

    @precondition(prec_key_type_not_set)
    @rule(key=all_keys)
    def get_key_type_not_set(self, key):
        with pytest.raises(RuntimeError, match="key type not set: insert at least one item first"):
            self.sorted_dict.get(key)

    @precondition(prec_key_type_set)
    @rule(key=rule_key_wrong_type())
    def get_wrong_type(self, key):
        with pytest.raises(
            TypeError, match=re.escape(f"got key {key!r} of type {type(key)}, want key of type {self.key_type}")
        ):
            self.sorted_dict.get(key)

    @precondition(prec_key_type_admits_nan)
    @rule(key=rule_key_is_nan())
    def get_nan(self, key):
        with pytest.raises(ValueError, match=re.escape(f"got bad key {key!r} of type {type(key)}")):
            self.sorted_dict.get(key)

    @precondition(prec_key_type_set)
    @rule(key=rule_key_right_type(), value=st.integers())
    def get_probably_something(self, key, value):
        assert self.sorted_dict.get(key) == self.normal_dict.get(key)
        assert self.sorted_dict.get(key, value) == self.normal_dict.get(key, value)

    @precondition(prec_keys_not_empty)
    @rule(key=rule_key_exists(), value=st.integers())
    def get_existing(self, key, value):
        assert self.sorted_dict.get(key) == self.normal_dict.get(key)
        assert self.sorted_dict.get(key, value) == self.normal_dict.get(key, value)

    ###########################################################################
    # `setdefault`.
    ###########################################################################

    @rule()
    def setdefault_wrong_call(self):
        with pytest.raises(TypeError, match=re.escape("setdefault() takes at most 2 arguments (3 given)")):
            self.sorted_dict.setdefault(object, object, object)

    @precondition(prec_key_type_not_set)
    @rule(key=all_keys)
    def setdefault_key_type_not_set(self, key):
        with pytest.raises(RuntimeError, match="key type not set: insert at least one item first"):
            self.sorted_dict.setdefault(key)

    @precondition(prec_key_type_set)
    @rule(key=rule_key_wrong_type())
    def setdefault_wrong_type(self, key):
        with pytest.raises(
            TypeError, match=re.escape(f"got key {key!r} of type {type(key)}, want key of type {self.key_type}")
        ):
            self.sorted_dict.setdefault(key)

    @precondition(prec_key_type_admits_nan)
    @rule(key=rule_key_is_nan())
    def setdefault_nan(self, key):
        with pytest.raises(ValueError, match=re.escape(f"got bad key {key!r} of type {type(key)}")):
            self.sorted_dict.setdefault(key)

    @precondition(prec_key_type_set)
    @rule(key=rule_key_right_type())
    def setdefault_probably_new(self, key):
        if key not in self.normal_dict:
            self.keys.append(key)
        assert self.sorted_dict.setdefault(key) == self.normal_dict.setdefault(key)

    @precondition(prec_key_type_set)
    @rule(key=rule_key_right_type(), value=st.integers())
    def setdefault_probably_new_default(self, key, value):
        if key not in self.normal_dict:
            self.keys.append(key)
        assert self.sorted_dict.setdefault(key, value) == self.normal_dict.setdefault(key, value)

    @precondition(prec_keys_not_empty)
    @rule(key=rule_key_exists())
    def sedefault_existing(self, key):
        assert self.sorted_dict.setdefault(key) == self.normal_dict.setdefault(key)

    ###########################################################################
    # `init`.
    ###########################################################################

    @rule()
    def init(self):
        self.reinitialise()


TestSortedDictionary = SortedDictChecker.TestCase
