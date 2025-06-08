# Changelog

## 0.7.1

<ul class="change-fix">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/131">#131</a> Allow most modifications to a sorted dictionary
  while iterating over its keys. Prohibit deleting the key any iterator is about to yield next. Prohibit clearing the
  sorted dictionary when any unexhausted iterators exist.</li>
</ul>

## 0.7.0

<ul class="change-new">
  <li><a href="https://github.com/tfpf/pysorteddict/pull/120">#120</a> Define sorted dictionary views and iterators.
  Update <code>SortedDict.keys</code> to return said view. Prohibit modifications to a sorted dictionary while
  iterating over its keys.</li>
</ul>

## 0.6.0

<ul class="change-break">
  <li><a href="https://github.com/tfpf/pysorteddict/commit/84830cb2d98afa28a1be188f42da5938080a5435">84830cb2d98a</a>
  Update the check for the import of <code>decimal.Decimal</code> with type verification. Change the error message to
  reflect that importing the type failed.</li>
  <li><a href="https://github.com/tfpf/pysorteddict/pull/118">#118</a> Update <code>SortedDict.__contains__</code> to
  raise the same exceptions <code>SortedDict.__getitem__</code> raises (excluding <code>KeyError</code>) instead of
  silently returning <code>False</code>. (This is consistent with the behaviour of <code>dict.__contains__</code>,
  which raises <code>TypeError</code> for unhashable types.)</li>
</ul>
