# Usage

All keys in a sorted dictionary must be of the same type, which is determined when the first key-value pair is inserted
into it. The values, though, can be of any type.

```python
import json

from pysorteddict import SortedDict

sorted_dict = SortedDict()
sorted_dict["honestly"] = "weight"
sorted_dict["gain is"] = 31.692
sorted_dict["times"] = "easier than"
sorted_dict["losing"] = ["weight"]
assert sorted_dict.key_type is str
print(json.dumps(sorted_dict, indent=2, sort_keys=False))
```

The above Python script will output the keys in ascending order.

```json
{
  "gain is": 31.692,
  "honestly": "weight",
  "losing": [
    "weight"
  ],
  "times": "easier than"
}
```

The following key types are supported.

* `bytes`
* `float`
* `int`
* `str`
