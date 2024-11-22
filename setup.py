import platform

import setuptools

setuptools.setup(
    ext_modules=[
        setuptools.Extension(
            name="pysorteddict",
            extra_compile_args=["-std=c++20"],
            sources=["src/pysorteddict/pysorteddict.cc"],
            py_limited_api=True,
        )
    ]
)
