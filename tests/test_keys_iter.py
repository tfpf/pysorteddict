import pytest
import collections
import re
import random
import itertools

from pysorteddict import SortedDict


@pytest.fixture
def sorted_dict(request):
    d = SortedDict()
    for i in range(request.param):
        d[i] = i
    return d

@pytest.mark.parametrize("sorted_dict", [0, 1, 10, 100, 1_000, 10_000, 100_000], indirect=True)
@pytest.mark.parametrize("iterators", range(1, 10))
@pytest.mark.parametrize("advance", [0, 1, 2, 4, 8, 16])
def test_setitem_when_iterators_active(sorted_dict, iterators, advance):
    sorted_dict_len = len(sorted_dict)
    sorted_dict_keys = sorted_dict.keys()
    sorted_dict_keys_iters = [iter(sorted_dict_keys) for _ in range(iterators)]
    advances = [random.randint(0, min(advance, sorted_dict_len)) for _ in sorted_dict_keys_iters]
    for sorted_dict_keys_iter, advance in zip(sorted_dict_keys_iters, advances):
        [*itertools.islice(sorted_dict_keys_iter, advance)]
    keys = [*sorted_dict_keys]
    counter = collections.Counter(advances)
    for advance in advances:
        if advance < sorted_dict_len:
            with pytest.raises(RuntimeError, match=re.escape(f"operation not permitted: key-value pair locked by {counter[advance]} iterator(s)")):
                del sorted_dict[keys[advance]]
    if count := (iterators - counter[sorted_dict_len]):
        with pytest.raises(RuntimeError, match=re.escape(f"operation not permitted: sorted dictionary locked by {count} iterator(s)")):
            sorted_dict.clear()


if __name__ == "__main__":
    pytest.main()
