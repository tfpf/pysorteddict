import glob
import platform
import sysconfig

import setuptools

if (system := platform.system()) == "Darwin":
    # Clang doesn't know how to resolve template specialisations when explicit
    # instantiations are provided.
    sysconfig.get_config_vars().update({"CC": "gcc-12", "CXX": "g++-12"})

# Only non-static (i.e. platform-specific) configurations go here.
if system != "Windows" or sysconfig.get_platform().startswith("mingw"):
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
