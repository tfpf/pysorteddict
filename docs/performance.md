# Performance

<div class="notice">This page is a work-in-progress.</div>

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

Everything required to run these performance benchmarks can be found in the GitHub repository.

## Overview

The average execution times of some expressions are tabulated against the lengths of the sorted dictionaries used.

| Expression         | &emsp;10<sup>2</sup>&emsp; | &emsp;10<sup>3</sup>&emsp; | &emsp;10<sup>4</sup>&emsp; | &emsp;10<sup>5</sup>&emsp; | &emsp;10<sup>6</sup>&emsp; | &emsp;10<sup>7</sup>&emsp; |
| :--------:         | :------------------------: | :------------------------: | :------------------------: | :------------------------: | :------------------------: | :------------------------: |
| `0.00 in d`        | 36.8 ns                    | 48.9 ns                    | 64.6 ns                    | 83.1 ns                    | 94.0 ns                    | 106 ns                     |
| `0.33 in d`        | 44.8 ns                    | 59.8 ns                    | 67.6 ns                    | 84.2 ns                    | 100 ns                     | 113 ns                     |
| `0.67 in d`        | 39.7 ns                    | 56.1 ns                    | 67.7 ns                    | 76.5 ns                    | 97.4 ns                    | 114 ns                     |
| `1.00 in d`        | 31.8 ns                    | 60.3 ns                    | 64.5 ns                    | 84.6 ns                    | 92.2 ns                    | 116 ns                     |
| `for _ in d: pass` | 875 ns                     | 9.40 μs                    | 129 μs                     | 1.98 ms                    | 96.1 ms                    | 1.08 s                     |

## Details

### Membership Check

```{image} _static/images/perf-__contains__-light.png
:align: center
:class: only-light
```

```{image} _static/images/perf-__contains__-dark.png
:align: center
:class: only-dark
```

### Iteration

```{image} _static/images/perf-__iter__-light.png
:align: center
:class: only-light
```

```{image} _static/images/perf-__iter__-dark.png
:align: center
:class: only-dark
```
