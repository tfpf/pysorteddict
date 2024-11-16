import setuptools

setuptools.setup(
    ext_modules=[
        setuptools.Extension(name="pysorteddict", sources=["src/pysorteddict/pysorteddict.cc"], py_limited_api=True)
    ]
)
