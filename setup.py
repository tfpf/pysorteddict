import platform

import setuptools

setuptools.setup(
    ext_modules=[
        setuptools.Extension(
            name="pysorteddict",
            extra_compile_args=["/std:c++14" if platform.system() == "Windows" else "-std=c++11"],
            sources=["src/pysorteddict/pysorteddict.cc"],
            py_limited_api=True,
        )
    ]
)
