#! /usr/bin/env python3

from pysorteddict import SortedDict

sd = SortedDict(int)
print(sd)
sd[1] = "anda"
print(sd[1])
del sd
