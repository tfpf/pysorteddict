# Development

## Repository Overview

[Explore the repository on GitHub.](https://github.com/tfpf/pysorteddict)

Related components are grouped into directories.

### `docs/`

Code for the static website you are viewing right now.

### `src/pysorteddict/`

Core logic.

#### `sorted_dict_type.cc`

Implementation of the Python `SortedDict` type.

#### `sorted_dict_view_type.cc`

Implementation of views over `SortedDict` objects—superclasses of `SortedDictItems`, `SortedDictItemsFwdIter`,
`SortedDictItemsRevIter`, `SortedDictKeys`,  `SortedDictKeysFwdIter`, `SortedDictKeysRevIter`, `SortedDictValues`,
`SortedDictValuesFwdIter` and `SortedDictValuesRevIter`.

#### `sorted_dict_items_type.cc`

Implementation of the Python `SortedDictItems`, `SortedDictItemsFwdIter` and `SortedDictItemsRevIter` types. Exposed to
users only indirectly via `SortedDict.items`.

#### `sorted_dict_keys_type.cc`

Implementation of the Python `SortedDictKeys`, `SortedDictKeysFwdIter` and `SortedDictKeysRevIter` types. Exposed to
users only indirectly via `SortedDict.keys`.

#### `sorted_dict_values_type.cc`

Implementation of the Python `SortedDictValues`, `SortedDictValuesFwdIter` and `SortedDictValuesRevIter` types. Exposed
to users only indirectly via `SortedDict.values`.

#### `sorted_dict_module.cc`

Implementation of the Python `pysorteddict` module. Glue between Python methods and C++ methods.

#### `sorted_dict_utils.hh`

Miscellanies for ease of development.

### `tests/functional/`

Functional (behavioural) tests.

#### `test_fuzz.py`

Randomised tests which cover most scenarios.

#### `test_keys_iter.py`

Tests for `SortedDictKeysIter`.

#### `test_uncommon_errors.py`

Tests for errors not expected to occur during regular usage.

### `tests/performance/`

Performance tests.

#### `performance.ipynb`

Tests to benchmark the performance of some methods.

## Install pysorteddict for Develpoment

First, ensure that a C++20 compiler and the Python development headers and libraries are available.

### Linux and macOS

```shell
python -m venv pysorteddict-venv
. pysorteddict-venv/bin/activate
pip install meson-python ninja
pip install --editable . --no-build-isolation
```

### Windows with Command Prompt

```bat
python -m venv pysorteddict-venv
pysorteddict-venv\Scripts\activate.bat
pip install meson-python ninja
pip install --editable . --no-build-isolation
```

### Windows with MSYS2 UCRT64

```shell
pacman -S mingw-w64-ucrt-x86_64-meson-python
python -m venv --system-site-packages pysorteddict-venv
. pysorteddict-venv/bin/activate
pip install --editable . --no-build-isolation
```
