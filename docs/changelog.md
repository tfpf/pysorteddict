# Changelog

## [0.7.1](https://github.com/tfpf/pysorteddict/compare/v0.7.0...v0.7.1)

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/131">#131</a> Allow most modifications to a sorted dictionary
  while iterating over its keys. Prohibit deleting the key any iterator is about to yield next. Prohibit clearing the
  sorted dictionary when any unexhausted iterators exist.</li>
</ul>

## [0.7.0](https://github.com/tfpf/pysorteddict/compare/v0.6.0...v0.7.0)

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/120">#120</a> Define sorted dictionary views and iterators.
  Update <code>SortedDict.keys</code> to return a view. Prohibit modifications to a sorted dictionary while iterating
  over its keys.</li>
</ul>

## [0.6.0](https://github.com/tfpf/pysorteddict/compare/v0.5.3...v0.6.0)

<ul class="change-break">
  <li><a href="https://github.com/tfpf/pysorteddict/commit/84830cb2d98afa28a1be188f42da5938080a5435">84830cb2d98a</a>
  Update the check for the import of <code>decimal.Decimal</code> with type verification. Change the error message to
  reflect that importing the type failed.</li>
  <li><a href="https://github.com/tfpf/pysorteddict/pull/118">#118</a> Update <code>SortedDict.__contains__</code> to
  raise the same exceptions <code>SortedDict.__getitem__</code> raises (excluding <code>KeyError</code>) instead of
  silently returning <code>False</code>. (This is consistent with the behaviour of <code>dict.__contains__</code>,
  which raises <code>TypeError</code> for unhashable types.)</li>
</ul>

## [0.5.3](https://github.com/tfpf/pysorteddict/compare/v0.5.2...v0.5.3)

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/commit/61c6d76c88c2849bed2c092afe2aeb7349bd33bf">61c6d76c88c2</a>
  Add GitHub links to project metadata.</li>
</ul>

## [0.5.2](https://github.com/tfpf/pysorteddict/compare/v0.5.1...v0.5.2)

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/commit/6bf7bbdef8cbc83d64d57e2eb50b2c3a5146a63b">6bf7bbdef8cb</a>
  Remove GitHub links from project metadata.</li>
  <li><a href="https://github.com/tfpf/pysorteddict/pull/111">#111</a> Change the error message about failure to import
  Python types which are not built-in.</li>
</ul>

## [0.5.1](https://github.com/tfpf/pysorteddict/compare/v0.5.0...v0.5.1)

No user-facing changes of note.

## [0.5.0](https://github.com/tfpf/pysorteddict/compare/v0.4.6...v0.5.0)

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/102">#102</a> Support <code>decimal.Decimal</code> keys.</li>
</ul>

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/commit/8c145e38eb98d623a640521ec996d49cc1af90fd">8c145e38eb98</a>
  Add GitHub links to project metadata.</li>
</ul>

## [0.4.6](https://github.com/tfpf/pysorteddict/compare/v0.4.5...v0.4.6)

No user-facing changes of note.

## [0.4.5](https://github.com/tfpf/pysorteddict/compare/v0.4.4...v0.4.5)

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/95">#95</a> Change the type of the error raised when the key
  type is not set from <code>ValueError</code> to <code>RuntimeError</code>.</li>
</ul>

## [0.4.4](https://github.com/tfpf/pysorteddict/compare/v0.4.3...v0.4.4)

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/91">#91</a> Change error messages to provide actionable
  information.</li>
</ul>

## [0.4.3](https://github.com/tfpf/pysorteddict/compare/v0.4.2...v0.4.3)

No user-facing changes of note.

## [0.4.2](https://github.com/tfpf/pysorteddict/compare/v0.4.1...v0.4.2)

No user-facing changes of note.

## [0.4.1](https://github.com/tfpf/pysorteddict/compare/v0.4.0...v0.4.1)

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/75">#75</a> Add C++ header files from the project to the
  source and built distributions.</li>
</ul>

## [0.4.0](https://github.com/tfpf/pysorteddict/compare/v0.3.0...v0.4.0)

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/67">#67</a> Support <code>float</code> keys.</li>
  <li><a href="https://github.com/tfpf/pysorteddict/pull/71">#71</a> Define <code>SortedDict.key_type</code> to query
  the key type of a sorted dictionary.</li>
</ul>

## [0.3.0](https://github.com/tfpf/pysorteddict/compare/v0.2.1...v0.3.0)

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/45">#45</a> Define a cleanup function for sorted dictionaries
  to prevent memory leaks.</li>
</ul>

<ul class="change-break">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/47">#47</a> Update the sorted dictionary constructor to ignore
  all arguments. Update <code>SortedDict.__setitem__</code> to set the key type upon succeeding for the first
  time.</li>
</ul>

## [0.2.1](https://github.com/tfpf/pysorteddict/compare/v0.2.0...v0.2.1)

No user-facing changes of note.

## [0.2.0](https://github.com/tfpf/pysorteddict/compare/v0.1.0...v0.2.0)

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/36">#36</a> Define <code>SortedDict.copy</code> to return a
  shallow copy.</li>
</ul>

## [0.1.0](https://github.com/tfpf/pysorteddict/compare/v0.0.8...v0.1.0)

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/29">#29</a> Define <code>SortedDict.__contains__</code>.</li>
  <li><a href="https://github.com/tfpf/pysorteddict/pull/31">#31</a> Define <code>SortedDict.clear</code> to remove all
  key-value pairs from a sorted dictionary.</li>
</ul>
