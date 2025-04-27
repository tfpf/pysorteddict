#! /usr/bin/env python3

import json

from pysorteddict import SortedDict

sorted_dict = SortedDict(int)
sorted_dict[5659] = "gaining weight is"
sorted_dict[1992] = 31.692
sorted_dict[24274] = "times easier than"
sorted_dict[9765] = ["losing", "weight"]
print(json.dumps(sorted_dict, indent=2))
