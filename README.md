# Heterogeneous Dynamic Lists (Python Lists in C++)

## Overview

Implementation of Python-like lists in C++ that allow heterogeneous data type storage.

## Implementation

The implementation uses a vector of `std::any` objects and an `ElementProxy` class to return the stored data, which can then be resolved by the user into the appropriate data type.

This method allows the user to access data using the following syntax:

```cpp
int a = Pylist[index].getValue<int>();
```

## Usage

The end product is an includable library that can be used as follows:

```cpp
#include <PythonList>
```

## Documentation

For further information on the detailed plan and thought processes, refer to the `Documentation` folder.