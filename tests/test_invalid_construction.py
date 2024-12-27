import pytest

from pysorteddict import SortedDict


@pytest.mark.skip
def test_no_arguments():
    with pytest.raises(TypeError) as ctx:
        SortedDict()
    assert ctx.value.args[0] == "function missing required argument 'key_type' (pos 1)"


@pytest.mark.skip
def test_superfluous_arguments():
    with pytest.raises(TypeError) as ctx:
        SortedDict(object, object)
    assert ctx.value.args[0] == "function takes at most 1 argument (2 given)"


@pytest.mark.skip
@pytest.mark.parametrize("key_type", [object, object(), 63, 5.31, "effort", b"salt", ["hear", 0x5EE], (1.61, "taste")])
def test_wrong_type(key_type):
    with pytest.raises(TypeError) as ctx:
        SortedDict(key_type)
    assert ctx.value.args[0] == "constructor argument must be a supported type"
