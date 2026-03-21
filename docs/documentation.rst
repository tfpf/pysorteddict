Documentation
=============

.. default-domain:: py

.. py:module:: pysorteddict

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
            print(d.key_type)
            d[b"foo"] = ()
            print(d.key_type)

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
