import builtins
import platform
import random
import sys

import pytest

from pysorteddict import SortedDict


class Resources:
    """
    Store resources used to generate similar test cases for different key
    types.
    """

    cpython = platform.python_implementation() == "CPython"

    def __init__(self, key_type: type):
        self.key_type = key_type
        self.key_subtype = type("sub" + self.key_type.__name__, (self.key_type,), {})

        self.rg = random.Random(__name__)
        keys = [self.generate_key() for _ in range(1000)]
        values = [self.generate_key() for _ in keys]
        self.normal_dict = dict(zip(keys, values, strict=True))

        self.sorted_dict = SortedDict(self.key_type)
        for key, value in zip(keys, values, strict=True):
            self.sorted_dict[key] = value

        self.sorted_dict_copy = self.sorted_dict.copy()

        # Store the reference count of an item in a list at the position at
        # which it appears in the normal dictionary. At this point, the
        # reference counts are all 4, and will drop to 3 once all local
        # variables are destroyed. However, querying the reference count
        # increases it, so I store 4. Whenever a test changes the reference
        # count of any item, I set the new reference count at its index.
        # Remember that reference counting is specific to the CPython
        # implementation.
        if self.cpython:
            self.keys_refcounts = [4] * len(self.normal_dict)
            self.values_refcounts = [4] * len(self.normal_dict)

    def generate_key(self, *, small: bool = True) -> object:
        """
        Generate a key for a dictionary. It will be a new object (i.e. not an
        interned one).

        :param large: Whether to generate a small or large key. (A small key
        will never be equal to a large key.)

        :return: Key.
        """
        match self.key_type:
            # The pattern must be non-capturing (otherwise, it raises a syntax
            # error because the remaining patterns become unreachable). Hence,
            # whenever the pattern is an existing name which can be shadowed,
            # it has to be written like this.
            case builtins.int:
                if small:
                    return self.rg.randrange(1000, 2000)
                return self.rg.randrange(2000, 3000)

            case _:
                raise RuntimeError


@pytest.fixture
def resources(request):
    resources = Resources(request.param)
    yield resources

    if resources.cpython:
        for expected, observed in zip(
            resources.keys_refcounts, map(sys.getrefcount, resources.normal_dict.keys()), strict=True
        ):
            assert expected == observed
        for expected, observed in zip(
            resources.values_refcounts, map(sys.getrefcount, resources.normal_dict.values()), strict=True
        ):
            assert expected == observed


# Run each test with each key type. Currently, there is only one key type.
pytestmark = pytest.mark.parametrize("resources", [int], indirect=True)


def test_contains_wrong_type(resources):
    assert resources.key_subtype() not in resources.sorted_dict


def test_contains_no(resources):
    key = resources.generate_key(small=False)
    assert key not in resources.sorted_dict

    if resources.cpython:
        assert 2 == sys.getrefcount(key)
