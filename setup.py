import glob

import setuptools

setuptools.setup(
    ext_modules=[
        setuptools.Extension(
            "pysorteddict",
            glob.glob("src/pysorteddict/*.cc"),
        )
    ]
)
