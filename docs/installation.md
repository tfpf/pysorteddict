# Installation

pysorteddict requires Python 3.10 or newer.

## Linux on Desktop, macOS and Windows

Binary wheels for CPython and PyPy are provided on PyPI for most 64-bit Linux, macOS and Windows hosts.

```shell
pip install pysorteddict
```

[View the project on PyPI.](https://pypi.org/project/pysorteddict/)

## Linux on Raspberry Pi

Wheels for the Raspberry Pi family of computers can be found on piwheels.

```shell
pip install -i https://www.piwheels.org/simple pysorteddict
```

[View the project on piwheels.](https://www.piwheels.org/project/pysorteddict/)

## Emscripten/WebAssembly

Pyodide wheels (compiled to WebAssembly) are available here.

* [pysorteddict-0.13.0-cp312-cp312-pyodide_2024_0_wasm32.whl](_static/wheels/pysorteddict-0.13.0-cp312-cp312-pyodide_2024_0_wasm32.whl)
* [pysorteddict-0.13.0-cp313-cp313-pyodide_2025_0_wasm32.whl](_static/wheels/pysorteddict-0.13.0-cp313-cp313-pyodide_2025_0_wasm32.whl)

To download Pyodide wheels for an older version, check the corresponding GitHub release.

[View all releases on GitHub.](https://github.com/tfpf/pysorteddict/releases)



<div class="notice">
If there are no wheels available for your platform, running the above installation command will result in compilation
from source. For this to be successful, a C++20 compiler must be available, and the Python development headers and
libraries must be installed. This has been tested only with Clang, GCC and MSVC.
</div>
