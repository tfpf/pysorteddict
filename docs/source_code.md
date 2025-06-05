# Source Code

[Explore the source code on GitHub.](https://github.com/tfpf/pysorteddict)

## `docs/`

Code for what you are reading right now.

## `src/pysorteddict/`

Core logic.

### `sorted_dict_type.cc`

Implementation of the Python `SortedDict` type.

### `sorted_dict_view_type.cc`

Implementation of views over `SortedDict` objects. Superclasses of `SortedDictKeys` and `SortedDictKeysIter`.

### `sorted_dict_keys_type.cc`

Implementation of the Python `SortedDictKeys` and `SortedDictKeysIter` types. Exposed to users only indirectly via
`SortedDict.keys`.

### `sorted_dict_module.cc`

Implementation of the Python `pysorteddict` module. Glue between Python methods and C++ methods.

### `sorted_dict_utils.hh`

Miscellanies for ease of development.

## `tests/`

Behavioural tests.

### `test_fuzz.py`

Randomised tests which cover most scenarios.

### `test_keys_iter.py`

Tests for `SortedDictKeysIter`.
