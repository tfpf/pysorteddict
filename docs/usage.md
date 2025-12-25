# Usage

## Basic Usage

All keys in a sorted dictionary must be of the same type, which is determined when the first key-value pair is inserted
into it. The values, though, can be of any type.

```python
from pysorteddict import SortedDict

d = SortedDict()
d["honestly"] = "weight"
d["gain is"] = 31.692
d["times"] = "easier than"
d["losing"] = ["weight"]

assert d.key_type is str

for key, value in d.items():
    print(key, "->", value)
```

The above Python script will output the keys in ascending order.

```text
gain is -> 31.692
honestly -> weight
losing -> ['weight']
times -> easier than
```

## Demo

You can try pysorteddict out in the JupyterLite REPL below. When you click on 'Try Now', it will start a Pyodide kernel
and run some code to install pysorteddict and import it. This will take a few seconds.

<div class="only-light">

```{replite}
---
execute: True
height: 600px
kernel: python
prompt: Try Now
theme: JupyterLab Light
toolbar: 1
width: 100%
---
%pip install /pysorteddict/_static/wheels/pysorteddict-0.13.0-cp313-cp313-pyodide_2025_0_wasm32.whl

from pysorteddict import SortedDict

d = SortedDict()
```

</div>

<div class="only-dark">

```{replite}
---
execute: True
height: 600px
kernel: python
prompt: Try Now
theme: JupyterLab Dark
toolbar: 1
width: 100%
---
%pip install /pysorteddict/_static/wheels/pysorteddict-0.13.0-cp313-cp313-pyodide_2025_0_wasm32.whl

from pysorteddict import SortedDict

d = SortedDict()
```

</div>
