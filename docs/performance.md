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

Average execution times of some expressions are tabulated against the lengths of the sorted dictionaries used.

| Expression         | &emsp;10<sup>2</sup>&emsp; | &emsp;10<sup>3</sup>&emsp; | &emsp;10<sup>4</sup>&emsp; | &emsp;10<sup>5</sup>&emsp; | &emsp;10<sup>6</sup>&emsp; | &emsp;10<sup>7</sup>&emsp; |
| :--------:         | :------------------------: | :------------------------: | :------------------------: | :------------------------: | :------------------------: | :------------------------: |
| `0.00 in d`        |                            |                            |                            |                            |                            |                            |
| `0.33 in d`        |                            |                            |                            |                            |                            |                            |
| `0.67 in d`        |                            |                            |                            |                            |                            |                            |
| `1.00 in d`        |                            |                            |                            |                            |                            |                            |
| `for _ in d: pass` |                            |                            |                            |                            |                            |                            |

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
