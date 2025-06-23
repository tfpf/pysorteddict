import os
import sysconfig
from glob import glob

from setuptools import Extension, setup

# Only non-static (i.e. platform-specific) configurations go here.
if os.name != "nt" or sysconfig.get_platform().startswith("mingw"):
    extra_compile_args = ["-flto", "-std=c++20"]
    extra_link_args = ["-flto"]
else:
    extra_compile_args = ["/GL", "/std:c++20"]
    extra_link_args = ["/LTCG"]
setup(
    ext_modules=[
        Extension(
            "pysorteddict",
            glob("src/pysorteddict/*.cc"),
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args,
        )
    ]
)
