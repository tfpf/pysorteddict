# Installation

```shell
pip install pysorteddict
```

pysorteddict requires Python 3.10 or newer. It has been tested using CPython and PyPy, and binary wheels for the same
are provided on PyPI for most 64-bit Linux, macOS and Windows hosts.

[View the project on PyPI.](https://pypi.org/project/pysorteddict/)

Pyodide wheels for the latest version can be downloaded from here.

* [pysorteddict-0.13.0-cp312-cp312-pyodide_2024_0_wasm32.whl](_static/wheels/pysorteddict-0.13.0-cp312-cp312-pyodide_2024_0_wasm32.whl)
* [pysorteddict-0.13.0-cp313-cp313-pyodide_2025_0_wasm32.whl](_static/wheels/pysorteddict-0.13.0-cp313-cp313-pyodide_2025_0_wasm32.whl)

To download Pyodide wheels for an older version, check the corresponding GitHub release.

[View all releases on GitHub.](https://github.com/tfpf/pysorteddict/releases)

Wheels for the Raspberry Pi family of computers may be found on piwheels, but they might lag behind those on PyPI.

[View the project on piwheels.](https://www.piwheels.org/project/pysorteddict/)

The piwheels page is not controlled by me.

<div class="notice">
If there are no wheels available for your platform, running the above installation command will result in compilation
from source. For this to be successful, a C++20 compiler must be available, and the Python development headers and
libraries must be installed. This has been tested only with Clang, GCC and MSVC.
</div>
