import os
import sysconfig

import setuptools

if os.name != "nt" or sysconfig.get_platform().startswith("mingw"):
    # Either one of Linux, macOS and Cygwin, or MSYS2.
    cxx_standard_arg = "-std=c++14"
else:
    # Windows.
    cxx_standard_arg = "/std:c++14"

setuptools.setup(
    ext_modules=[
        setuptools.Extension(
            name="pysorteddict",
            extra_compile_args=[cxx_standard_arg],
            sources=["src/pysorteddict/pysorteddict.cc"],
            py_limited_api=True,
        )
    ]
)
