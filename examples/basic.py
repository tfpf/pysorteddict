#! /usr/bin/env python3

import json

from pysorteddict import SortedDict

sorted_dict = SortedDict()
sorted_dict["honestly"] = "weight"
sorted_dict["gain is"] = 31.692
sorted_dict["times"] = "easier than"
sorted_dict["losing"] = ["weight"]

assert sorted_dict.key_type is str

print(json.dumps(sorted_dict, indent=2, sort_keys=False))
