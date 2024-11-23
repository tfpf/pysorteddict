# pysorteddict

Like Python's `dict`, but the keys are always in ascending order.

[![Hatch project](https://img.shields.io/badge/%F0%9F%A5%9A-Hatch-4051b5.svg)](https://github.com/pypa/hatch)

[![style](https://github.com/tfpf/pysorteddict/actions/workflows/style.yml/badge.svg)](https://github.com/tfpf/pysorteddict/actions/workflows/style.yml)
[![test](https://github.com/tfpf/pysorteddict/actions/workflows/test.yml/badge.svg)](https://github.com/tfpf/pysorteddict/actions/workflows/test.yml)
[![package](https://github.com/tfpf/pysorteddict/actions/workflows/package.yml/badge.svg)](https://github.com/tfpf/pysorteddict/actions/workflows/package.yml)

<p align="center">
 <img src="https://github.com/user-attachments/assets/e9d1e78e-c0fd-4d87-93f6-e293ddef31ba" />
</p>

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
sorted_dict[24274] = "times easier than"
sorted_dict[9765] = ["losing", "weight"]
print(sorted_dict)
```

This program should output
`{1992: 31.692, 5659: 'gaining weight is', 9765: ['losing', 'weight'], 24274: 'times harder than'}`. Note that the keys
are in ascending order.

## Documentation

### `class pysorteddict.SortedDict(key_type)`

Constructor. Create a new sorted dictionary in which the keys are of type `key_type`. As of the current version,
`key_type` must be `int`. Support for some more types will be added in due course.

#### `len(d)`

Return the number of key-value pairs in a sorted dictionary `d`.

#### `d[key]`

Return the value mapped to `key` in a sorted dictionary `d`.

* If `type(key)` does not match `key_type` passed to the constructor, raise `TypeError`.
* If `key` is not present in `d`, raise `KeyError`.

#### `d[key] = value`

Map `value` to `key` in a sorted dictionary `d`, overwriting the previously-mapped value (if any).

* If `type(key)` does not match `key_type` passed to the constructor, raise `TypeError`.

#### `del d[key]`

Remove `key` and the value mapped to it from a sorted dictionary `d`.

* If `key` is not present in `d`, raise `KeyError`.

#### `str(d)`

Return a human-readable representation of a sorted dictionary `d`.

#### `d.items()`

Return the list of key-value pairs in a sorted dictionary `d`. This list will be sorted.

#### `d.keys()`

Return the list of keys in a sorted dictionary `d`. This list will be sorted.

#### `d.values()`

Return the list of values in a sorted dictionary `d`. The order of the values will be such that the keys they are
mapped to will be in ascending order.
