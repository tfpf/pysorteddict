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
  iterating over its keys.
</ul>
