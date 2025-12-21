# Usage

All keys in a sorted dictionary must be of the same type, which is determined when the first key-value pair is inserted
into it. The values, though, can be of any type.

```python
from pysorteddict import SortedDict

sorted_dict = SortedDict()
sorted_dict["honestly"] = "weight"
sorted_dict["gain is"] = 31.692
sorted_dict["times"] = "easier than"
sorted_dict["losing"] = ["weight"]

assert sorted_dict.key_type is str

for key, value in sorted_dict.items():
    print(key, "->", value)
```

The above Python script will output the keys in ascending order.

```text
gain is -> 31.692
honestly -> weight
losing -> ['weight']
times -> easier than
```

You can try it out in the JupyterLite notebook below.

<div class="only-light">

```{replite}
---
execute: False
height: 600px
kernel: python
showbanner: 0
theme: JupyterLab Light
toolbar: 1
width: 100%
---
print(1)
```

</div>

<div class="only-dark">

```{replite}
---
execute: False
height: 600px
kernel: python
showbanner: 0
theme: JupyterLab Dark
toolbar: 1
width: 100%
---
print(1)
```

</div>
