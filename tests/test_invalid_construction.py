import pytest

from pysorteddict import SortedDict

missing_argument = "function missing required argument 'key_type' (pos 1)"
too_many_arguments = "function takes at most 1 argument (2 given)"
wrong_argument = "constructor argument must be a supported type"


def test_no_arguments():
    with pytest.raises(TypeError) as ctx:
        SortedDict()
    assert ctx.value.args[0] == missing_argument


def test_superfluous_arguments():
    with pytest.raises(TypeError) as ctx:
        SortedDict(object, object)
    assert ctx.value.args[0] == too_many_arguments


@pytest.mark.parametrize("key_type", [object, object(), 63, 5.31, "valiant effort", b"salt", ["hear", 0x5EE]])
def test_wrong_type(key_type):
    with pytest.raises(TypeError) as ctx:
        SortedDict(key_type)
    assert ctx.value.args[0] == wrong_argument
