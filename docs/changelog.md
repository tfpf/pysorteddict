# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project adheres to
[Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.13.0](https://github.com/tfpf/pysorteddict/compare/v0.12.1...v0.13.0)

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/224">#224</a> Provide built distributions for the GIL-enabled
  build of Python 3.14.</li>
  <li><a href="https://github.com/tfpf/pysorteddict/pull/230">#230</a> Support <code>datetime.date</code>,
  <code>datetime.timedelta</code>, <code>fractions.Fraction</code>, <code>ipaddress.IPv4Address</code>,
  <code>ipaddress.IPv4Interface</code>, <code>ipaddress.IPv4Network</code>, <code>ipaddress.IPv6Address</code>,
  <code>ipaddress.IPv6Interface</code>, <code>ipaddress.IPv6Network</code>, <code>pathlib.PosixPath</code>,
  <code>pathlib.PurePosixPath</code>, <code>pathlib.PureWindowsPath</code>, <code>pathlib.WindowsPath</code>,
  <code>time.struct_time</code> and <code>uuid.UUID</code> keys.</li>
  <li><a href="https://github.com/tfpf/pysorteddict/pull/235">#235</a> Provide built distributions for the GIL-disabled
  build of Python 3.14 on Linux and macOS.</li>
  <li><a href="https://github.com/tfpf/pysorteddict/pull/234">#234</a> Provide a built distribution for the
  GIL-disabled build of Python 3.14 on Windows.</li>
  <li><a href="https://github.com/tfpf/pysorteddict/pull/239">#239</a> Provide built distributions for the Pyodide
  build of Python 3.12 and 3.13.</li>
</ul>

## [0.12.1](https://github.com/tfpf/pysorteddict/compare/v0.12.0...v0.12.1)

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/219">#219</a> Allow using a sorted dictionary even if
  importing <code>decimal.Decimal</code> fails.</li>
</ul>

## [0.12.0](https://github.com/tfpf/pysorteddict/compare/v0.11.0...v0.12.0)

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/210">#210</a> Define
  <code>SortedDictItems.__reversed__</code>. Define <code>SortedDictItemsRevIter</code> as a reverse iterator over the
  values of a sorted dictionary. Define <code>SortedDictItemsRevIter.__next__</code>. Define
  <code>SortedDictKeys.__reversed__</code> and <code>SortedDict.__reversed__</code>. Define
  <code>SortedDictKeysRevIter</code> as a reverse iterator over the keys of a sorted dictionary. Define
  <code>SortedDictKeysRevIter.__next__</code>. Define <code>SortedDictValues.__reversed__</code>. Define
  <code>SortedDictValuesRevIter</code> as a reverse iterator over the values of a sorted dictionary. Define
  <code>SortedDictValuesRevIter.__next__</code>.</li>
</ul>

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/205">#205</a> Add homepage build dependencies to project
  metadata.</li>
</ul>

<ul class="change-break">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/211">#211</a> Rename <code>SortedDictItemsIter</code> to
  <code>SortedDictItemsFwdIter</code>, <code>SortedDictKeysIter</code> to <code>SortedDictKeysFwdIter</code> and
  <code>SortedDictValuesIter</code> to <code>SortedDictValuesFwdIter</code>.</li>
</ul>

## [0.11.0](https://github.com/tfpf/pysorteddict/compare/v0.10.0...v0.11.0)

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/196">#196</a> Define <code>SortedDict.get</code> to return the
  value mapped to a key or a specified default.</li>
  <li><a href="https://github.com/tfpf/pysorteddict/pull/198">#198</a> Define <code>SortedDict.setdefault</code> to
  return the value mapped to a key or a specified default and map that default to the key.</li>
</ul>

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/commit/8fe14d796917482f830af5921d5b18546a9d7843">8fe14d796917</a>
  Add optional testing dependencies to project metadata.</li>
  <li><a href="https://github.com/tfpf/pysorteddict/pull/197">#197</a> Define
  <code>SortedDictItems.__contains__</code>.</li>
</ul>

## [0.10.0](https://github.com/tfpf/pysorteddict/compare/v0.9.0...v0.10.0)

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/184">#184</a> Define <code>SortedDictValues</code> as a view
  over the values of a sorted dictionary. Define <code>SortedDictValues.__repr__</code>,
  <code>SortedDictValues.__len__</code>, <code>SortedDictValues.__getitem__</code> and
  <code>SortedDictValues.__iter__</code>. Define <code>SortedDictValuesIter</code> as an iterator over the values of a
  sorted dictionary. Define <code>SortedDictValuesIter.__iter__</code> and <code>SortedDictValuesIter.__next__</code>.
  Update <code>SortedDict.values</code> to return the aforementioned view.</li>
  <li><a href="https://github.com/tfpf/pysorteddict/pull/187">#187</a> Define <code>SortedDictItems</code> as a view
  over the items of a sorted dictionary. Define <code>SortedDictItems.__repr__</code>,
  <code>SortedDictItems.__len__</code>, <code>SortedDictItems.__getitem__</code> and
  <code>SortedDictItems.__iter__</code>. Define <code>SortedDictItemsIter</code> as an iterator over the items of a
  sorted dictionary. Define <code>SortedDictItemsIter.__iter__</code> and <code>SortedDictItemsIter.__next__</code>.
  Update <code>SortedDict.items</code> to return the aforementioned view.</li>
</ul>

## [0.9.0](https://github.com/tfpf/pysorteddict/compare/v0.8.2...v0.9.0)

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/179">#179</a> Define <code>SortedDict.__iter__</code>.</li>
</ul>

## [0.8.2](https://github.com/tfpf/pysorteddict/compare/v0.8.1...v0.8.2)

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/175">#175</a> Update <code>SortedDictKeys.__getitem__</code>
  to speed up the creation of a positive slice near the end or a negative slice near the beginning.</li>
</ul>

## [0.8.1](https://github.com/tfpf/pysorteddict/compare/v0.8.0...v0.8.1)

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/169">#169</a> Update <code>SortedDictKeys.__getitem__</code>
  to raise the same errors <code>list.__getitem__</code> raises.</li>
</ul>

## [0.8.0](https://github.com/tfpf/pysorteddict/compare/v0.7.3...v0.8.0)

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/163">#163</a> Define
  <code>SortedDictKeys.__getitem__</code>.</li>
</ul>

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/164">#164</a> Update <code>SortedDict.__len__</code> to raise
  <code>OverflowError</code> if the length exceeds the maximum value a <code>Py_ssize_t</code> can have.</li>
</ul>

## [0.7.3](https://github.com/tfpf/pysorteddict/compare/v0.7.2...v0.7.3)

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/149">#149</a> Exclude test files from the source distribution.
  Exclude source files from the built distributions.</li>
  <li><a href="https://github.com/tfpf/pysorteddict/pull/157">#157</a> Define
  <code>SortedDictKeys.__contains__</code>.</li>
</ul>

## [0.7.2](https://github.com/tfpf/pysorteddict/compare/v0.7.1...v0.7.2)

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/141">#141</a> Add changelog link to project metadata.</li>
</ul>

## [0.7.1](https://github.com/tfpf/pysorteddict/compare/v0.7.0...v0.7.1)

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/131">#131</a> Allow most modifications to a sorted dictionary
  while iterating over its keys. Prohibit deleting the key any iterator is about to yield next. Prohibit clearing the
  sorted dictionary when any unexhausted iterators exist.</li>
</ul>

## [0.7.0](https://github.com/tfpf/pysorteddict/compare/v0.6.0...v0.7.0)

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/120">#120</a> Define <code>SortedDictKeys</code> as a view
  over the keys of a sorted dictionary. Define <code>SortedDictKeys.__repr__</code>,
  <code>SortedDictKeys.__len__</code> and <code>SortedDictKeys.__iter__</code>. Define <code>SortedDictKeysIter</code>
  as an iterator over the keys of a sorted dictionary. Define <code>SortedDictKeysIter.__iter__</code> and
  <code>SortedDictKeysIter.__next__</code>. Update <code>SortedDict.keys</code> to return the aforementioned view.
  Prohibit modifications to a sorted dictionary while iterating over its keys.</li>
</ul>

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
  subclasses of supported types as unsupported ([#22](https://github.com/tfpf/pysorteddict/pull/22)).

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
