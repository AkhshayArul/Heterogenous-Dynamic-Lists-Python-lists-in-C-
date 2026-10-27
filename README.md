# Heterogeneous Dynamic Lists (Python Lists in C++)

## Overview

Implementation of Python-like lists in C++ that allow heterogeneous data type storage.

## Implementation

The implementation uses a vector of std::any objects and an ElementProxy class to return the stored data, which can then be resolved by the user into the appropriate data type .

This method allows the user to access data using the following syntax:

```cpp
int a = Pylist[index].getValue<int>();
```

## Usage

### Requirements

- A C++17-compatible compiler
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

The installation directory will contain the library, headers, and CMake package files required by other CMake projects.

### 5. Use the Library

Include the public header and create a `PythonList` object:

```cpp
#include <PythonList>
#include <iostream>

int main() {
    PythonList list;

    list.append(42);
    list.append(3.14);
    list.append("Hello, World!");

    std::cout << list << '\n';

    return 0;
}
```

Example output:

```text
[42, 3.14, Hello, World!]
```

The library supports heterogeneous elements, allowing different data types to be stored in the same list.

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

Replace `<installation-directory>` with the path to the directory where `PythonList` was installed. For example, if the installation directory is `install` inside the library repository, use its absolute path.

Build and run the consumer:

```bash
cmake --build build
```

The consumer project must use a compatible compiler and toolchain for the installed library.


## Requirements

Before using this project, ensure you have the following:

- *C++17-compatible compiler:* Required for features such as std::any.
- *C++ standard library:* A standard library implementation that supports C++17.
- *Terminal or command-line interface:* To compile and run the program.

No external libraries are required; the project uses the C++ standard library.

## Documentation

For further information on the detailed plan and thought processes, refer to the Documentation folder.
