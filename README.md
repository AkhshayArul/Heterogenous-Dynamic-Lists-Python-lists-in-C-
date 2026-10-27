# Heterogeneous Dynamic Lists (Python Lists in C++)

## Overview

An implementation of Python-like lists in C++ that supports heterogeneous data storage, allowing elements of different data types to coexist in the same list.

## Implementation

`PythonList` uses `std::vector<std::any>` to store heterogeneous elements. A nested `ProxyElement` class provides access to stored elements through `operator[]`.

Elements can be retrieved using `getValue<T>()` or, where appropriate, implicit type conversion.

For example:

```cpp
PythonList list;
list.append(42);
list.append("Hello");

int number = list[0];
std::string message = list[1].getValue<std::string>();
```

The library provides Python-like list operations, including insertion, removal, popping, extending, negative indexing, and slicing.

The implementation uses C++17 features and the C++ standard library without requiring external libraries.

## Usage

### Requirements

- A C++17-compatible compiler and standard library
- [CMake](https://cmake.org/download/) 3.20 or later
- A compatible build toolchain

### 1. Clone the Repository

```bash
git clone https://github.com/AkhshayArul/Heterogenous-Dynamic-Lists-Python-lists-in-C-.git
cd Heterogenous-Dynamic-Lists-Python-lists-in-C-
```

### 2. Build the Library

Configure the project and generate the build files:

```bash
cmake -S . -B build
```

Build the library:

```bash
cmake --build build
```

If you are using MSYS2 UCRT64 with MinGW Makefiles, you can specify the generator explicitly:

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

Use the same generator for subsequent CMake commands on that build directory.

### 3. Run the Tests

Run the regression tests using CTest:

```bash
ctest --test-dir build --output-on-failure
```

### 4. Install the Library

Install the compiled library, public headers, and CMake package configuration files:

```bash
cmake --install build --prefix ./install
```

The installation prefix contains the library, public headers, and CMake package files needed by other CMake projects.

### 5. Use the Library

Include the public header and create a `PythonList` object:

```cpp
#include <PythonList>
#include <iostream>
#include <string>

int main() {
    PythonList list;

    list.append(42);
    list.append(3.14);
    list.append("Hello, World!");

    std::cout << list << '\n';

    int number = list[0];
    std::string message = list[2].getValue<std::string>();

    std::cout << number << '\n';
    std::cout << message << '\n';

    return 0;
}
```

Example output:

```text
[42, 3.14, Hello, World!]
42
Hello, World!
```

The list can store elements of different supported types, including integers, floating-point values, strings, and nested lists.

### 6. Integrate with Another CMake Project

After installing the library, another CMake project can discover and link against it.

In the consumer project's `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.20)

project(MyApplication LANGUAGES CXX)

find_package(PythonList CONFIG REQUIRED)

add_executable(MyApplication main.cpp)

target_link_libraries(MyApplication PRIVATE PythonList::PythonList)
```

Configure the consumer project by specifying the installation prefix:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="<installation-directory>"
```

Replace `<installation-directory>` with the path to the directory where `PythonList` was installed. For example, if you installed the library into the `install` directory inside the library repository, specify that directory's absolute path.

Build the consumer application:

```bash
cmake --build build
```

The consumer project must use a compatible compiler and toolchain for the installed library.

## Features

- Heterogeneous storage using `std::any`
- Element access through `operator[]` and `ProxyElement`
- Typed element retrieval using `getValue<T>()`
- Implicit conversion to supported element types
- Appending elements and references
- Insertion and removal of elements
- Pop-and-return operations
- Extending a list with another `PythonList`
- Checking whether a value exists
- Negative indexing
- List concatenation using `operator+` and `operator+=`
- Slicing
- Stream output using the library's printing mechanism
- Custom type-mismatch exception handling

## Documentation

For details about the design decisions, implementation process, and development phases, refer to the `Documentation` directory.