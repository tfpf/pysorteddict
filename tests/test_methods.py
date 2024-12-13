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
        self.keys = [self.generate_key() for _ in range(1000)]
        self.values = [self.generate_key() for _ in self.keys]
        self.normal_dict = dict(zip(self.keys, self.values, strict=True))

        sorted_dict = SortedDict(self.key_type)
        for key, value in zip(self.keys, self.values, strict=True):
            sorted_dict[key] = value
        self.sorted_dicts = [sorted_dict, sorted_dict.copy()]

        # Store the reference count of an item in a list at the position at
        # which it appears in the normal dictionary. The reference counts are
        # all 4, but querying the reference count increases it, so I store 5.
        # Whenever a test changes the reference count of any item, I set the
        # new reference count at its index.
        self.keys = [*self.normal_dict]
        self.values = [*self.normal_dict.values()]
        if self.cpython:
            # Reference counting is specific to CPython.
            self.keys_refcounts = [5] * len(self.keys)
            self.values_refcounts = [5] * len(self.values)

    def generate_key(self, *, small: bool = True) -> int:
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
    """
    Create test resources for the given key type (passed as a parameter to this
    fixture).
    """
    resources = Resources(request.param)
    yield resources

    # Tearing down: verify the reference counts.
    if resources.cpython:
        for observed, expected in zip(
            map(sys.getrefcount, resources.normal_dict), resources.keys_refcounts, strict=True
        ):
            assert observed == expected
        for observed, expected in zip(
            map(sys.getrefcount, resources.normal_dict.values()), resources.values_refcounts, strict=True
        ):
            assert observed == expected


@pytest.fixture
def sorted_dict(request, resources):
    """
    Obtain either the sorted dictionary or its copy based on the index (passed
    as a parameter to this fixture). The aim is to test both it and its copy
    with the same rigour.
    """
    sorted_dict = resources.sorted_dicts[request.param]
    yield sorted_dict

    # Tearing down: verify some non-mutating methods.
    assert len(sorted_dict) == len(resources.normal_dict)
    assert str(sorted_dict) == str(dict(sorted(resources.normal_dict.items())))


# Run each test with each key type, and on the sorted dictionary and its copy.
pytestmark = [
    pytest.mark.parametrize("resources", [int], indirect=True),
    pytest.mark.parametrize("sorted_dict", [0, 1], ids=["original", "copy"], indirect=True),
]


def test_contains_wrong_type(resources, sorted_dict):
    assert resources.key_subtype() not in sorted_dict


def test_contains_no(resources, sorted_dict):
    key = resources.generate_key(small=False)
    assert key not in sorted_dict

    if resources.cpython:
        assert sys.getrefcount(key) == 2


def test_contains_yes(resources, sorted_dict):
    key = resources.rg.choice(resources.keys)
    assert key in sorted_dict

    if resources.cpython:
        assert sys.getrefcount(key) == 6
