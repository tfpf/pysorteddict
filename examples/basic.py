#! /usr/bin/env python3

from pysorteddict import SortedDict

sd = SortedDict(int)
sd[1] = "anda"
sd[2] = "gundu"
print(sd)
print(sd[1])
del sd
