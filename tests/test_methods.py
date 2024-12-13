import pytest

class Resources:
    def __init__(self, key_type: type):
        self.key_type = key_type
    def __str__(self):
        return hex(id(self)) + ":" + repr(self.key_type)

@pytest.fixture
def resources(request):
    return Resources(request.param)

@pytest.mark.parametrize("resources", [int, float], indirect=True)
def test_dummy(resources):
    print(resources)
