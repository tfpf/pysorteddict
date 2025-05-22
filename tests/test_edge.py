import pytest

from pysorteddict import SortedDict


@pytest.fixture
def sorted_dict():
    d = SortedDict()
    d[0] = 0
    return d

def test_modify_when_iterator_active(sorted_dict):
    sorted_dict_keys_iter = iter(sorted_dict.keys())
    del sorted_dict[0]
    [*sorted_dict_keys_iter]

if __name__ == "__main__":
    pytest.main()
