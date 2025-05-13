This page documents pysorteddict v0.4.5.

`SortedDict` may be imported explicitly:

```python
from pysorteddict import SortedDict
```

or implicitly using the wildcard (though this is not recommended).

```python
from pysorteddict import *
```

# Constructor

## `SortedDict(*args, **kwargs) -> SortedDict`

Create an empty sorted dictionary. `args` and `kwargs` are ignored.

# Properties

## `d.key_type: type | None`

Return the key type of the sorted dictionary `d`, or `None` if no key-value pairs have been inserted in it.

```python
from pysorteddict import *
d = SortedDict()
assert d.key_type is None
d[b"foo"] = ()
assert d.key_type is bytes
```

# Magic Methods

## `repr(d)`

Return a human-readable representation of the sorted dictionary `d`.

## `key in d`

Return whether `key` is present in the sorted dictionary `d`.

## `len(d)`

Return the number of key-value pairs in the sorted dictionary `d`.

## `d[key]`

Return the value mapped to `key` in the sorted dictionary `d`.

### Exceptions

If no key-value pairs have been inserted into `d` yet, raise `ValueError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"]
```

```
Traceback (most recent call last):
  File "C:/Users/vpaij/Documents/projects/pysorteddict/examples/basic.py", line 3, in <module>
    d["foo"]
    ~^^^^^^^
ValueError: key type not set: insert at least one item first
```

Otherwise, if `type(key)` does not match the type of the first key inserted into `d`, raise `TypeError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ("bar", "baz")
d[100]
```

```
Traceback (most recent call last):
  File "C:/Users/vpaij/Documents/projects/pysorteddict/examples/basic.py", line 4, in <module>
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

```
Traceback (most recent call last):
  File "C:/Users/vpaij/Documents/projects/pysorteddict/examples/basic.py", line 4, in <module>
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

```
Traceback (most recent call last):
  File "C:/Users/vpaij/Documents/projects/pysorteddict/examples/basic.py", line 4, in <module>
    d["spam"]
    ~^^^^^^^^
KeyError: 'spam'
```

## `d[key] = value`

Map `value` to `key` in the sorted dictionary `d`, replacing the previously-mapped value (if any).

### Exceptions

If no key-value pairs have been inserted into `d` yet and `type(key)` isn't one of the supported types (`bytes`,
`float`, `int` and `str`), raise `TypeError`.

```python
from pysorteddict import *
d = SortedDict()
d[["eggs"]] = None
```

```
Traceback (most recent call last):
  File "C:/Users/vpaij/Documents/projects/pysorteddict/examples/basic.py", line 3, in <module>
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

```
Traceback (most recent call last):
  File "C:/Users/vpaij/Documents/projects/pysorteddict/examples/basic.py", line 4, in <module>
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

```
Traceback (most recent call last):
  File "C:/Users/vpaij/Documents/projects/pysorteddict/examples/basic.py", line 4, in <module>
    d[float("nan")] = {}
    ~^^^^^^^^^^^^^^
ValueError: got bad key nan of type <class 'float'>
```

## `del d[key]`

Remove `key` and the value mapped to it from the sorted dictionary `d`.

### Exceptions

If no key-value pairs have been inserted into `d` yet, raise `ValueError`.

```python
from pysorteddict import *
d = SortedDict()
del d["foo"]
```

```
Traceback (most recent call last):
  File "C:/Users/vpaij/Documents/projects/pysorteddict/examples/basic.py", line 3, in <module>
    del d["foo"]
        ~^^^^^^^
ValueError: key type not set: insert at least one item first
```

Otherwise, if `type(key)` does not match the type of the first key inserted into `d`, raise `TypeError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ("bar", "baz")
del d[100]
```

```
Traceback (most recent call last):
  File "C:/Users/vpaij/Documents/projects/pysorteddict/examples/basic.py", line 4, in <module>
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

```
Traceback (most recent call last):
  File "C:/Users/vpaij/Documents/projects/pysorteddict/examples/basic.py", line 4, in <module>
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

```
Traceback (most recent call last):
  File "C:/Users/vpaij/Documents/projects/pysorteddict/examples/basic.py", line 4, in <module>
    del d["spam"]
        ~^^^^^^^^
KeyError: 'spam'
```

# Other Methods

## `d.clear()`

Remove all key-value pairs in the sorted dictionary `d`.

## `d.copy() -> SortedDict`

Return a shallow copy of the sorted dictionary `d`.

## `d.items() -> list[tuple[object, object]]`

Return the key-value pairs in the sorted dictionary `d`. The list will be sorted. It will exist independently of `d`;
it won't be a view on its items.

## `d.keys() -> list[object]`

Return the keys in the sorted dictionary `d`. The list will be sorted. It will exist independently of ``d``; it won't
be a view on its keys.

## `d.values() -> list[object]`

Return the values in the sorted dictionary `d`. The list will be sorted by the keys the values are mapped to. It will
exist independently of `d`; it won't be a view on its values.
