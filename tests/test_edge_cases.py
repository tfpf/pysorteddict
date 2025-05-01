import pytest

from pysorteddict import SortedDict


def test_bad_keys():
    bad_keys = [float("nan")]
    for bad_key in bad_keys:
        sorted_dict = SortedDict()
        with pytest.raises(ValueError, match="key type not set: insert at least one item first"):
            sorted_dict[bad_key]
        with pytest.raises(ValueError, match=f"bad key: {bad_key}"):
            sorted_dict[bad_key] = None
        sorted_dict[0] = 0  # To ensure that the key type didn't get set on unsuccessful insertion.
        sorted_dict = SortedDict()
        sorted_dict[type(bad_key)()] = None
        with pytest.raises(ValueError, match=f"bad key: {bad_key}"):
            sorted_dict[bad_key] = None


if __name__ == "__main__":
    pytest.main()
