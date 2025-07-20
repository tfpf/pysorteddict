# Performance

The table below describes the environment the performance benchmarks were run in.

| Component                      | Specification                              |
| :----------------------------: | :----------------------------------------: |
| CPU                            | Intel Core i9-12900H                       |
| CPU Frequency Scaling Governor | powersave                                  |
| RAM                            | 16 GiB DDR4                                |
| Kernel                         | Linux 6.1.0 (64-bit)                       |
| Operating System               | Debian 12 "bookworm"                       |
| Operating System Libraries     | GNU C Library 2.36, GNU C++ Library 12.2.0 |
| Python Interpreter             | CPython 3.11.2                             |
| Python Interpreter Libraries   | Jupyter 1.1.1, pysorteddict 0.9.0          |

The key type chosen was `float`, since it is easy to generate floating-point numbers uniformly distributed in the unit
interval. Comparing two `float`s is straightforward (as opposed to comparing, say, two `str`s—if their lengths are
different, they may introduce noise in the benchmarks). Before every benchmark, the random number generator was seeded
with _π_, a nothing-up-my-sleeve number.

There is an extra step required when using `float` keys: the check for NaN. Hence, the performance will be marginally
worse than if `int` keys were used.

<div class="notice">
The performance benchmarking code is in a Jupyter notebook in the GitHub repository. It contains everything required to
run the benchmarks.
</div>

## Overview

The average execution times of some expressions are tabulated against the lengths of the sorted dictionaries used.

| Expression         | &emsp;10<sup>2</sup>&emsp; | &emsp;10<sup>3</sup>&emsp; | &emsp;10<sup>4</sup>&emsp; | &emsp;10<sup>5</sup>&emsp; | &emsp;10<sup>6</sup>&emsp; | &emsp;10<sup>7</sup>&emsp; |
| :--------:         | :------------------------: | :------------------------: | :------------------------: | :------------------------: | :------------------------: | :------------------------: |
| `0.00 in d`        | 36.8 ns                    | 48.9 ns                    | 64.6 ns                    | 83.1 ns                    | 94.0 ns                    | 106 ns                     |
| `0.33 in d`        | 44.8 ns                    | 59.8 ns                    | 67.6 ns                    | 84.2 ns                    | 100 ns                     | 113 ns                     |
| `0.67 in d`        | 39.7 ns                    | 56.1 ns                    | 67.7 ns                    | 76.5 ns                    | 97.4 ns                    | 114 ns                     |
| `1.00 in d`        | 31.8 ns                    | 60.3 ns                    | 64.5 ns                    | 84.6 ns                    | 92.2 ns                    | 116 ns                     |
| `for _ in d: pass` | 818 ns                     | 8.37 μs                    | 119 μs                     | 2.01 ms                    | 97.1 ms                    | 1.06 s                     |

## Details

### Membership Check

The numbers 0.00, 0.33, 0.67 and 1.00 are spaced equally in the range spanned by the keys, but are absent in the sorted
dictionaries constructed using the seeded random number generator described above. Hence, a search for them in any of
those sorted dictionaries will not terminate permaturely.

```{image} _static/images/perf-contains-light.svg
:align: center
:class: only-light
```

```{image} _static/images/perf-contains-dark.svg
:align: center
:class: only-dark
```

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
```

```{image} _static/images/perf-setitem-dark.svg
:align: center
:class: only-dark
```

### Iteration

```{image} _static/images/perf-iter-light.svg
:align: center
:class: only-light
```

```{image} _static/images/perf-iter-dark.svg
:align: center
:class: only-dark
```
