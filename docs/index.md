# pysorteddict

Enriches Python with `SortedDict`, a sorted dictionary: a dictionary in which the keys are always in ascending order.
They are not sorted when queried; they are genuinely stored such that iterating over them yields a monotonically
increasing sequence. Needless to say, a given `SortedDict` instance only admits keys of a single type with a strict
ordering defined.

pysorteddict is implemented entirely in C++. `SortedDict` provides a Python interface to `std::map`.

:::{toctree}
:hidden:

installation
usage
documentation
performance
changelog
development
:::

<!--
Putting an HTML tag in a Markdown link containing an image is probably not a
good idea. Nesting inline attributes (a MyST Parser extension currently in
beta) probably isn't, either. The HTML document is generated exactly as
desired, though.
-->
[
[[![installation](_static/images/installation.svg)<br>Installation](installation)]{.card}
[[![usage](_static/images/usage.svg)<br>Usage](usage)]{.card}
[[![documentation](_static/images/documentation.svg)<br>Documentation](documentation)]{.card}
[[![performance](_static/images/performance.svg)<br>Performance](performance)]{.card}
[[![changelog](_static/images/changelog.svg)<br>Changelog](changelog)]{.card}
[[![development](_static/images/development.svg)<br>Development](development)]{.card}
]{.card-container}

## Why pysorteddict?

There are many sorted dictionary implementations for Python. Foremost among them is Sorted Containers, a mature library
which has seen use in real-world applications. So why use pysorteddict over Sorted Containers (or any of the others)?

pysorteddict has some rather attractive features which Sorted Containers does not have.

### Robust

Computations are relegated to the robust C++ sorted dictionary.

### Strongly Typed

Keys are not automatically converted between compatible types. For instance, although `0 == 0.0` in Python, a sorted
dictionary with integer keys will reject `0.0`.

### Strict

Conceptually different keys are considered different. For instance, although `0 == False` in Python, a sorted
dictionary with integer keys will not fetch the value mapped to `0` when queried with `False`.

### Correct

NaN is unconditionally rejected as a key.

### Stable Under Mutation During Iteration

Modifications to a sorted dictionary are allowed while iterating over it, and the results are well-defined.


