# Documentation

<details style="background-color: #E7FFE6; padding: 10px">

<summary>Looking for the documentation of an older version?</summary>

▸ [0.4.6](https://github.com/tfpf/pysorteddict/blob/v0.4.6/docs/documentation.md)
▸ [0.4.5](https://github.com/tfpf/pysorteddict/blob/v0.4.5/docs/index.md)
▸ [0.4.4](https://github.com/tfpf/pysorteddict/blob/v0.4.4/docs/index.md)

</details>

`SortedDict` may be imported explicitly:

```python
from pysorteddict import SortedDict
```

or implicitly using the wildcard (though this is not recommended).

```python
from pysorteddict import *
```

## Constructor

### `SortedDict(*args, **kwargs) -> SortedDict`

Create an empty sorted dictionary. `args` and `kwargs` are ignored.

#### Exceptions

If `decimal.Decimal` cannot be imported (which might be a symptom of a corrupt or damaged Python installation), raise
`ImportError` or a subclass thereof, or `AttributeError`.

```python
from pysorteddict import *
d = SortedDict()
```

```text
Traceback (most recent call last):
  File "…", line 2, in <module>
    d = SortedDict()
ModuleNotFoundError: No module named 'decimal'
```

```text
Traceback (most recent call last):
  File "…", line 2, in <module>
    d = SortedDict()
AttributeError: module 'decimal' has no attribute 'Decimal'
```

## Properties

### `d.key_type: type | None`

Return the key type of the sorted dictionary `d`, or `None` if no key-value pairs have been inserted in it.

```python
from pysorteddict import *
d = SortedDict()
assert d.key_type is None
d[b"foo"] = ()
assert d.key_type is bytes
```

## Magic Methods

### `repr(d)`

Return a human-readable representation of the sorted dictionary `d`.

### `key in d`

Return whether `key` is present in the sorted dictionary `d`.

### `len(d)`

Return the number of key-value pairs in the sorted dictionary `d`.

### `d[key]`

Return the value mapped to `key` in the sorted dictionary `d`.

#### Exceptions

If no key-value pairs have been inserted into `d` yet, raise `RuntimeError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"]
```

```text
Traceback (most recent call last):
  File "…", line 3, in <module>
    d["foo"]
    ~^^^^^^^
RuntimeError: key type not set: insert at least one item first
```

Otherwise, if `type(key)` does not match the type of the first key inserted into `d`, raise `TypeError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ("bar", "baz")
d[100]
```

```text
Traceback (most recent call last):
  File "…", line 4, in <module>
    d[100]
    ~^^^^^
TypeError: got key 100 of type <class 'int'>, want key of type <class 'str'>
```

Otherwise, if `key` is not comparable with instances of its type, raise `ValueError`.

```python
from pysorteddict import *
d = SortedDict()
d[1.1] = ("racecar",)
d[float("nan")]
```

```text
Traceback (most recent call last):
  File "…", line 4, in <module>
    d[float("nan")]
    ~^^^^^^^^^^^^^^
ValueError: got bad key nan of type <class 'float'>
```

Otherwise, if `key` is not present in `d`, raise `KeyError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ("bar", "baz")
d["spam"]
```

```text
Traceback (most recent call last):
  File "…", line 4, in <module>
    d["spam"]
    ~^^^^^^^^
KeyError: 'spam'
```

### `d[key] = value`

Map `value` to `key` in the sorted dictionary `d`, replacing the previously-mapped value (if any).

#### Exceptions

If no key-value pairs have been inserted into `d` yet and `type(key)` isn't one of the supported types (`bytes`,
`float`, `int`, `str` and `decimal.Decimal`), raise `TypeError`.

```python
from pysorteddict import *
d = SortedDict()
d[["eggs"]] = None
```

```text
Traceback (most recent call last):
  File "…", line 3, in <module>
    d[["eggs"]] = None
    ~^^^^^^^^^^
TypeError: got key ['eggs'] of unsupported type <class 'list'>
```

Otherwise, if `type(key)` does not match the type of the first key inserted into `d`, raise `TypeError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ("bar", "baz")
d[100] = "spam"
```

```text
Traceback (most recent call last):
  File "…", line 4, in <module>
    d[100] = "spam"
    ~^^^^^
TypeError: got key 100 of type <class 'int'>, want key of type <class 'str'>
```

Otherwise, if `key` is not comparable with instances of its type, raise `ValueError`.

```python
from pysorteddict import *
d = SortedDict()
d[1.1] = ("racecar",)
d[float("nan")] = {}
```

```text
Traceback (most recent call last):
  File "…", line 4, in <module>
    d[float("nan")] = {}
    ~^^^^^^^^^^^^^^
ValueError: got bad key nan of type <class 'float'>
```

### `del d[key]`

Remove `key` and the value mapped to it from the sorted dictionary `d`.

#### Exceptions

If no key-value pairs have been inserted into `d` yet, raise `RuntimeError`.

```python
from pysorteddict import *
d = SortedDict()
del d["foo"]
```

```text
Traceback (most recent call last):
  File "…", line 3, in <module>
    del d["foo"]
        ~^^^^^^^
RuntimeError: key type not set: insert at least one item first
```

Otherwise, if `type(key)` does not match the type of the first key inserted into `d`, raise `TypeError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ("bar", "baz")
del d[100]
```

```text
Traceback (most recent call last):
  File "…", line 4, in <module>
    del d[100]
        ~^^^^^
TypeError: got key 100 of type <class 'int'>, want key of type <class 'str'>
```

Otherwise, if `key` is not comparable with instances of its type, raise `ValueError`.

```python
from pysorteddict import *
d = SortedDict()
d[1.1] = ("racecar",)
del d[float("nan")]
```

```text
Traceback (most recent call last):
  File "…", line 4, in <module>
    del d[float("nan")]
        ~^^^^^^^^^^^^^^
ValueError: got bad key nan of type <class 'float'>
```

Otherwise, if `key` is not present in `d`, raise `KeyError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ("bar", "baz")
del d["spam"]
```

```text
Traceback (most recent call last):
  File "…", line 4, in <module>
    del d["spam"]
        ~^^^^^^^^
KeyError: 'spam'
```

## Other Methods

### `d.clear()`

Remove all key-value pairs in the sorted dictionary `d`.

### `d.copy() -> SortedDict`

Return a shallow copy of the sorted dictionary `d`.

### `d.items() -> list[tuple[object, object]]`

Return the key-value pairs in the sorted dictionary `d`. The list will be sorted. It will exist independently of `d`;
it won't be a view on its items.

### `d.keys() -> list[object]`

Return the keys in the sorted dictionary `d`. The list will be sorted. It will exist independently of ``d``; it won't
be a view on its keys.

### `d.values() -> list[object]`

Return the values in the sorted dictionary `d`. The list will be sorted by the keys the values are mapped to. It will
exist independently of `d`; it won't be a view on its values.

