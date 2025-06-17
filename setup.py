import os
import sysconfig
from glob import glob

from setuptools import Extension, setup

# MSVC supports designated initialisers only in C++20.
cxx_standard_arg = "-std=c++14" if os.name != "nt" or sysconfig.get_platform().startswith("mingw") else "/std:c++20"

setup(
    ext_modules=[Extension("pysorteddict", glob("src/pysorteddict/*.cc"), extra_compile_args=[cxx_standard_arg])],
    include_package_data=False,
)
