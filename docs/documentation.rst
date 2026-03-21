Documentation
=============

.. default-domain:: py

.. py:module:: pysorteddict

   .. class:: SortedDict

      Sorted dictionary. Key-value pairs are stored in ascending order of the keys, which must all be of the same type.

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

      .. method:: __init__

      .. property:: key_type
         :type: type | None
