# Usage

## Basic Usage

All keys in a sorted dictionary must be of the same type, which is determined when the first key-value pair is inserted
into it. The values, though, can be of any type.

:::{jupyter-execute}
from pysorteddict import SortedDict

d = SortedDict()
d["honestly"] = "weight"
d["gain is"] = 31.692
d["times"] = "easier than"
d["losing"] = ["weight"]

assert d.key_type is str

for key, value in d.items():
    print(key, "->", value)
:::

The keys are seen to be output in ascending order.

## Demo

You can try pysorteddict out in the JupyterLite REPL below. When you click on 'Try Now', it will start a Pyodide kernel
and run some code to install pysorteddict (using one of the hosted wheels) and import it. This will take a few seconds.

<div class="only-light">

:::{replite}
:height: 600px
:kernel: python
:prompt: Try Now
:theme: JupyterLab Light
:toolbar: 1
:width: 100%

import micropip

await micropip.install(
    "pysorteddict", index_urls=["https://tfpf.github.io/pysorteddict/simple/"]
)

from pysorteddict import SortedDict

d = SortedDict()
:::

</div>

<div class="only-dark">

:::{replite}
:height: 600px
:kernel: python
:prompt: Try Now
:theme: JupyterLab Dark
:toolbar: 1
:width: 100%

import micropip

await micropip.install(
    "pysorteddict", index_urls=["https://tfpf.github.io/pysorteddict/simple/"]
)

from pysorteddict import SortedDict

d = SortedDict()
:::

</div>
