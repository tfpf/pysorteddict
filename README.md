# pysorteddict

Provides `SortedDict`, which is a Python sorted dictionary: a Python dictionary in which the keys are always in
ascending order.

[![Hatch project](https://img.shields.io/badge/%F0%9F%A5%9A-Hatch-4051b5.svg)](https://github.com/pypa/hatch)
[![ci](https://github.com/tfpf/pysorteddict/actions/workflows/ci.yml/badge.svg)](https://github.com/tfpf/pysorteddict/actions/workflows/ci.yml)

<p align="center">
 <img src="https://github.com/user-attachments/assets/e9d1e78e-c0fd-4d87-93f6-e293ddef31ba" />
</p>

## Documentation

[View the documentation on GitHub Pages.](https://tfpf.github.io/pysorteddict/)

## Installation

[View the project on PyPI.](https://pypi.org/project/pysorteddict/)

```shell
pip install pysorteddict
```

pysorteddict requires Python 3.10 or newer. Built distributions (binary wheels) are provided for most 64-bit Linux,
macOS and Windows hosts. If you are on any other platform, install the Python development headers and libraries before
running the above command.

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
assert sorted_dict.key_type is str
print(json.dumps(sorted_dict, indent=2, sort_keys=False))
```

The above Python script will output the keys in ascending order.

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

The following key types are supported.

* `bytes`
* `float`
* `int`
* `str`

## Implementation Details

pysorteddict is implemented entirely in C++. `SortedDict` provides a Python interface to
`std::map<PyObject*, PyObject*, _>`.
