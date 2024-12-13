import builtins
import platform
import random
import sys

import pytest

from pysorteddict import SortedDict

# Reference counting is specific to CPython, so record this for later.
cpython = platform.python_implementation() == "CPython"


class Resources:
    """
    Store resources used to generate similar test cases for different key
    types.
    """

    def __init__(self, key_type: type):
        self.key_type = key_type
        self.key_subtype = type("sub" + self.key_type.__name__, (self.key_type,), {})

        self.rg = random.Random(__name__)
        self.keys = [self.gen() for _ in range(1000)]
        self.values = [self.gen() for _ in self.keys]
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
        self.keys_refcounts = [5] * len(self.keys)
        self.values = [*self.normal_dict.values()]
        self.values_refcounts = [5] * len(self.values)

    def gen(self, *, small: bool = True) -> int:
        """
        Generate a key or value for a dictionary. It will be a new object (i.e.
        not an interned one).

        :param large: Whether to generate a small or large key/value. (A small
        one will never be equal to a large one.)

        :return: Random result.
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
    if cpython:
        for observed, expected in zip(
            map(sys.getrefcount, resources.normal_dict), resources.keys_refcounts, strict=False
        ):
            assert observed == expected
        for observed, expected in zip(
            map(sys.getrefcount, resources.normal_dict.values()), resources.values_refcounts, strict=False
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
    key = resources.gen(small=False)
    assert key not in sorted_dict

    if cpython:
        assert sys.getrefcount(key) == 2


def test_contains_yes(resources, sorted_dict):
    key = resources.rg.choice(resources.keys)
    assert key in sorted_dict

    if cpython:
        assert sys.getrefcount(key) == 6


def test_getitem_wrong_type(resources, sorted_dict):
    with pytest.raises(TypeError) as ctx:
        sorted_dict[resources.key_subtype()]
    assert ctx.value.args[0] == f"key must be of type {resources.key_type!r}"


def test_getitem_missing(resources, sorted_dict):
    key = resources.gen(small=False)
    with pytest.raises(KeyError) as ctx:
        sorted_dict[key]
    assert ctx.value.args[0] == key

    if cpython:
        assert sys.getrefcount(key) == 3


def test_getitem_found(resources, sorted_dict):
    key = resources.rg.choice(resources.keys)
    value = sorted_dict[key]
    assert value == resources.normal_dict[key]

    if cpython:
        assert sys.getrefcount(key) == 6
        assert sys.getrefcount(value) == 6


def test_delitem_wrong_type(resources, sorted_dict):
    with pytest.raises(TypeError) as ctx:
        del sorted_dict[resources.key_subtype()]
    assert ctx.value.args[0] == f"key must be of type {resources.key_type!r}"


def test_delitem_missing(resources, sorted_dict):
    key = resources.gen(small=False)
    with pytest.raises(KeyError) as ctx:
        del sorted_dict[key]
    assert ctx.value.args[0] == key

    if cpython:
        assert sys.getrefcount(key) == 3


def test_delitem_found(resources, sorted_dict):
    idx, key = resources.rg.choice([*enumerate(resources.keys)])
    del resources.normal_dict[key]
    del sorted_dict[key]

    if cpython:
        resources.keys_refcounts[idx] -= 2
        resources.values_refcounts[idx] -= 2


def test_setitem_wrong_type(resources, sorted_dict):
    value = resources.gen()
    with pytest.raises(TypeError) as ctx:
        sorted_dict[resources.key_subtype()] = value
    assert ctx.value.args[0] == f"key must be of type {resources.key_type!r}"

    if cpython:
        assert sys.getrefcount(value) == 2


def test_setitem_insert(resources, sorted_dict):
    key, value = resources.gen(small=False), resources.gen()
    resources.normal_dict[key] = value
    sorted_dict[key] = value
    assert sorted_dict[key] == value

    if cpython:
        assert sys.getrefcount(key) == 4
        assert sys.getrefcount(value) == 4


def test_setitem_overwrite(resources, sorted_dict):
    idx, key = resources.rg.choice([*enumerate(resources.keys)])
    value = resources.gen()
    resources.normal_dict[key] = value
    sorted_dict[key] = value
    assert sorted_dict[key] == value

    if cpython:
        assert sys.getrefcount(value) == 4
        resources.values_refcounts[idx] -= 2
