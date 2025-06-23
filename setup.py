import sysconfig
from glob import glob

from setuptools import Extension, setup

# Only non-static (i.e. platform-specific) configurations go here.
cc = sysconfig.get_config_var("CC")
if cc and "msvc" in cc:
    extra_compile_args = ["/GL", "/std:c++20"]
    extra_link_args = ["/LTCG"]
else:
    extra_compile_args = ["-flto", "-std=c++20"]
    extra_link_args = ["-flto"]
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
