import re

import pytest

from pysorteddict import SortedDict


@pytest.fixture
def sorted_dict():
    return SortedDict()


def test_key_repr_error(sorted_dict):
    sorted_dict[2**15000] = 0
    with pytest.raises(
        ValueError,
        match=re.escape(
            "Exceeds the limit (4300 digits) for integer string conversion; use sys.set_int_max_str_digits() to increase the limit"
        ),
    ):
        str(sorted_dict)


def test_value_repr_error(sorted_dict):
    sorted_dict[0] = 2**15000
    with pytest.raises(
        ValueError,
        match=re.escape(
            "Exceeds the limit (4300 digits) for integer string conversion; use sys.set_int_max_str_digits() to increase the limit"
        ),
    ):
        str(sorted_dict)
