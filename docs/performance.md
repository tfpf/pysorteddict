# Performance

## Goals

pysorteddict was performance-benchmarked in order to:

* evaluate it under workloads resembling real applications; and
* see where it stands in comparison to the sorted dictionary implementation of Sorted Containers.

Sorted Containers is a mature Python library which has seen use in popular open-source projects, and is thus an
appropriate yardstick to measure pysorteddict against.

<div class="notice">
pysorteddict and Sorted Containers differ greatly in their sorted dictionary implementations.

* `pysorteddict.SortedDict` is typically a red-black tree—it is faster (by microseconds) for writes.
* `sortedcontainers.sorteddict.SortedDict` is typically a hash table and a sorted set of keys—it is faster (by
  nanoseconds) for reads.
</div>

## Environment

| Component                      | Specification                                |
| :----------------------------: | :------------------------------------------: |
| CPU                            | Intel Core i9-12900H                         |
| CPU Frequency Scaling Governor | powersave                                    |
| RAM                            | 16 GiB DDR5                                  |
| Kernel                         | Linux 6.1.0 (64-bit)                         |
| Operating System               | Debian 12 "bookworm"                         |
| Operating System Libraries     | GNU C Library 2.36, GNU C++ Library 12.2.0   |
| Python Interpreter             | CPython 3.11.2                               |
| Python Interpreter Libraries   | pysorteddict 0.13.0, Sorted Containers 2.4.0 |

## Strategy

The key type chosen was `float`, since it is easy to generate floating-point numbers uniformly distributed in the unit
interval. Comparing two `float`s is straightforward (as opposed to comparing, say, two `str`s—if their lengths are
different, they may introduce noise in the benchmarks). Before every benchmark, the random number generator was seeded
with _π_, a nothing-up-my-sleeve number.

<div class="notice">
The performance benchmarking code is in a Jupyter notebook in the GitHub repository. It contains everything required to
generate the data and graphs on this page.
</div>

## Overview

The average execution times of some expressions are tabulated against the lengths of the `pysorteddict.SortedDict`s
used.

```{eval-rst}
.. table::
   :widths: 6 1 1 1 1 1 1 1 1

   +--------------------------------+----------------------------------------------------------------------------------------------------------+
   | Expression                     | ``pysorteddict.SortedDict`` Length                                                                       |
   |                                +---------------------+----------------+----------------+----------------+----------------+----------------+
   |                                | 10\ :sup:`2`        | 10\ :sup:`3`   | 10\ :sup:`4`   | 10\ :sup:`5`   | 10\ :sup:`6`   | 10\ :sup:`7`   |
   |                                +------+------+-------+----------------+----------------+----------------+----------------+----------------+
   |                                | ◆    | ▪    | Unit  | 10\ :sup:`3`   | 10\ :sup:`4`   | 10\ :sup:`5`   | 10\ :sup:`6`   | 10\ :sup:`7`   |
   +================================+======+======+=======+================+================+================+================+================+
   | ``0.00 in d``    asd           | 35.3 | 20.2 | ns    | 47.4 ns        | 62.8 ns        | 81.1 ns        | 91.4 ns        | 102 ns         |
   +--------------------------------+------+------+-------+----------------+----------------+----------------+----------------+----------------+
   | ``0.33 in d``                  | 42.9 | 19.7 | ns    | 59.8 ns        | 66.8 ns        | 82.8 ns        | 100 ns         | 114 ns         |
   +--------------------------------+------+------+-------+----------------+----------------+----------------+----------------+----------------+
   | ``0.67 in d``                  | 38.8 | 21.5 | ns    | 55.6 ns        | 67.3 ns        | 76.2 ns        | 98.5 ns        | 115 ns         |
   +--------------------------------+------+------+-------+----------------+----------------+----------------+----------------+----------------+
   | ``1.00 in d``                  | 29.2 | 20.4 | ns    | 56.8 ns        | 60.6 ns        | 79.7 ns        | 87.1 ns        | 111 ns         |
   +--------------------------------+------+------+-------+----------------+----------------+----------------+----------------+----------------+
   | ``set_del(d, keys_33)``        | 3.97 μs             | 4.94 μs        | 5.93 μs        | 6.94 μs        | 7.86 μs        | 9.39 μs        |
   +--------------------------------+---------------------+----------------+----------------+----------------+----------------+----------------+
   | ``set_del(d, keys_67)``        | 8.33 μs             | 10.1 μs        | 12.6 μs        | 15.5 μs        | 20.8 μs        | 29.9 μs        |
   +--------------------------------+---------------------+----------------+----------------+----------------+----------------+----------------+
   | ``set_del(d, keys_100)``       | 12.7 μs             | 15.6 μs        | 21.4 μs        | 28.6 μs        | 41.0 μs        | 60.2 μs        |
   +--------------------------------+---------------------+----------------+----------------+----------------+----------------+----------------+
   | ``for _ in d: pass``           | 590 ns              | 6.26 μs        | 104 μs         | 2.17 ms        | 111 ms         | 1.34 s         |
   +--------------------------------+---------------------+----------------+----------------+----------------+----------------+----------------+
   | ``for _ in reversed(d): pass`` | 873 ns              | 8.71 μs        | 130 μs         | 2.42 ms        | 114 ms         | 1.32 s         |
   +--------------------------------+---------------------+----------------+----------------+----------------+----------------+----------------+
```

## Details

### Membership Check

The numbers 0.00, 0.33, 0.67 and 1.00 are spaced equally in the range spanned by the keys, but are absent in the sorted
dictionaries constructed using the seeded random number generator described above. Hence, a search for them in any of
those sorted dictionaries will not terminate permaturely.

```{image} _static/images/perf-contains-light.svg
:align: center
:class: only-light
:width: 100%
```

```{image} _static/images/perf-contains-dark.svg
:align: center
:class: only-dark
:width: 100%
```

Since `sortedcontainers.sorteddict.SortedDict` looks up keys in a hash table in constant time, its performance is
independent of the length of the sorted dictionary.

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

This benchmark was repeated for three different lengths of the `list` of random `float`s: 33, 67 and 100.

```{image} _static/images/perf-setitem-light.svg
:align: center
:class: only-light
:width: 100%
```

```{image} _static/images/perf-setitem-dark.svg
:align: center
:class: only-dark
:width: 100%
```

Since `pysorteddict.SortedDict` inserts and deletes keys from a red-black tree in logarithmic time, it is much faster
at mutating data.

### Iteration

```{image} _static/images/perf-iter-light.svg
:align: center
:class: only-light
:width: 100%
```

```{image} _static/images/perf-iter-dark.svg
:align: center
:class: only-dark
:width: 100%
```

Since `pysorteddict.SortedDict` does a lot of bookkeeping to allow mutation during iteration, it is slower at
iterating.
