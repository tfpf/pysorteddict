# Performance

## Goals

pysorteddict was performance-benchmarked in order to:

* evaluate it under synthetic workloads targeting the specific features it provides; and
* understand how well the underlying data structure (typically a red-black tree) handles those workloads.

Nonetheless, the results should still be broadly indicative of real-world performance.

## Environment

| Component                      | Specification                              |
| :----------------------------: | :----------------------------------------: |
| CPU                            | Intel Core i9-12900H                       |
| CPU Frequency Scaling Governor | powersave                                  |
| RAM                            | 16 GiB DDR5                                |
| Kernel                         | Linux 6.12.74 (64-bit)                     |
| Operating System               | Debian 13 "trixie"                         |
| Operating System Libraries     | GNU C Library 2.41, GNU C++ Library 14.2.0 |
| Python Interpreter             | CPython 3.13.5                             |
| Python Interpreter Libraries   | pysorteddict 0.15.1                        |

## Strategy

The key type chosen was `float`, since it is easy to generate floating-point numbers uniformly distributed in the unit
interval. Comparing two `float`s is straightforward (as opposed to comparing, say, two `str`s—if their lengths are
different, they may introduce noise in the benchmarks). Before every benchmark, the random number generator was seeded
with _π_, a nothing-up-my-sleeve number.

<div class="notice">
The performance benchmarking code is in a Jupyter notebook in the GitHub repository. It contains everything required to
generate the data and graphs on this page.
</div>

## Results

### Memory

The C++ sorted dictionary does not expose public methods to estimate its memory usage. A practical workaround is to
check the resident set size of the Python process before and after creating a sorted dictionary. However, the
difference between these number is only an estimate of the actual memory usage, because it includes the space required
by all objects Python may simultaneously create, and also because the operating system may reuse memory released by
deleted objects, resulting in no memory spike if the sorted dictionary is small.

:::{image} _static/images/perf-memory-light.svg
:align: center
:class: only-light
:width: 100%
:::

:::{image} _static/images/perf-memory-dark.svg
:align: center
:class: only-dark
:width: 100%
:::

### Lookup

The numbers 0.00, 0.33, 0.67 and 1.00 are spaced equally in the range spanned by the keys, but are absent in the sorted
dictionaries constructed using the seeded random number generator described above. Hence, a search for them in the
red-black tree backing any `pysorteddict.SortedDict` will not terminate permaturely.

:::{image} _static/images/perf-contains-light.svg
:align: center
:class: only-light
:width: 100%
:::

:::{image} _static/images/perf-contains-dark.svg
:align: center
:class: only-dark
:width: 100%
:::

### Insertion and Deletion

Inserting or deleting an item into or from a sorted dictionary changes its length. Hence, benchmarks which only insert
or only delete items cannot be said to have been performed on a sorted dictionary of a particular length. Therefore,
the strategy chosen was:

* generate a `list` of random `float`s;
* insert all of them into the sorted dictionary; and
* delete all of them from the sorted dictionary in order of insertion.

Only the last two steps (defined in a function `set_del`) were timed. After these, ideally, the sorted dictionary
should return to the original state, allowing it to be used for the next round of timing. In practice, it is likely to
be in a different state because of rebalancing operations. But that change of state can be assumed to simulate the
real-world effects of insertions and deletions, so this is a sound strategy.

This benchmark was repeated for four different lengths of the `list` of random `float`s: 100, 200, 300 and 400.

:::{image} _static/images/perf-setitem-light.svg
:align: center
:class: only-light
:width: 100%
:::

:::{image} _static/images/perf-setitem-dark.svg
:align: center
:class: only-dark
:width: 100%
:::

### Batch Insertion and Deletion

Extending the logic of the previous benchmark, the strategy here was:

* generate a `list` of `tuple`s of random `float`s and `None`;
* update the sorted dictionary with them; and
* clear the sorted dictionary.

In effect, this benchmark indicates the time taken to populate and empty a sorted dictionary.

:::{image} _static/images/perf-update_clear-light.svg
:align: center
:class: only-light
:width: 100%
:::

:::{image} _static/images/perf-update_clear-dark.svg
:align: center
:class: only-dark
:width: 100%
:::

### Iteration

:::{image} _static/images/perf-iter-light.svg
:align: center
:class: only-light
:width: 100%
:::

:::{image} _static/images/perf-iter-dark.svg
:align: center
:class: only-dark
:width: 100%
:::

## Data

The benchmark data used to plot the above graphs is tabulated below.

```{eval-rst}
.. table::
   :widths: 3 1 1 1 1 1 1

   +--------------------------------+-----------------------------------------------------------------------------------------------------+
   | Expression                     | Sorted Dictionary Length                                                                            |
   |                                +----------------+----------------+----------------+----------------+----------------+----------------+
   |                                | 10\ :sup:`2`   | 10\ :sup:`3`   | 10\ :sup:`4`   | 10\ :sup:`5`   | 10\ :sup:`6`   | 10\ :sup:`7`   |
   +================================+================+================+================+================+================+================+
   | ``setup(…)``                   | 0 B            | 8.00 KiB       | 508 KiB        | 11.6 MiB       | 121 MiB        | 1.16 GiB       |
   +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------------+
   | ``0.00 in d``                  | 37.6 ns        | 50.2 ns        | 67.5 ns        | 83.6 ns        | 95.1 ns        | 115 ns         |
   +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------------+
   | ``0.33 in d``                  | 42.9 ns        | 61.1 ns        | 66.4 ns        | 80.3 ns        | 95.8 ns        | 112 ns         |
   +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------------+
   | ``0.67 in d``                  | 37.7 ns        | 56.1 ns        | 65.1 ns        | 73.8 ns        | 95.8 ns        | 113 ns         |
   +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------------+
   | ``1.00 in d``                  | 29.0 ns        | 57.0 ns        | 58.3 ns        | 77.1 ns        | 84.9 ns        | 107 ns         |
   +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------------+
   | ``set_del(d, keys_100)``       | 12.5 μs        | 17.0 μs        | 23.8 μs        | 33.0 μs        | 46.2 μs        | 65.0 μs        |
   +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------------+
   | ``set_del(d, keys_200)``       | 29.2 μs        | 42.2 μs        | 59.1 μs        | 83.0 μs        | 124 μs         | 206 μs         |
   +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------------+
   | ``set_del(d, keys_300)``       | 52.0 μs        | 70.6 μs        | 94.7 μs        | 148 μs         | 246 μs         | 395 μs         |
   +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------------+
   | ``set_del(d, keys_400)``       | 76.8 μs        | 98.8 μs        | 131 μs         | 198 μs         | 365 μs         | 528 μs         |
   +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------------+
   | ``update_clear(d, items)``     | 6.92 μs        | 127 μs         | 1.88 ms        | 30.8 ms        | 1.04 s         | 20.6 s         |
   +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------------+
   | ``for _ in d: pass``           | 615 ns         | 6.26 μs        | 106 μs         | 1.85 ms        | 103 ms         | 1.31 s         |
   +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------------+
   | ``for _ in reversed(d): pass`` | 834 ns         | 8.21 μs        | 133 μs         | 2.15 ms        | 111 ms         | 1.36 s         |
   +--------------------------------+----------------+----------------+----------------+----------------+----------------+----------------+
```
