import glob
import os
import sysconfig

import setuptools

if os.name != "nt":
    # Clang doesn't know how to resolve template specialisations when explicit
    # instantiations are provided.
    sysconfig.get_config_vars().update({"CC": "gcc", "CXX": "g++"})

# Only non-static (i.e. platform-specific) configurations go here.
if os.name != "nt" or sysconfig.get_platform().startswith("mingw"):
    extra_compile_args = ["-flto", "-std=c++20"]
    extra_link_args = ["-flto"]
else:
    extra_compile_args = ["/GL", "/std:c++20"]
    extra_link_args = ["/LTCG"]
setuptools.setup(
    ext_modules=[
        setuptools.Extension(
            "pysorteddict",
            glob.glob("src/pysorteddict/*.cc"),
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args,
        )
    ]
)
