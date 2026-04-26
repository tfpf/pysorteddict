Documentation
#############

.. default-domain:: py

.. currentmodule:: pysorteddict

.. data:: __version__
   :type: str

   The installed version of pysorteddict.

   .. jupyter-execute::

      from importlib.metadata import version

      import pysorteddict

      assert version("pysorteddict") == pysorteddict.__version__
      print(pysorteddict.__version__)

Sorted Dictionary
*****************

.. class:: SortedDict

   Sorted analogue of the ``dict`` type. Wraps the C++ sorted dictionary (``std::map``): key-value pairs are stored
   in ascending order of the keys, which must all be of the same type.

   The following key types are always supported.

   * ``bool``
   * ``bytes``
   * ``float``
   * ``int``
   * ``str``

   The following key types are supported if they are importable (which they should always be—failure to import them
   may be a sign of a corrupt or damaged Python installation).

   * ``datetime.date``
   * ``datetime.timedelta``
   * ``decimal.Decimal``
   * ``fractions.Fraction``
   * ``ipaddress.IPv4Address``
   * ``ipaddress.IPv4Interface``
   * ``ipaddress.IPv4Network``
   * ``ipaddress.IPv6Address``
   * ``ipaddress.IPv6Interface``
   * ``ipaddress.IPv6Network``
   * ``pathlib.PosixPath``
   * ``pathlib.PurePosixPath``
   * ``pathlib.PureWindowsPath``
   * ``pathlib.WindowsPath``
   * ``time.struct_time``
   * ``uuid.UUID``

   .. raw:: html

      <details class="warning">

      <summary>Shadowing standard library modules providing these types may lead to undefined behaviour.</summary>

   .. code-block:: python

      from pathlib import Path

      with Path(__file__).with_name("decimal.py").open("w") as writer:
          print("import math", file=writer)
          print('Decimal = type("Decimal", (float,), {"is_nan": lambda self: math.isnan(self)})', file=writer)

      from pysorteddict import SortedDict

      from decimal import Decimal

      d = SortedDict()
      d[Decimal(0)] = None

   Here, the imported ``Decimal`` (which is actually ``float`` with an ``is_nan`` method defined) came from the
   newly created ``decimal.py`` instead of the standard library module ``decimal``. This will work. However, if
   ``Decimal.__lt__`` (the comparison function used by the underlying C++ ``std::map``) is overridden to raise an
   exception, undefined behaviour will result.

   .. raw:: html

      </details>

   ``SortedDict`` can be type-specified for use in type hints as, for instance, ``SortedDict[str, float]``, similar to
   ``dict``. It can also be subclassed to override its methods.

   .. method:: __init__()

      Initialise an empty sorted dictionary.

   .. property:: key_type
      :type: type | None

      The key type of the sorted dictionary. It is initially not set (so it evaluates to ``None``), and is
      automatically set when a key-value pair is inserted.

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         assert d.key_type is None

         d[b"foo"] = ()
         assert d.key_type is bytes

      It can manually be set to a supported key type before inserting a key-value pair. This is useful to avoid the
      ``RuntimeError``\s which would otherwise be raised on any read operations.

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         d.key_type = bytes
         d[b"foo"] = ()

      .. raw:: html

         <details class="warning">

         <summary>This property may raise exceptions.</summary>

      :raises ValueError: if an attempt is made to set it to an unsupported key type.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d.key_type = list

      :raises AttributeError: if an attempt is made to change it after it has been set.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d[b"foo"] = ()
         d.key_type = str

      .. raw:: html

         </details>

   .. method:: __repr__() -> str

      Return a human-readable representation of the sorted dictionary.

   .. method:: __contains__(key: Any) -> bool

      Return whether ``key`` is present in the sorted dictionary.

      .. raw:: html

         <details class="warning">

         <summary>This method may raise exceptions.</summary>

      :raises RuntimeError: if the key type of the sorted dictionary is not set.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         "foo" in d

      :raises TypeError: if ``type(key)`` does not match the key type of the sorted dictionary.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         100 in d

      :raises ValueError: if ``key`` is not comparable with instances of its type.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d[1.1] = ("racecar",)
         float("nan") in d

      .. raw:: html

         </details>

   .. method:: __len__() -> int

      Return the number of key-value pairs in the sorted dictionary.

      .. raw:: html

         <details class="warning">

         <summary>This method may raise exceptions.</summary>

      :raises OverflowError: if the number of key-value pairs exceeds ``PY_SSIZE_T_MAX`` (which is usually
       9223372036854775807 on 64-bit operating systems, making this exception astronomically unlikely).

      .. raw:: html

         </details>

   .. method:: __getitem__(key: Any) -> Any

      Return the value mapped to ``key`` in the sorted dictionary.

      .. raw:: html

         <details class="warning">

         <summary>This method may raise exceptions.</summary>

      :raises RuntimeError: if the key type of the sorted dictionary is not set.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"]

      :raises TypeError: if ``type(key)`` does not match the key type of the sorted dictionary.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         d[100]

      :raises ValueError: if ``key`` is not comparable with instances of its type.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d[1.1] = ("racecar",)
         d[float("nan")]

      :raises KeyError: if ``key`` is not present in the sorted dictionary.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         d["spam"]

      .. raw:: html

         </details>

   .. method:: __setitem__(key: Any, value: Any)

      Insert ``key`` into the sorted dictionary (if it isn't already in it) and map ``value`` to it, replacing the
      previously mapped value (if any).

      .. raw:: html

         <details class="warning">

         <summary>This method may raise exceptions.</summary>

      :raises TypeError: if the key type of the sorted dictionary is not set and ``type(key)`` isn't one of the
       supported types.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d[["eggs"]] = None

      :raises TypeError: if ``type(key)`` does not match the key type of the sorted dictionary.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         d[100] = "spam"

      :raises ValueError: if ``key`` is not comparable with instances of its type.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d[1.1] = ("racecar",)
         d[float("nan")] = {}

      .. raw:: html

         </details>

   .. method:: __delitem__(key: Any)

      Remove ``key`` and the value mapped to it from the sorted dictionary.

      .. raw:: html

         <details class="warning">

         <summary>This method may raise exceptions.</summary>

      :raises RuntimeError: if the key type of the sorted dictionary is not set.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         del d["foo"]

      :raises TypeError: if ``type(key)`` does not match the key type of the sorted dictionary.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         del d[100]

      :raises ValueError: if ``key`` is not comparable with instances of its type.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d[1.1] = ("racecar",)
         del d[float("nan")]

      :raises KeyError: if ``key`` is not present in the sorted dictionary.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         del d["spam"]

      :raises RuntimeError: if there exists a forward iterator over the items, keys or values of the sorted
       dictionary pointing to ``key`` (meaning that calling ``next`` on the iterator would return ``(key, d[key])``,
       ``key`` or ``d[key]`` respectively).

      .. raw:: html

         <svg version="1.1" width="100%" viewBox="0 0 400 100" xmlns="http://www.w3.org/2000/svg">
           <circle cx="30" cy="50" r="2" fill="#27B4A6" opacity="0.4" />
           <circle cx="40" cy="50" r="2" fill="#27B4A6" opacity="0.4" />
           <circle cx="50" cy="50" r="2" fill="#27B4A6" opacity="0.4" />
           <polygon points="70,40 70,60 90,50" fill="#27B4A6" opacity="0.4" />
           <rect x="110" y="20" width="60" height="60" fill="#27B4A6" opacity="0.4"/>
           <text x="140" y="10" text-anchor="middle" dominant-baseline="middle">Last yielded (if any)</text>
           <polygon points="190,40 190,60 210,50" fill="#27B4A6" opacity="0.4" />
           <rect x="230" y="20" width="60" height="60" fill="#27B4A6" opacity="0.4"/>
           <text x="260" y="10" text-anchor="middle" dominant-baseline="middle">Next to yield</text>
           <text x="260" y="90" text-anchor="middle" dominant-baseline="middle">Deletion forbidden</text>
           <polygon points="310,40 310,60 330,50" fill="#27B4A6" opacity="0.4" />
           <circle cx="350" cy="50" r="2" fill="#27B4A6" opacity="0.4" />
           <circle cx="360" cy="50" r="2" fill="#27B4A6" opacity="0.4" />
           <circle cx="370" cy="50" r="2" fill="#27B4A6" opacity="0.4" />
         </svg>

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         for i in range(5):
             d[i] = None
         ii = iter(d.items())
         ki = iter(d.keys())
         vi = iter(d.values())
         del d[0]

      :raises RuntimeError: if there exists a reverse iterator over the items, keys or values of the sorted
       dictionary pointing to the key immediately less than ``key`` (meaning that calling ``next`` on the iterator
       last returned ``(key, d[key])``, ``key`` or ``d[key]`` respectively).

      .. raw:: html

         <svg version="1.1" width="100%" viewBox="0 0 400 100" xmlns="http://www.w3.org/2000/svg">
           <circle cx="30" cy="50" r="2" fill="#27B4A6" opacity="0.4" />
           <circle cx="40" cy="50" r="2" fill="#27B4A6" opacity="0.4" />
           <circle cx="50" cy="50" r="2" fill="#27B4A6" opacity="0.4" />
           <polygon points="70,50 90,60 90,40" fill="#27B4A6" opacity="0.4" />
           <rect x="110" y="20" width="60" height="60" fill="#27B4A6" opacity="0.4"/>
           <text x="140" y="10" text-anchor="middle" dominant-baseline="middle">Next to yield</text>
           <polygon points="190,50 210,60 210,40" fill="#27B4A6" opacity="0.4" />
           <rect x="230" y="20" width="60" height="60" fill="#27B4A6" opacity="0.4"/>
           <text x="260" y="10" text-anchor="middle" dominant-baseline="middle">Last yielded</text>
           <text x="260" y="90" text-anchor="middle" dominant-baseline="middle">Deletion forbidden</text>
           <polygon points="310,50 330,60 330,40" fill="#27B4A6" opacity="0.4" />
           <circle cx="350" cy="50" r="2" fill="#27B4A6" opacity="0.4" />
           <circle cx="360" cy="50" r="2" fill="#27B4A6" opacity="0.4" />
           <circle cx="370" cy="50" r="2" fill="#27B4A6" opacity="0.4" />
         </svg>

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         for i in range(5):
             d[i] = None
         ii = reversed(d.items())
         ki = reversed(d.keys())
         vi = reversed(d.values())
         assert (next(ii), next(ki), next(vi)) == ((4, None), 4, None)
         del d[4]

      .. raw:: html

         </details>

         <details class="warning">

         <summary>This method may behave differently with PyPy.</summary>

      PyPy does not run the destructor of an object immediately after it becomes unreachable. Hence, iterators
      deleted prematurely will keep a key-value pair locked.

      .. code-block:: python

         import gc

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = "bar"
         d["baz"] = 1
         ii = iter(d.items())
         ki = iter(d.keys())
         vi = iter(d.values())
         del ii, ki, vi
         # gc.collect()
         del d["baz"]

      Uncommenting the commented line runs any required destructors, ensuring that no exception is raised.

      .. raw:: html

         </details>

   .. method:: __iter__() -> SortedDictKeysFwdIter

      Return a forward iterator over the keys in the sorted dictionary. ``iter(d)`` is an efficient shorthand for
      ``iter(d.keys())``. Typical usage is to iterate directly over the sorted dictionary instead of making this
      call.

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ()
         d["bar"] = [100]
         d["baz"] = 3.14

         for key in d:
             print(key)

   See also :meth:`SortedDictKeys.__iter__`.

   .. method:: __reversed__() -> SortedDictKeysRevIter

      Return a reverse iterator over the keys in the sorted dictionary. ``reversed(d)`` is an efficient shorthand for
      ``reversed(d.keys())``.

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ()
         d["bar"] = [100]
         d["baz"] = 3.14

         for key in reversed(d):
             print(key)

   See also :meth:`SortedDictKeys.__reversed__`.

   .. method:: clear()

      Remove all key-value pairs in the sorted dictionary.

      .. raw:: html

         <details class="warning">

         <summary>This method may raise exceptions.</summary>

      :raises RuntimeError: if there exist unexhausted iterators over the items, keys or values of the sorted
       dictionary.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = "bar"
         ii = iter(d.items())
         ki = iter(d.keys())
         vi = reversed(d.values())
         d.clear()

      .. raw:: html

         </details>

         <details class="warning">

         <summary>This method may behave differently with PyPy.</summary>

      PyPy does not run the destructor of an object immediately after it becomes unreachable. Hence, iterators deleted
      prematurely will keep a sorted dictionary locked.

      .. code-block:: python

         import gc

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = "bar"
         ii = iter(d.items())
         ki = iter(d.keys())
         vi = reversed(d.values())
         del ii, ki, vi
         # gc.collect()
         d.clear()

      Uncommenting the commented line runs any required destructors, ensuring that no exception is raised.

      .. raw:: html

         </details>

   .. method:: copy() -> SortedDict

      Return a shallow copy of the sorted dictionary.

   .. method:: get(key: Any, default: Any = None, /) -> Any

      Return the value mapped to ``key`` in the sorted dictionary, or ``default`` if ``key`` isn't in present in it.

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = "bar"

         assert d.get("foo") == "bar"
         assert d.get("baz") is None
         assert d.get("spam", "eggs") == "eggs"

      .. raw:: html

         <details class="warning">

         <summary>This method may raise exceptions.</summary>

      :raises RuntimeError: if the key type of the sorted dictionary is not set.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d.get("foo")

      :raises TypeError: if ``type(key)`` does not match the key type of the sorted dictionary.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         d.get(100)

      :raises ValueError: if ``key`` is not comparable with instances of its type.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d[1.1] = ("racecar",)
         d.get(float("nan"))

      .. raw:: html

         </details>

   .. method:: items() -> SortedDictItems

      Return a dynamic view on the key-value pairs in the sorted dictionary.

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         items = d.items()
         d["foo"] = ()
         print(items)

         d["bar"] = [100]
         print(items)

         d["baz"] = 3.14
         print(items)

      See :ref:`sorted-dictionary-views`.

   .. method:: keys() -> SortedDictKeys

      Return a dynamic view on the keys in the sorted dictionary.

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         keys = d.keys()
         d["foo"] = ()
         print(keys)

         d["bar"] = [100]
         print(keys)

         d["baz"] = 3.14
         print(keys)

      See :ref:`sorted-dictionary-views`.

   .. method:: setdefault(key: Any, default: Any = None, /) -> Any

      If ``key`` is present in the sorted dictionary, return the value mapped to it. Otherwise, insert ``key`` into it,
      map ``default`` to ``key`` and return ``default``.

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = "bar"

         assert d.setdefault("foo") == "bar"
         assert d.setdefault("baz") is None
         assert d["baz"] is None
         assert d.setdefault("spam", "eggs") == "eggs"
         assert d["spam"] == "eggs"

      .. raw:: html

         <details class="warning">

         <summary>This method may raise exceptions.</summary>

      :raises RuntimeError: if the key type of the sorted dictionary is not set.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d.setdefault("foo")

      :raises TypeError: if ``type(key)`` does not match the key type of the sorted dictionary.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         d.setdefault(100)

      :raises ValueError: if ``key`` is not comparable with instances of its type.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d[1.1] = ("racecar",)
         d.setdefault(float("nan"))

      .. raw:: html

         </details>

   .. method:: update(other: dict | Iterable[Sequence[Any]], **kwargs)

      Update the sorted dictionary with the keys and values from ``other``. ``kwargs`` is ignored.

      The rough Python equivalent of the logic written in C++ is as follows.

      .. code-block:: python

         def update(self, other, **kwargs):
            if hasattr(other, "keys"):
                for key in other:
                    self[key] = other[key]
            else:
                for key, value in other:
                    self[key] = value

      If ``other`` has a ``keys`` method, it is assumed to be a dictionary, and the keys and values in it are inserted
      into the sorted dictionary. Else, it must be an iterable which yields 2-length sequences; these are treated as
      key-value pairs and inserted into the sorted dictionary.

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()

         d.update({"spam": {}, "eggs": ""})
         print(d)

         d.update([("foo", ()), ("bar", [100]), ("baz", 3.14)])
         print(d)

      .. raw:: html

         <details class="warning">

         <summary>This method may raise exceptions.</summary>

      :raises TypeError: if ``other`` is not iterable.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d.update(None)

      :raises TypeError: if ``other`` did not yield a sequence at some point.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d.update([None])

      :raises ValueError: if ``other`` did not yield a 2-length sequence at some point.

      .. jupyter-execute::
         :raises:

         from pysorteddict import SortedDict

         d = SortedDict()
         d.update([[None]])

      :raises: the same exception that iterating over ``other`` raises (if any).
      :raises: the same exception that reading ``other[key]`` raises (if any).
      :raises: the same exception that writing ``self[key]`` (:meth:`SortedDict.__setitem__`) raises (if
       any).

      .. raw:: html

         </details>

   .. method:: values() -> SortedDictValues

      Return a dynamic view on the values in the sorted dictionary.

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         values = d.values()
         d["foo"] = ()
         print(values)

         d["bar"] = [100]
         print(values)

         d["baz"] = 3.14
         print(values)

      See :ref:`sorted-dictionary-views`.

.. _sorted-dictionary-views:

Sorted Dictionary Views
***********************

Sorted dictionary views are dynamic views on a sorted dictionary: they are immutable and cannot be used to mutate the
sorted dictionary, but always reflect its current state.

There are three view types.

.. class:: SortedDictItems

   A view representing a sorted set of key-value pairs. Instances of this type are returned by
   :meth:`SortedDict.items`, but it is not user-importable.

   .. method:: __len__() -> int

      Return the number of key-value pairs in the sorted dictionary view.

      The behaviour is equivalent to that of :meth:`len(d) <SortedDict.__len__>` where ``d`` is the underlying sorted
      dictionary.

   .. method:: __contains__(ob: Any) -> bool

      Return whether ``ob`` is present in the sorted dictionary view.

      If ``ob`` is not a two-element ``tuple``, return ``False``. Otherwise, the behaviour is equivalent to that of
      ``ob[0] in d and d[ob[0]] == ob[1]`` where ``d`` is the underlying sorted dictionary.

   .. method:: __getitem__(index_or_slice: int | slice) -> Any

      Return the key-value pair at the given index or those in the given slice. The behaviour is equivalent to indexing
      or slicing a ``list`` containing the key-value pairs.

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         items = d.items()
         d["foo"] = ()
         d["bar"] = [100]
         d["baz"] = 3.14
         d["spam"] = {}
         d["eggs"] = ""

         print(d)
         print(items[0], items[2], items[4])
         print(items[:3])
         print(items[1:])
         print(items[-3:3])
         print(items[-5:4:2])
         print(items[::-1])

   .. method:: __iter__() -> SortedDictItemsFwdIter

      Return a forward iterator over the key-value pairs in the sorted dictionary view.

      .. raw:: html

         <details class="notice">

         <summary>Modifications made while iterating over the key-value pairs have well-defined behaviour.</summary>

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ()
         d["bar"] = [100]
         d["baz"] = 3.14
         for key, value in d.items():
             d[key] = f"spam_{value}"
             d["a_" + key] = f"eggs_{value}"
             if "foo" in d:
                 del d["foo"]

         for key, value in d.items():
            print(key, "->", value)

      See the exceptions raised by :meth:`SortedDict.__delitem__` and :meth:`SortedDict.clear` for the restrictions
      that apply.

      .. raw:: html

         </details>

   .. method:: __reversed__() -> SortedDictItemsRevIter

      Return a reverse iterator over the key-value pairs in the sorted dictionary view.

      .. raw:: html

         <details class="notice">

         <summary>Modifications made while iterating over the key-value pairs have well-defined behaviour.</summary>

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ()
         d["bar"] = [100]
         d["baz"] = 3.14
         for key, value in reversed(d.items()):
             d[key] = f"spam_{value}"
             d["z_" + key] = f"eggs_{value}"
             if "bar" in d:
                 del d["bar"]

         for key, value in d.items():
            print(key, "->", value)

      See the exceptions raised by :meth:`SortedDict.__delitem__` and :meth:`SortedDict.clear` for the restrictions
      that apply.

      .. raw:: html

         </details>

.. class:: SortedDictKeys

   A view representing a sorted set of keys. Instances of this type are returned by :meth:`SortedDict.keys`, but it is
   not user-importable.

   .. method:: __len__() -> int

      Return the number of keys in the sorted dictionary view.

      The behaviour is equivalent to that of :meth:`len(d) <SortedDict.__len__>` where ``d`` is the underlying sorted
      dictionary.

   .. method:: __contains__(ob: Any) -> bool

      Return whether ``ob`` is present in the sorted dictionary view.

      The behaviour is equivalent to that of :meth:`ob in d <SortedDict.__contains__>` where ``d`` is the underlying
      sorted dictionary.

   .. method:: __getitem__(index_or_slice: int | slice) -> Any

      Return the key at the given index or those in the given slice. The behaviour is equivalent to indexing or slicing
      a ``list`` containing the keys.

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         keys = d.keys()
         d["foo"] = ()
         d["bar"] = [100]
         d["baz"] = 3.14
         d["spam"] = {}
         d["eggs"] = ""

         print(d)
         print(keys[0], keys[2], keys[4])
         print(keys[:3])
         print(keys[1:])
         print(keys[-3:3])
         print(keys[-5:4:2])
         print(keys[::-1])

   .. method:: __iter__() -> SortedDictKeysFwdIter

      Return a forward iterator over the keys in the sorted dictionary view.

      .. raw:: html

         <details class="notice">

         <summary>Modifications made while iterating over the keys have well-defined behaviour.</summary>

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ()
         d["bar"] = [100]
         d["baz"] = 3.14
         for key in d:
             d[key] = "spam"
             d["a_" + key] = "eggs"
             if "foo" in d:
                 del d["foo"]

         for key, value in d.items():
            print(key, "->", value)

      See the exceptions raised by :meth:`SortedDict.__delitem__` and :meth:`SortedDict.clear` for the restrictions
      that apply.

      .. raw:: html

         </details>

   .. method:: __reversed__() -> SortedDictKeysRevIter

      Return a reverse iterator over the keys in the sorted dictionary view.

      .. raw:: html

         <details class="notice">

         <summary>Modifications made while iterating over the keys have well-defined behaviour.</summary>

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ()
         d["bar"] = [100]
         d["baz"] = 3.14
         for key in reversed(d):
             d[key] = "spam"
             d["z_" + key] = "eggs"
             if "bar" in d:
                 del d["bar"]

         for key, value in d.items():
            print(key, "->", value)

      See the exceptions raised by :meth:`SortedDict.__delitem__` and :meth:`SortedDict.clear` for the restrictions
      that apply.

      .. raw:: html

         </details>

.. class:: SortedDictValues

   A view representing an array of values ordered by the keys they are mapped to. Instances of this type are returned
   by :meth:`SortedDict.values`, but it is not user-importable.

   .. method:: __len__() -> int

      Return the number of values in the sorted dictionary view.

      The behaviour is equivalent to that of :meth:`len(d) <SortedDict.__len__>` where ``d`` is the underlying sorted
      dictionary.

   .. method:: __contains__(ob: Any) -> bool

      Return whether ``ob`` is present in the sorted dictionary view.

      The behaviour is equivalent to that of ``ob in l`` where ``l`` is a ``list`` of the elements in the view in the
      same order. In other words, making this call leads to an element-by-element comparison.

   .. method:: __getitem__(index_or_slice: int | slice) -> Any

      Return the value at the given index or those in the given slice. The behaviour is equivalent to indexing or
      slicing a ``list`` containing the values.

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         values = d.values()
         d["foo"] = ()
         d["bar"] = [100]
         d["baz"] = 3.14
         d["spam"] = {}
         d["eggs"] = ""

         print(d)
         print(values[0], values[2], values[4])
         print(values[:3])
         print(values[1:])
         print(values[-3:3])
         print(values[-5:4:2])
         print(values[::-1])

   .. method:: __iter__() -> SortedDictValuesFwdIter

      Return a forward iterator over the values in the sorted dictionary view.

      .. raw:: html

         <details class="notice">

         <summary>Modifications made while iterating over the values have well-defined behaviour.</summary>

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ()
         d["bar"] = [100]
         d["baz"] = 3.14
         for value in d.values():
             d[f"a_{value}"] = f"spam_{value}"
             d["eggs"] = value
             if "foo" in d:
                 del d["foo"]

         for key, value in d.items():
            print(key, "->", value)

      See the exceptions raised by :meth:`SortedDict.__delitem__` and :meth:`SortedDict.clear` for the restrictions
      that apply.

      .. raw:: html

         </details>

   .. method:: __reversed__() -> SortedDictValuesRevIter

      Return a reverse iterator over the values in the sorted dictionary view.

      .. raw:: html

         <details class="notice">

         <summary>Modifications made while iterating over the values have well-defined behaviour.</summary>

      .. jupyter-execute::

         from pysorteddict import SortedDict

         d = SortedDict()
         d["foo"] = ()
         d["bar"] = [100]
         d["baz"] = 3.14
         for value in reversed(d.values()):
             d[f"z_{value}"] = f"spam_{value}"
             d["eggs"] = value
             if "bar" in d:
                 del d["bar"]

         for key, value in d.items():
            print(key, "->", value)

      See the exceptions raised by :meth:`SortedDict.__delitem__` and :meth:`SortedDict.clear` for the restrictions
      that apply.

      .. raw:: html

         </details>
