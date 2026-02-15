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
pip install --extra-index-url https://www.piwheels.org/simple pysorteddict
```

[View the project on piwheels.](https://www.piwheels.org/project/pysorteddict/)

## Emscripten

Pyodide wheels for the Emscripten runtime (targeting WebAssembly) are hosted here.

<a href="simple/pysorteddict/">View all available Pyodide wheels.</a>

<div class="notice">
If there are no wheels available for your platform, attempting installation will result in compilation from source. For
this to be successful, a C++20 compiler must be available, and the Python development headers and libraries must be
installed. This has been tested only with Clang, GCC and MSVC.
</div>
