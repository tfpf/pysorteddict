# Development

## Repository Overview

[Explore the repository on GitHub.](https://github.com/tfpf/pysorteddict)

The organisation of the code is rather straightforward. Related components are grouped into directories.

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

## Developer's Setup

To begin with, ensure that a C++20 compiler and Python 3.10 (or newer) along with its development headers and libraries
are available.

[Hatch](https://hatch.pypa.io/latest/) is recommended to set up pysorteddict for development. Enter the command

```shell
hatch shell
```

to automatically enter a managed virtual environment and install the project in editable mode.

Alternatively, create a virtual environment and install the project manually.

:::{tab} Linux, macOS
```shell
python -m venv pysorteddict-venv
. pysorteddict-venv/bin/activate
pip install meson-python ninja
pip install --editable . --no-build-isolation
```
:::

:::{tab} Windows
```bat
py -m venv pysorteddict-venv
pysorteddict-venv\Scripts\activate
pip install meson-python ninja
pip install --editable . --no-build-isolation
```
:::

:::{tab} MSYS2 UCRT64
```shell
pacman -S mingw-w64-ucrt-x86_64-meson-python
python -m venv --system-site-packages pysorteddict-venv
. pysorteddict-venv/bin/activate
pip install --editable . --no-build-isolation
```
:::
