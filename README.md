# pysorteddict

Provides `SortedDict`, which is a Python sorted dictionary: a Python dictionary in which the keys are always in
ascending order.

[![Hatch project](https://img.shields.io/badge/%F0%9F%A5%9A-Hatch-4051b5.svg)](https://github.com/pypa/hatch)

[![style](https://github.com/tfpf/pysorteddict/actions/workflows/style.yml/badge.svg)](https://github.com/tfpf/pysorteddict/actions/workflows/style.yml)
[![package](https://github.com/tfpf/pysorteddict/actions/workflows/package.yml/badge.svg)](https://github.com/tfpf/pysorteddict/actions/workflows/package.yml)

<p align="center">
 <img src="https://github.com/user-attachments/assets/e9d1e78e-c0fd-4d87-93f6-e293ddef31ba" />
</p>

## Installation

[pysorteddict is available on PyPI.](https://pypi.org/project/pysorteddict/) It requires Python 3.10 or newer. Built
distributions (binary wheels) are provided for Linux, macOS and Windows, so installing is straightforward.

```shell
pip install pysorteddict
```

If you are on any other platform, install the Python development headers and libraries before running the above
command.

## Usage

All keys in a sorted dictionary must be of the same type, which is determined when the first key-value pair is inserted
into it. The values, though, can be of any type.

```python
import json

from pysorteddict import SortedDict

sorted_dict = SortedDict()
sorted_dict["honestly"] = "weight"
sorted_dict["gain is"] = 31.692
sorted_dict["times"] = "easier than"
sorted_dict["losing"] = ["weight"]
print(json.dumps(sorted_dict, indent=2, sort_keys=False))
```

When run, this program will output the keys in ascending order.

```json
{
  "gain is": 31.692,
  "honestly": "weight",
  "losing": [
    "weight"
  ],
  "times": "easier than"
}
```

## Implementation Details

pysorteddict is implemented entirely in C++. `SortedDict` provides a Python interface to
`std::map<PyObject*, PyObject*, _>`.

## Documentation

### Constructor

#### `SortedDict(*args, **kwargs) -> SortedDict`

Create an empty sorted dictionary. `args` and `kwargs` are ignored.

### Magic Methods

#### `repr(d)`

Return a human-readable representation of the sorted dictionary `d`.

#### `key in d`

Return whether `key` is present in the sorted dictionary `d`.

#### `len(d)`

Return the number of key-value pairs in the sorted dictionary `d`.

#### `d[key]`

Return the value mapped to `key` in the sorted dictionary `d`.

* If no key-value pairs have been inserted into `d` yet, raise `ValueError`.
* If `type(key)` does not match the type of the first key inserted into `d`, raise `TypeError`.
* If `key` is not present in `d`, raise `KeyError`.

#### `d[key] = value`

Map `value` to `key` in the sorted dictionary `d`, replacing the previously-mapped value (if any).

* If no key-value pairs have been inserted into `d` yet and `type(key)` isn't one of the supported types (`bytes`,
  `int` and `str`), raise `TypeError`.
* If `type(key)` does not match the type of the first key inserted into `d`, raise `TypeError`.

#### `del d[key]`

Remove `key` and the value mapped to it from the sorted dictionary `d`.

* If no key-value pairs have been inserted into `d` yet, raise `ValueError`.
* If `type(key)` does not match the type of the first key inserted into `d`, raise `TypeError`.
* If `key` is not present in `d`, raise `KeyError`.

### Other Methods

#### `d.clear()`

Remove all key-value pairs in the sorted dictionary `d`.

#### `d.copy() -> SortedDict`

Return a shallow copy of the sorted dictionary `d`.

#### `d.items() -> list[tuple[object, object]]`

Return the key-value pairs in the sorted dictionary `d`. The list will be sorted. It will exist independently of `d`;
it won't be a view on its items.

#### `d.keys() -> list[object]`

Return the keys in the sorted dictionary `d`. The list will be sorted. It will exist independently of ``d``; it won't
be a view on its keys.

#### `d.values() -> list[object]`

Return the values in the sorted dictionary `d`. The list will be sorted by the keys the values are mapped to. It will
exist independently of `d`; it won't be a view on its values.
