# Work in progress

This page documents pysorteddict v0.4.4. To view the documentation of older versions, visit one of the links below.
* [v0.0.7](https://github.com/tfpf/pysorteddict/blob/v0.0.7/README.md)

### Constructor

#### `SortedDict(*args, **kwargs) -> SortedDict`

Create an empty sorted dictionary. `args` and `kwargs` are ignored.

### Properties

#### `d.key_type: type | None`

Return the key type of the sorted dictionary `d`, or `None` if no key-value pairs have been inserted in it.

### Magic Methods

#### `repr(d)`

Return a human-readable representation of the sorted dictionary `d`.

#### `key in d`

Return whether `key` is present in the sorted dictionary `d`.

#### `len(d)`

Return the number of key-value pairs in the sorted dictionary `d`.

#### `d[key]`

Return the value mapped to `key` in the sorted dictionary `d`.

If no key-value pairs have been inserted into `d` yet, raise `ValueError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"]
```

```
Traceback (most recent call last):
  File "…", line 3, in <module>
    d["foo"]
    ~^^^^^^^
ValueError: key type not set: insert at least one item first
```

Otherwise, if `type(key)` does not match the type of the first key inserted into `d`, raise `TypeError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ("bar", "baz")
d[0xC0FFEE]
```

```
Traceback (most recent call last):
  File "…", line 4, in <module>
    d[0xC0FFEE]
    ~^^^^^^^^^^
TypeError: wrong key type: want <class 'str'>, got <class 'int'>
```

Otherwise, if `key` is not comparable with instances of its type, raise `ValueError`.

```python
from pysorteddict import *
d = SortedDict()
d[1.1] = ("racecar",)
d[float("nan")]
```

```
Traceback (most recent call last):
  File "…", line 4, in <module>
    d[float("nan")]
    ~^^^^^^^^^^^^^^
ValueError: bad key: nan
```

Otherwise, if `key` is not present in `d`, raise `KeyError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ("bar", "baz")
d["spam"]
```

```
Traceback (most recent call last):
  File "…", line 4, in <module>
    d["spam"]
    ~^^^^^^^^
KeyError: 'spam'
```

#### `d[key] = value`

Map `value` to `key` in the sorted dictionary `d`, replacing the previously-mapped value (if any).

If no key-value pairs have been inserted into `d` yet and `type(key)` isn't one of the supported types (`bytes`,
`float`, `int` and `str`), raise `TypeError`.

```python
from pysorteddict import *
d = SortedDict()
d[["eggs"]] = None
```

```
Traceback (most recent call last):
  File "…", line 3, in <module>
    d[["eggs"]] = None
    ~^^^^^^^^^^
TypeError: unsupported key type: <class 'list'>
```

Otherwise, if `type(key)` does not match the type of the first key inserted into `d`, raise `TypeError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ("bar", "baz")
d[0xC0FFEE] = "spam"
```

```
Traceback (most recent call last):
  File "…", line 4, in <module>
    d[0xC0FFEE] = "spam"
    ~^^^^^^^^^^
TypeError: wrong key type: want <class 'str'>, got <class 'int'>
```

Otherwise, if `key` is not comparable with instances of its type, raise `ValueError`.

```python
from pysorteddict import *
d = SortedDict()
d[1.1] = ("racecar",)
d[float("nan")] = {}
```

```
Traceback (most recent call last):
  File "…", line 4, in <module>
    d[float("nan")] = {}
    ~^^^^^^^^^^^^^^
ValueError: bad key: nan
```

#### `del d[key]`

Remove `key` and the value mapped to it from the sorted dictionary `d`.

If no key-value pairs have been inserted into `d` yet, raise `ValueError`.

```python
from pysorteddict import *
d = SortedDict()
del d["foo"]
```

```
Traceback (most recent call last):
  File "…", line 3, in <module>
    del d["foo"]
        ~^^^^^^^
ValueError: key type not set: insert at least one item first
```

Otherwise, if `type(key)` does not match the type of the first key inserted into `d`, raise `TypeError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ("bar", "baz")
del d[0xC0FFEE]
```

```
Traceback (most recent call last):
  File "…", line 4, in <module>
    del d[0xC0FFEE]
        ~^^^^^^^^^^
TypeError: wrong key type: want <class 'str'>, got <class 'int'>
```

Otherwise, if `key` is not comparable with instances of its type, raise `ValueError`.

```python
from pysorteddict import *
d = SortedDict()
d[1.1] = ("racecar",)
del d[float("nan")]
```

```
Traceback (most recent call last):
  File "…", line 4, in <module>
    del d[float("nan")]
        ~^^^^^^^^^^^^^^
ValueError: bad key: nan
```

Otherwise, if `key` is not present in `d`, raise `KeyError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ("bar", "baz")
del d["spam"]
```

```
Traceback (most recent call last):
  File "…", line 4, in <module>
    del d["spam"]
        ~^^^^^^^^
KeyError: 'spam'
```

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

