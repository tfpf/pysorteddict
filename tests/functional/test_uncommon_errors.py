import pytest

from pysorteddict import SortedDict


def test_key_repr_error():
    sorted_dict = SortedDict()
    sorted_dict[2**15000] = 0
    with pytest.raises(ValueError, match="Exceeds the limit"):
        str(sorted_dict)


def test_value_repr_error():
    sorted_dict = SortedDict()
    sorted_dict[0] = 2**15000
    with pytest.raises(ValueError, match="Exceeds the limit"):
        str(sorted_dict)


def test_reverse_iterator_yield_after_removing_elements():
    sorted_dict = SortedDict()
    sorted_dict[0] = 0
    sorted_dict[1] = 1
    r = reversed(sorted_dict)
    assert next(r) == 1
    del sorted_dict[0]
    with pytest.raises(StopIteration):
        next(r)
