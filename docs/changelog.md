# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project adheres to
[Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.13.1](https://github.com/tfpf/pysorteddict/compare/v0.13.0...v0.13.1) (2026-02-16)

### Fixed

* `SortedDictItemsRevIter`, `SortedDictKeysRevIter` and `SortedDictValuesRevIter` do not crash the program when the
  key-value pair they were referencing is erased ([#260](https://github.com/tfpf/pysorteddict/pull/260)).

## [0.13.0](https://github.com/tfpf/pysorteddict/compare/v0.12.1...v0.13.0) (2025-12-23)

### Added

* `datetime.date`, `datetime.timedelta`, `fractions.Fraction`, `ipaddress.IPv4Address`, `ipaddress.IPv4Interface`,
  `ipaddress.IPv4Network`, `ipaddress.IPv6Address`, `ipaddress.IPv6Interface`, `ipaddress.IPv6Network`,
  `pathlib.PosixPath`, `pathlib.PurePosixPath`, `pathlib.PureWindowsPath`, `pathlib.WindowsPath`, `time.struct_time`
  and `uuid.UUID` as supported key types ([#230](https://github.com/tfpf/pysorteddict/pull/230)).

## [0.12.1](https://github.com/tfpf/pysorteddict/compare/v0.12.0...v0.12.1) (2025-10-22)

### Fixed

* `SortedDict` constructor never raises `ImportError`; if importing a supported key type fails, it is silently demoted
  to an unsupported key type ([#219](https://github.com/tfpf/pysorteddict/pull/219)).

## [0.12.0](https://github.com/tfpf/pysorteddict/compare/v0.11.0...v0.12.0) (2025-10-19)

### Added

* `SortedDictItems` method `__reversed__`; `SortedDictItemsRevIter` and method `__next__`; `SortedDict` method
  `__reversed__`; `SortedDictKeys` method `__reversed__`; `SortedDictKeysRevIter` and method `__next__`;
  `SortedDictValues` method `__reversed__`; and `SortedDictValuesRevIter` and method `__next__`
  ([#210](https://github.com/tfpf/pysorteddict/pull/210)).
  * `SortedDict` method `__delitem__` raises `RuntimeError` if there exists an unexhausted reverse iterator which just
    yielded the key to be deleted.

### Changed

* `SortedDictItemsIter` is renamed to `SortedDictItemsFwdIter`; `SortedDictKeysIter` is renamed to
  `SortedDictKeysFwdIter`; and `SortedDictValuesIter` is renamed to `SortedDictValuesFwdIter`
  ([#211](https://github.com/tfpf/pysorteddict/pull/211)).

## [0.11.0](https://github.com/tfpf/pysorteddict/compare/v0.10.0...v0.11.0) (2025-07-20)

### Added

* `SortedDict` method `get` ([#196](https://github.com/tfpf/pysorteddict/pull/196)).
* `SortedDictItems` method `__contains__` ([#197](https://github.com/tfpf/pysorteddict/pull/197)).
* `SortedDict` method `setdefault` ([#198](https://github.com/tfpf/pysorteddict/pull/198)).

## [0.10.0](https://github.com/tfpf/pysorteddict/compare/v0.9.0...v0.10.0) (2025-07-13)

### Added

* `SortedDictValues` and methods `__repr__`, `__len__`, `__getitem__` and `__iter__`; and `SortedDictValuesIter` and
  method `__next__` ([#184](https://github.com/tfpf/pysorteddict/pull/184)).
* `SortedDictItems` and methods `__repr__`, `__len__`, `__getitem__` and `__iter__`; and `SortedDictItemsIter` and
  method `__next__` ([#187](https://github.com/tfpf/pysorteddict/pull/187)).

### Changed

* `SortedDict` method `values` returns a view (`SortedDictValues`) instead of a list
  ([#184](https://github.com/tfpf/pysorteddict/pull/184)).
* `SortedDict` method `items` returns a view (`SortedDictItems`) instead of a list
  ([#187](https://github.com/tfpf/pysorteddict/pull/187)).

## [0.9.0](https://github.com/tfpf/pysorteddict/compare/v0.8.2...v0.9.0) (2025-07-05)

### Added

* `SortedDict` method `__iter__` ([#179](https://github.com/tfpf/pysorteddict/pull/179)).

## [0.8.2](https://github.com/tfpf/pysorteddict/compare/v0.8.1...v0.8.2) (2025-07-04)

### Changed

* `SortedDictKeys` method `__getitem__` creates positive-step slices near the end and negative-step slices near the
  beginning faster ([#175](https://github.com/tfpf/pysorteddict/pull/175)).

## [0.8.1](https://github.com/tfpf/pysorteddict/compare/v0.8.0...v0.8.1) (2025-07-04)

### Fixed

* `SortedDictKeys` method `__getitem__` raises the same exceptions as `list.__getitem__`
  ([#169](https://github.com/tfpf/pysorteddict/pull/169)).

## [0.8.0](https://github.com/tfpf/pysorteddict/compare/v0.7.3...v0.8.0) (2025-07-02)

### Added

* `SortedDictKeys` method `__getitem__` ([#163](https://github.com/tfpf/pysorteddict/pull/163)).

### Changed

* `SortedDict` method `__len__` raises `OverflowError` if its length exceeds `PY_SSIZE_T_MAX`
  ([#164](https://github.com/tfpf/pysorteddict/pull/164)).

## [0.7.3](https://github.com/tfpf/pysorteddict/compare/v0.7.2...v0.7.3) (2025-06-22)

### Added

* `SortedDictKeys` method `__contains__` ([#157](https://github.com/tfpf/pysorteddict/pull/157)).

## [0.7.2](https://github.com/tfpf/pysorteddict/compare/v0.7.1...v0.7.2) (2025-06-08)

No user-facing changes of note.

## [0.7.1](https://github.com/tfpf/pysorteddict/compare/v0.7.0...v0.7.1) (2025-06-05)

### Fixed

* `SortedDict` method `__setitem__` never raises `RuntimeError`; `SortedDict` method `__delitem__` raises
  `RuntimeError` if there exists an iterator about to yield the key to be deleted; `SortedDict` method `clear` raises
  `RuntimeError` if there exists any unexhausted iterator over it
  ([#131](https://github.com/tfpf/pysorteddict/pull/131)).

## [0.7.0](https://github.com/tfpf/pysorteddict/compare/v0.6.0...v0.7.0) (2025-05-25)

### Added

* `SortedDictKeys` and methods `__repr__`, `__len__` and `__iter__`; and `SortedDictKeysIter` and method `__next__`
  ([#120](https://github.com/tfpf/pysorteddict/pull/120)).
  * `SortedDict` methods `__setitem__`, `__delitem__` and `clear` raise `RuntimeError` if called while iterating over
    a view.

### Changed

* `SortedDict` method `keys` returns a view (`SortedDictKeys`) instead of a list
  ([#120](https://github.com/tfpf/pysorteddict/pull/120)).

## [0.6.0](https://github.com/tfpf/pysorteddict/compare/v0.5.3...v0.6.0) (2025-05-18)

### Changed

* `SortedDict` constructor checks whether the supported key type it imported is actually a type, and describes the
  problem more clearly in the exception message in case of failure
  ([84830cb2d98a](https://github.com/tfpf/pysorteddict/commit/84830cb2d98afa28a1be188f42da5938080a5435)).
* `SortedDict` method `__contains__` raises the same exceptions as `__getitem__` (except `KeyError`) instead of
  silently returning `False` in case of errors ([#118](https://github.com/tfpf/pysorteddict/pull/118)).

## [0.5.3](https://github.com/tfpf/pysorteddict/compare/v0.5.2...v0.5.3) (2025-05-17)

No user-facing changes of note.

## [0.5.2](https://github.com/tfpf/pysorteddict/compare/v0.5.1...v0.5.2) (2025-05-17)

### Fixed

* `SortedDict` constructor always raises `ImportError` upon failure to import supported key types which are not
  built-in instead of relying on the exception raised by the import machinery
  ([#111](https://github.com/tfpf/pysorteddict/pull/111)).

## [0.5.1](https://github.com/tfpf/pysorteddict/compare/v0.5.0...v0.5.1) (2025-05-17)

No user-facing changes of note.

## [0.5.0](https://github.com/tfpf/pysorteddict/compare/v0.4.6...v0.5.0) (2025-05-16)

### Added

* `decimal.Decimal` as a supported key type ([#102](https://github.com/tfpf/pysorteddict/pull/102)).

## [0.4.6](https://github.com/tfpf/pysorteddict/compare/v0.4.5...v0.4.6) (2025-05-15)

No user-facing changes of note.

## [0.4.5](https://github.com/tfpf/pysorteddict/compare/v0.4.4...v0.4.5) (2025-05-14)

### Fixed

* `SortedDict` methods `__delitem__` and `__setitem__` raise `RuntimeError` instead of `ValueError` when the key type
  is not set ([#95](https://github.com/tfpf/pysorteddict/pull/95)).

## [0.4.4](https://github.com/tfpf/pysorteddict/compare/v0.4.3...v0.4.4) (2025-05-11)

### Changed

* Indexing a `SortedDict` instance with invalid keys shows the key and its type in the exception message
  ([#91](https://github.com/tfpf/pysorteddict/pull/91)).

## [0.4.3](https://github.com/tfpf/pysorteddict/compare/v0.4.2...v0.4.3) (2025-05-11)

No user-facing changes of note.

## [0.4.2](https://github.com/tfpf/pysorteddict/compare/v0.4.1...v0.4.2) (2025-05-07)

No user-facing changes of note.

## [0.4.1](https://github.com/tfpf/pysorteddict/compare/v0.4.0...v0.4.1) (2025-05-01)

No user-facing changes of note.

## [0.4.0](https://github.com/tfpf/pysorteddict/compare/v0.3.0...v0.4.0) (2025-05-01)

### Added

* `float` as a supported key type ([#67](https://github.com/tfpf/pysorteddict/pull/67)).
* `SortedDict` property `key_type` ([#71](https://github.com/tfpf/pysorteddict/pull/71)).

## [0.3.0](https://github.com/tfpf/pysorteddict/compare/v0.2.1...v0.3.0) (2025-04-27)

### Added

* `SortedDict` method `__repr__` ([#49](https://github.com/tfpf/pysorteddict/pull/49)).
* `bytes` and `str` as supported key types ([#55](https://github.com/tfpf/pysorteddict/pull/55)).

### Changed

* `SortedDict` constructor ignores all arguments; `SortedDict` method `__setitem__` sets the key type upon succeeding
  for the first time ([#47](https://github.com/tfpf/pysorteddict/pull/47)).

### Removed

* `SortedDict` method `__str__` ([#49](https://github.com/tfpf/pysorteddict/pull/49)).

## [0.2.1](https://github.com/tfpf/pysorteddict/compare/v0.2.0...v0.2.1) (2024-12-14)

No user-facing changes of note.

## [0.2.0](https://github.com/tfpf/pysorteddict/compare/v0.1.0...v0.2.0) (2024-12-14)

### Added

* `SortedDict` method `copy` ([#36](https://github.com/tfpf/pysorteddict/pull/36)).

## [0.1.0](https://github.com/tfpf/pysorteddict/compare/v0.0.8...v0.1.0) (2024-12-01)

### Added

* `SortedDict` method `__contains__` ([#29](https://github.com/tfpf/pysorteddict/pull/29)).
* `SortedDict` method `clear` ([#31](https://github.com/tfpf/pysorteddict/pull/31)).

## [0.0.8](https://github.com/tfpf/pysorteddict/compare/v0.0.7...v0.0.8) (2024-11-28)

### Fixed

* `SortedDict` constructor raises `TypeError` instead of `ValueError` when it receives an unsupported type, and treats
  subclasses of supported types as unsupported; `SortedDict` method `__setitem__` rejects instances of subclasses of
  the key type ([#22](https://github.com/tfpf/pysorteddict/pull/22)).

## [0.0.7](https://github.com/tfpf/pysorteddict/compare/v0.0.6...v0.0.7) (2024-11-27)

No user-facing changes of note.

## [0.0.6](https://github.com/tfpf/pysorteddict/compare/v0.0.5...v0.0.6) (2024-11-24)

No user-facing changes of note.

## [0.0.5](https://github.com/tfpf/pysorteddict/compare/v0.0.4...v0.0.5) (2024-11-23)

No user-facing changes of note.

## 0.0.4 (2024-11-23)

### Added

* `SortedDict` and its constructor accepting the key type, which must be `int`
  ([8ef0310913b4](https://github.com/tfpf/pysorteddict/commit/8ef0310913b47b1539b6524d0cf94424825c0a38)).
* `SortedDict` destructor ([#1](https://github.com/tfpf/pysorteddict/pull/1)).
* `SortedDict` methods `__len__`, `__getitem__`, `__setitem__`, `__delitem__` and `__str__`
  ([#2](https://github.com/tfpf/pysorteddict/pull/2)).
  * `__setitem__` rejects a key if it is not a subclass of what was passed to the constructor.
* `SortedDict` methods `items`, `keys` and `values` ([#3](https://github.com/tfpf/pysorteddict/pull/3)).
  * All three return lists, not views.
