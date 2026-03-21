Documentation
#############

.. default-domain:: py

.. currentmodule:: pysorteddict

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

      from pysorteddict import *

      from decimal import Decimal

      d = SortedDict()
      d[Decimal(0)] = None

   Here, the imported ``Decimal`` (which is actually ``float`` with an ``is_nan`` method defined) came from the
   newly created ``decimal.py`` instead of the standard library module ``decimal``. This will work. However, if
   ``Decimal.__lt__`` (the comparison function used by the underlying C++ ``std::map``) is overridden to raise an
   exception, undefined behaviour will result.

   .. raw:: html

      </details>

   The methods of ``SortedDict`` are somewhat similar to those of ``dict``.

   .. method:: __init__()

      Initialise an empty sorted dictionary.

   .. property:: key_type
      :type: type | None

      The key type of the sorted dictionary, or ``None`` if no key-value pairs have been inserted in it.

      .. jupyter-execute::

         from pysorteddict import *

         d = SortedDict()
         assert d.key_type is None

         d[b"foo"] = ()
         assert d.key_type is bytes

   .. method:: __repr__() -> str

      Return a human-readable representation of the sorted dictionary.

   .. method:: __contains__(key: Any) -> bool

      Return whether ``key`` is present in the sorted dictionary.

      .. raw:: html

         <details class="warning">

         <summary>This method may raise exceptions.</summary>

      :raises RuntimeError: if no key-value pairs have been inserted yet.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         "foo" in d

      :raises TypeError: if ``type(key)`` does not match the type of the first key inserted.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         100 in d

      :raises ValueError: if ``key`` is not comparable with instances of its type.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

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

      :raises RuntimeError: if no key-value pairs have been inserted yet.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d["foo"]

      :raises TypeError: if ``type(key)`` does not match the type of the first key inserted.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         d[100]

      :raises ValueError: if ``key`` is not comparable with instances of its type.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d[1.1] = ("racecar",)
         d[float("nan")]

      :raises KeyError: if ``key`` is not present in the sorted dictionary.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

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

      :raises TypeError: if no key-value pairs have been inserted yet and ``type(key)`` isn't one of the supported
       types.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d[["eggs"]] = None

      :raises TypeError: if ``type(key)`` does not match the type of the first key inserted.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         d[100] = "spam"

      :raises ValueError: if ``key`` is not comparable with instances of its type.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d[1.1] = ("racecar",)
         d[float("nan")] = {}

      .. raw:: html

         </details>

   .. method:: __detitem__(key: Any)

      Remove ``key`` and the value mapped to it from the sorted dictionary.

      .. raw:: html

         <details class="warning">

         <summary>This method may raise exceptions.</summary>

      :raises RuntimeError: if no key-value pairs have been inserted yet.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         del d["foo"]

      :raises TypeError: if ``type(key)`` does not match the type of the first key inserted.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         del d[100]

      :raises ValueError: if ``key`` is not comparable with instances of its type.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d[1.1] = ("racecar",)
         del d[float("nan")]

      :raises KeyError: if ``key`` is not present in the sorted dictionary.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         del d["spam"]

      :raises RuntimeError: if there exists a forward iterator over the items, keys or values of the sorted
       dictionary pointing to ``key`` (meaning that calling ``next`` on the iterator would return ``(key, d[key])``,
       ``key`` or ``d[key]`` respectively).

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

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

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

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

         from pysorteddict import *

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

         from pysorteddict import *

         d = SortedDict()
         d["foo"] = ()
         d["bar"] = [100]
         d["baz"] = 3.14
         for key in d:
             print(key)

   .. method:: __reversed__() -> SortedDictKeysRevIter

      Return a reverse iterator over the keys in the sorted dictionary. ``reversed(d)`` is an efficient shorthand for
      ``reversed(d.keys())``.

      .. jupyter-execute::

         from pysorteddict import *

         d = SortedDict()
         d["foo"] = ()
         d["bar"] = [100]
         d["baz"] = 3.14
         for key in reversed(d):
             print(key)

   .. method:: clear()

      Remove all key-value pairs in the sorted dictionary.

      .. raw:: html

         <details class="warning">

         <summary>This method may raise exceptions.</summary>

      :raises RuntimeError: if there exist unexhausted iterators over the items, keys or values of the sorted
       dictionary.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

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

         from pysorteddict import *

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

         from pysorteddict import *

         d = SortedDict()
         d["foo"] = "bar"
         assert d.get("foo") == "bar"
         assert d.get("baz") is None
         assert d.get("spam", "eggs") == "eggs"

      .. raw:: html

         <details class="warning">

         <summary>This method may raise exceptions.</summary>

      :raises RuntimeError: if no key-value pairs have been inserted yet.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d.get("foo")

      :raises TypeError: if ``type(key)`` does not match the type of the first key inserted.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         d.get(100)

      :raises ValueError: if ``key`` is not comparable with instances of its type.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d[1.1] = ("racecar",)
         d.get(float("nan"))

      .. raw:: html

         </details>

   .. method:: items() -> SortedDictItems

      Return a dynamic view on the items in the sorted dictionary.

      .. jupyter-execute::

         from pysorteddict import *

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

         from pysorteddict import *

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

         from pysorteddict import *

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

      :raises RuntimeError: if no key-value pairs have been inserted yet.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d.setdefault("foo")

      :raises TypeError: if ``type(key)`` does not match the type of the first key inserted.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d["foo"] = ("bar", "baz")
         d.setdefault(100)

      :raises ValueError: if ``key`` is not comparable with instances of its type.

      .. jupyter-execute::
         :raises:

         from pysorteddict import *

         d = SortedDict()
         d[1.1] = ("racecar",)
         d.setdefault(float("nan"))

      .. raw:: html

         </details>

   .. method:: values() -> SortedDictValues

      Return a dynamic view on the values in the sorted dictionary.

      .. jupyter-execute::

         from pysorteddict import *

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

There are three view types, none of which are importable or user-constructible, since they are exposed only indirectly.

.. class:: SortedDictItems

   A view representing a sorted set of key-value pairs; the return type of :meth:`SortedDict.items`.

   .. method:: __contains__(ob: Any) -> bool

      Return whether ``ob`` is present in the sorted dictionary view.

      If ``ob`` is not a two-element ``tuple``, return ``False``. Otherwise, the behaviour is equivalent to that of
      ``ob[0] in d and d[ob[0]] == ob[1]`` where ``d`` is the underlying sorted dictionary.

.. class:: SortedDictKeys

   A view representing a sorted set of keys; the return type of :meth:`SortedDict.keys`.

   .. method:: __contains__(ob: Any) -> bool

      Return whether ``ob`` is present in the sorted dictionary view.

      The behaviour is equivalent to that of ``ob in d`` where ``d`` is the underlying sorted dictionary.

.. class:: SortedDictValues

   A view representing an array of values ordered by the keys they are mapped to; the return type of
   :meth:`SortedDict.values`.

   .. method:: __contains__(ob: Any) -> bool

      Return whether ``ob`` is present in the sorted dictionary view.

      The behaviour is equivalent to that of ``ob in l`` where ``l`` is a ``list`` of the elements in the view in the
      same order. In other words, making this call leads to an element-by-element comparison.
