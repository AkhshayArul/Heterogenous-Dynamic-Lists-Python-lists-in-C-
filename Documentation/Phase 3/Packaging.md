# Phase 3 — Packaging and Distribution

## Objective

The objective of Phase 3 is to make `PythonList` usable as a standalone C++ library, allowing other projects to include and use it without directly compiling its source files.

## Build System

CMake is used to configure and build the project. The root `CMakeLists.txt` defines the `PythonList` library target, configures its public include directories, and specifies C++17 as the required language standard.

Regression tests are also integrated into the CMake build system using CTest.

## Installation

CMake installation rules install the compiled library, public headers, and package configuration files into the specified installation directory.

The public header `PythonList` allows users to include the library using:

```cpp
#include <PythonList>
```

## Package Configuration

The project exports the `PythonList::PythonList` CMake target. The installed package configuration allows other CMake projects to locate the library using `find_package()` and link against the exported target.

## Verification

The following steps were verified:

- The library builds successfully using CMake.
- The regression tests pass.
- The library and public headers install successfully.
- A separate consumer project can locate, link, and use the installed library.

The consumer project successfully appended an integer and a string to a `PythonList` and printed the contents.

## Conclusion

The library can now be built, installed, and consumed by another CMake project without directly including its implementation source file.