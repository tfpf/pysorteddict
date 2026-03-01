import sys

import pytest

from pysorteddict import SortedDict


def test_key_repr_error():
    sorted_dict = SortedDict()
    sorted_dict[10 ** sys.get_int_max_str_digits()] = 0
    with pytest.raises(ValueError, match="Exceeds the limit"):
        str(sorted_dict)


def test_value_repr_error():
    sorted_dict = SortedDict()
    sorted_dict[0] = 10 ** sys.get_int_max_str_digits()
    with pytest.raises(ValueError, match="Exceeds the limit"):
        str(sorted_dict)


def test_remove_first_element_while_referenced_by_reverse_iterator():
    sorted_dict = SortedDict()
    sorted_dict[0] = 0
    sorted_dict[1] = 1
    r = reversed(sorted_dict)
    assert next(r) == 1
    del sorted_dict[0]
    with pytest.raises(StopIteration):
        next(r)
