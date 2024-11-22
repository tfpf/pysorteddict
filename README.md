# pysorteddict

Like `dict`, but the keys are always sorted in ascending order.

## Installation

[pysorteddict is available on PyPI.](https://pypi.org/project/pysorteddict/)

```shell
pip install pysorteddict
```

At the moment, only a source distribution is provided, so make sure to install the Python development headers and
libraries for your operating system. Otherwise, the above command may fail.

## Usage

All keys in a sorted dictionary must be of the same type, which must be passed to the constructor. The values, though,
can be of any type.

```python
from pysorteddict import SortedDict

sorted_dict = SortedDict(int)
sorted_dict[5659] = "gaining weight is"
sorted_dict[1992] = 31.692
sorted_dict[24274] = "times harder than"
sorted_dict[9765] = ["losing", "weight"]
print(sorted_dict)
```

This program should output
`{1992: 31.692, 5659: 'gaining weight is', 9765: ['losing', 'weight'], 24274: 'times harder than'}`. Note that the keys
are in ascending order.

## Documentation
