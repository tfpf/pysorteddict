#! /usr/bin/env python3

from pysorteddict import SortedDict

sd = SortedDict(int)

sd[100] = "wut"
sd[1] = "ok"
sd[1] = "notok"
print(sd)
print(sd[1])
del sd
