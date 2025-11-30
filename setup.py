import glob
import os
import sysconfig

import setuptools

# Only non-static (i.e. platform-specific) configurations go here.
if os.name != "nt" or sysconfig.get_platform().startswith("mingw"):
    extra_compile_args = ["-flto", "-std=c++20"]
else:
    extra_compile_args = ["/GL", "/std:c++20"]
setuptools.setup(
    ext_modules=[
        setuptools.Extension(
            "pysorteddict",
            glob.glob("src/pysorteddict/*.cc"),
            extra_compile_args=extra_compile_args,
        )
    ]
)
