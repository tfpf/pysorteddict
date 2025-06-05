# Documentation

<details class="notice">

<summary>Looking for the documentation of an older version?</summary>

▸ [0.7.0](https://github.com/tfpf/pysorteddict/blob/v0.7.0/docs/documentation.md)  
▸ [0.6.0](https://github.com/tfpf/pysorteddict/blob/v0.6.0/docs/documentation.md)  
▸ [0.5.3](https://github.com/tfpf/pysorteddict/blob/v0.5.3/docs/documentation.md)
▸ [0.5.2](https://github.com/tfpf/pysorteddict/blob/v0.5.2/docs/documentation.md)
▸ [0.5.1](https://github.com/tfpf/pysorteddict/blob/v0.5.1/docs/documentation.md)
▸ [0.5.0](https://github.com/tfpf/pysorteddict/blob/v0.5.0/docs/documentation.md)  
▸ [0.4.6](https://github.com/tfpf/pysorteddict/blob/v0.4.6/docs/documentation.md)
▸ [0.4.5](https://github.com/tfpf/pysorteddict/blob/v0.4.5/docs/index.md)
▸ [0.4.4](https://github.com/tfpf/pysorteddict/blob/v0.4.4/docs/index.md)

</details>

`SortedDict` is a thread-unsafe sorted dictionary. It may be imported explicitly:

```python
from pysorteddict import SortedDict
```

or implicitly using the wildcard (though this is not recommended).

```python
from pysorteddict import *
```

The following key types are supported.

* `bytes`
* `float`
* `int`
* `str`
* `decimal.Decimal`

## Constructor

### `SortedDict(*args, **kwargs) -> SortedDict`

Create an empty sorted dictionary. `args` and `kwargs` are ignored.

<details class="warning">

<summary>This method may raise exceptions.</summary>

If any of the supported key types which are not built-in (only `decimal.Decimal` as of this version) cannot be imported
(which might be a symptom of a corrupt or damaged Python installation), raises `ImportError`.

```python
from pysorteddict import *
d = SortedDict()
```

```text
Traceback (most recent call last):
  File "…", line 2, in <module>
    d = SortedDict()
ImportError: failed to import the `decimal.Decimal` type
```

</details>

## Properties

### `d.key_type: type | None`

The key type of the sorted dictionary `d`, or `None` if no key-value pairs have been inserted in it.

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

<details class="warning">

<summary>This method may raise exceptions.</summary>

If no key-value pairs have been inserted into `d` yet, raises `RuntimeError`.

```python
from pysorteddict import *
d = SortedDict()
"foo" in d
```

```text
Traceback (most recent call last):
  File "…", line 3, in <module>
    "foo" in d
RuntimeError: key type not set: insert at least one item first
```

Otherwise, if `type(key)` does not match the type of the first key inserted into `d`, raises `TypeError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ("bar", "baz")
100 in d
```

```text
Traceback (most recent call last):
  File "…", line 4, in <module>
    100 in d
TypeError: got key 100 of type <class 'int'>, want key of type <class 'str'>
```

Otherwise, if `key` is not comparable with instances of its type, raises `ValueError`.

```python
from pysorteddict import *
d = SortedDict()
d[1.1] = ("racecar",)
float("nan") in d
```

```text
Traceback (most recent call last):
  File "…", line 4, in <module>
    float("nan") in d
ValueError: got bad key nan of type <class 'float'>
```

</details>

### `len(d)`

Return the number of key-value pairs in the sorted dictionary `d`.

### `d[key]`

Return the value mapped to `key` in the sorted dictionary `d`.

<details class="warning">

<summary>This method may raise exceptions.</summary>

If no key-value pairs have been inserted into `d` yet, raises `RuntimeError`.

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

Otherwise, if `type(key)` does not match the type of the first key inserted into `d`, raises `TypeError`.

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

Otherwise, if `key` is not comparable with instances of its type, raises `ValueError`.

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

Otherwise, if `key` is not present in `d`, raises `KeyError`.

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

</details>

### `d[key] = value`

Map `value` to `key` in the sorted dictionary `d`, replacing the previously-mapped value (if any).

<details class="warning">

<summary>This method may raise exceptions.</summary>

If no key-value pairs have been inserted into `d` yet and `type(key)` isn't one of the supported types (`bytes`,
`float`, `int`, `str` and `decimal.Decimal`), raises `TypeError`.

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

Otherwise, if `type(key)` does not match the type of the first key inserted into `d`, raises `TypeError`.

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

Otherwise, if `key` is not comparable with instances of its type, raises `ValueError`.

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

</details>

### `del d[key]`

Remove `key` and the value mapped to it from the sorted dictionary `d`.

<details class="warning">

<summary>This method may raise exceptions.</summary>

If no key-value pairs have been inserted into `d` yet, raises `RuntimeError`.

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

Otherwise, if `type(key)` does not match the type of the first key inserted into `d`, raises `TypeError`.

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

Otherwise, if `key` is not comparable with instances of its type, raises `ValueError`.

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

Otherwise, if `key` is not present in `d`, raises `KeyError`.

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

Otherwise, if there exists an iterator over the keys of `d` pointing to `key` (i.e. calling `next` on the iterator
would yield `key`), raises `RuntimeError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = "bar"
d["baz"] = 1
i = iter(d.keys())
del d["baz"]
```

```text
Traceback (most recent call last):
  File "…", line 6, in <module>
    del d["baz"]
        ~^^^^^^^
RuntimeError: operation not permitted: key-value pair locked by 1 iterator(s)
```

</details>

<details class="warning">

<summary>This method may behave differently with PyPy.</summary>

PyPy does not run the destructor of an object immediately after it becomes unreachable. Hence, prematurely-deleted
iterators will keep a key-value pair locked.

```python
import gc
from pysorteddict import *
d = SortedDict()
d["foo"] = "bar"
d["baz"] = 1
i = iter(d.keys())
del i
# gc.collect()
del d["baz"]
```

```text
Traceback (most recent call last):
  File "/home/tfpf/Documents/projects/pysorteddict/t.py", line 9, in <module>
    del d["baz"]
        ~^^^^^^^
RuntimeError: operation not permitted: key-value pair locked by 1 iterator(s)
```

Uncommenting the commented line runs any required destructors and makes this error go away.

</details>

## Other Methods

### `d.clear()`

Remove all key-value pairs in the sorted dictionary `d`.

<details class="warning">

<summary>This method may raise exceptions.</summary>

If there exists an unexhausted iterator over the keys of `d`, raises `RuntimeError`.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = "bar"
i = iter(d.keys())
d.clear()
```

```text
Traceback (most recent call last):
  File "…", line 5, in <module>
    d.clear()
RuntimeError: operation not permitted: sorted dictionary locked by 1 iterator(s)
```

</details>

<details class="warning">

<summary>This method may behave differently with PyPy.</summary>

PyPy does not run the destructor of an object immediately after it becomes unreachable. Hence, prematurely-deleted
iterators will keep a sorted dictionary locked.

```python
import gc
from pysorteddict import *
d = SortedDict()
d["foo"] = "bar"
d["baz"] = 1
i = iter(d.keys())
del i
# gc.collect()
d.clear()
```

```text
Traceback (most recent call last):
  File "…", line 9, in <module>
    d.clear()
RuntimeError: operation not permitted: sorted dictionary locked by 1 iterator(s)
```

Uncommenting the commented line runs any required destructors and makes this error go away.

</details>

### `d.copy() -> SortedDict`

Return a shallow copy of the sorted dictionary `d`.

### `d.items() -> list[tuple[object, object]]`

Return the key-value pairs in the sorted dictionary `d`. The list will be sorted. It will exist independently of `d`;
it won't be a view on its items.

### `d.keys() -> SortedDictKeys`

Return a dynamic view on the keys in the sorted dictionary `d`.

```python
from pysorteddict import *
d = SortedDict()
keys = d.keys()
d["foo"] = ()
print(keys)
d["bar"] = [100]
print(keys)
d["baz"] = 3.14
print(keys)
```

```text
SortedDictKeys(['foo'])
SortedDictKeys(['bar', 'foo'])
SortedDictKeys(['bar', 'baz', 'foo'])
```

<div class="notice">
Iteration over the keys is supported, and the sorted dictionary can be modified while such an iteration is ongoing.

```python
from pysorteddict import *
d = SortedDict()
d["foo"] = ()
d["bar"] = [100]
d["baz"] = 3.14
for key in d.keys():
    d[key] = "spam"
print(d)
```

```text
SortedDict({'bar': 'spam', 'baz': 'spam', 'foo': 'spam'})
```

There are some minor restrictions on modification during iteration. See [`del d[key]`](#del-d-key) and
[`d.clear()`](d-clear).
</div>

### `d.values() -> list[object]`

Return the values in the sorted dictionary `d`. The list will be sorted by the keys the values are mapped to. It will
exist independently of `d`; it won't be a view on its values.
