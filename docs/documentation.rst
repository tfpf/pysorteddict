Documentation
=============

.. default-domain:: py

.. py:module:: pysorteddict

   .. class:: SortedDict

      Sorted dictionary which wraps the C++ sorted dictionary (``std::map``): key-value pairs are stored in ascending
      order of the keys, which must all be of the same type.

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

      .. method:: __init__()

         Initialise an empty sorted dictionary.

      .. property:: key_type
         :type: type | None

         The key type of the sorted dictionary, or ``None`` if no key-value pairs have been inserted in it.

         .. code-block:: python

            from pysorteddict import *
            d = SortedDict()
            assert d.key_type is None
            d[b"foo"] = ()
            assert d.key_type is bytes

      .. method:: __repr__()

         Return a human-readable representation of the sorted dictionary.

      .. method:: __contains__(key: Any) -> bool

         Return whether ``key`` is present in the sorted dictionary.

         :raises RuntimeError: if no key-value pairs have been inserted yet.

         .. jupyter-execute::
            :raises:

            from pysorteddict import *
            d = SortedDict()
            "foo" in d

         :raises RuntimeError:

