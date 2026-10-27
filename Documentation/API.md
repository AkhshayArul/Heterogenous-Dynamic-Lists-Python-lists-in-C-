# PythonList API Reference

This document describes the public API implemented by `PythonList` in `List.h` and `List.cpp`.

## Contents

- [Overview](#overview)
- [Requirements and inclusion](#requirements-and-inclusion)
- [Types and exceptions](#types-and-exceptions)
- [Class reference](#class-reference)
  - [Construction and storage](#construction-and-storage)
  - [Element access](#element-access)
  - [Adding elements](#adding-elements)
  - [Removing elements](#removing-elements)
  - [Queries and size management](#queries-and-size-management)
  - [Combining lists](#combining-lists)
  - [Slicing](#slicing)
  - [Printing](#printing)
- [Examples](#examples)
- [Behavior notes and limitations](#behavior-notes-and-limitations)

## Overview

`PythonList` is a C++17 class for storing heterogeneous values in one list. Its implementation uses `std::vector<std::any>` internally and provides operations inspired by Python lists, including negative indexing, insertion, removal, popping, concatenation, and slicing.

The library's public header is:

```cpp
#include <PythonList>
```

When compiling directly from the source tree, make sure the compiler's include path contains the project's `include` directory.

## Requirements and inclusion

- A C++17-compatible compiler and standard library.
- Include the public header using `#include <PythonList>` when using the installed library, or `#include "List.h"` when including the implementation header directly.

The installed library can be discovered from a CMake project using its exported target:

```cmake
find_package(PythonList CONFIG REQUIRED)
target_link_libraries(MyApplication PRIVATE PythonList::PythonList)
```

## Types and exceptions

### `PythonList`

The main container class. It stores values in insertion order and permits values of different types to coexist.

### `PythonList::ProxyElement`

A nested proxy returned by `PythonList::operator[]`. It refers to the selected stored `std::any`, enabling typed access, implicit conversion, and stream output.

A proxy refers to an element in the list; it is not an independent copy of the value. As with references or iterators into a `std::vector`, modifying the list in ways that reallocate or erase elements can invalidate existing proxies.

### `TypeMismatchError`

```cpp
class TypeMismatchError : public std::runtime_error;
```

Thrown by `ProxyElement::getValue<T>()` when the stored value is neither a `T` nor a `std::reference_wrapper<T>`.

### `UnsupportedOperationError`

```cpp
class UnsupportedOperationError : public std::runtime_error;
```

Thrown by `remove()` or `contains()` when the requested value's type does not support equality comparison with itself.

### `ArgumentNotPresentError`

```cpp
class ArgumentNotPresentError : public std::runtime_error;
```

Thrown by `remove()` when no matching value of the requested type is found.

Other operations can throw standard exceptions, as described below.

## Class reference

### Construction and storage

The provided header does not declare explicit constructors or a destructor. The compiler-generated special member functions are therefore used.

#### `append(T&& data)`

```cpp
template <typename T>
void append(T&& data);
```

Appends a value to the end of the list. The value is stored in `std::any`, with the template argument forwarded to the storage operation. The library registers a printer for the decayed type when that type supports stream insertion.

**Example**

```cpp
PythonList list;
list.append(42);
list.append(3.14);
list.append(std::string("hello"));
```

**Notes**

- `std::any` stores a value, so ordinary calls store their own value rather than an alias to the original variable.
- The stored type must be acceptable to `std::any` (in particular, it must meet `std::any`'s value-type requirements).
- String literals have a dedicated `append(const char*)` overload, which stores them as `std::string`.

#### `append(const char* data)`

```cpp
void append(const char* data);
```

Converts the C string to `std::string` and appends it. Registers the string printer.

**Example**

```cpp
list.append("hello");  // Stored as std::string
```

The pointer must refer to a valid null-terminated C string.

#### `appendReference(T& data)`

```cpp
template <typename T>
void appendReference(T& data);
```

Appends a `std::reference_wrapper<T>` referring to the supplied object instead of storing an ordinary copy of that object. If `T` is printable, a printer is registered for the stored reference-wrapper type.

**Example**

```cpp
int value = 10;
PythonList list;
list.appendReference(value);

int& stored = list[0].getValue<std::reference_wrapper<int>>().get();
stored = 20;  // Changes value
```

**Important:** the referenced object must outlive every use of the list element. This overload does not take ownership of the referenced object.

To retrieve the referenced object directly, the type requested from `getValue<T>()` must match the stored `std::reference_wrapper<T>` type. The current implicit conversion operator does not automatically unwrap reference wrappers.

### Element access

#### `operator[](std::ptrdiff_t index)`

```cpp
ProxyElement operator[](std::ptrdiff_t index);
```

Returns a proxy referring to the element at `index`. Negative indexes count backward from the end: `-1` selects the last element, `-2` the second-to-last, and so on.

**Example**

```cpp
PythonList list;
list.append(10);
list.append(20);
list.append(30);

int first = list[0];
int last = list[-1];
int secondLast = list[-2];
```

**Exceptions**

- Throws `std::out_of_range` if the adjusted index is outside the list.

This overload is non-`const`; the header does not provide a `const` indexing overload.

#### `PythonList::ProxyElement::getValue<T>()`

```cpp
template <typename T>
T& getValue();
```

Returns a mutable reference to the stored value when it is stored directly as `T`. It also supports retrieving a stored `std::reference_wrapper<T>` when `T` is the wrapper type itself.

**Example**

```cpp
PythonList list;
list.append(42);

int& value = list[0].getValue<int>();
value = 100;
```

**Exceptions**

- Throws `TypeMismatchError` if the requested type does not match the stored type.

The current implementation does not provide a `const` overload of `getValue<T>()`.

#### `PythonList::ProxyElement::operator T()`

```cpp
template <typename T>
operator T();
```

Allows the proxy to be converted to a requested type by calling `getValue<T>()`.

**Example**

```cpp
int number = list[0];
```

The requested type must match the stored type; this is not a general numeric conversion facility. A mismatch throws `TypeMismatchError`.

### Adding elements

#### `insert(std::ptrdiff_t index, T&& data)`

```cpp
template <typename T>
void insert(std::ptrdiff_t index, T&& data);
```

Inserts a value at the specified position.

Index handling:

- A negative index is adjusted by the current list size.
- If the adjusted negative index is still below zero, the value is inserted at the beginning.
- An index greater than the list size is clamped to the end.
- An index equal to the list size appends the value.

**Example**

```cpp
PythonList list;
list.append(10);
list.append(30);

list.insert(1, 20);  // [10, 20, 30]
list.insert(-1, 25); // Inserts before the last element
list.insert(100, 40); // Inserts at the end
```

The stored value's decayed type is used to construct the `std::any`. If the type supports stream insertion, its printer is registered.

#### `insert(std::ptrdiff_t index, const char* data)`

```cpp
void insert(std::ptrdiff_t index, const char* data);
```

Inserts the C string as a `std::string`, using the same index-clamping behavior as the templated overload.

### Removing elements

#### `pop(std::ptrdiff_t index = -1)`

```cpp
std::any pop(std::ptrdiff_t index = -1);
```

Removes and returns the element at the requested index. The default index `-1` removes the last element.

The returned object is an `std::any`; retrieve the value using `std::any_cast`.

**Example**

```cpp
PythonList list;
list.append(10);
list.append(20);

std::any removed = list.pop(); // Removes 20
int value = std::any_cast<int>(removed);
```

**Exceptions**

- Throws `std::out_of_range` if the list is empty.
- Throws `std::out_of_range` if the adjusted index is outside the list.

#### `remove(const T& value)`

```cpp
template <typename T>
void remove(const T& value);
```

Removes the first element whose stored type exactly matches the decayed type of `value` and whose value compares equal to `value`.

**Example**

```cpp
PythonList list;
list.append(10);
list.append(20);
list.append(10);

list.remove(10); // Removes the first int equal to 10
```

**Exceptions**

- Throws `UnsupportedOperationError` if the value type is not equality-comparable with itself.
- Throws `ArgumentNotPresentError` if no matching value of the same type is found.

This is exact-type matching: for example, an `int` search value does not match a stored `double`, even if the numeric values are equal.

#### `remove(const char* value)`

```cpp
void remove(const char* value);
```

Converts the C string to `std::string` and removes the first matching stored string. Throws `ArgumentNotPresentError` if no matching string is present.

### Queries and size management

#### `size() const`

```cpp
std::size_t size() const;
```

Returns the number of elements in the list.

#### `empty() const`

```cpp
bool empty() const;
```

Returns `true` if the list contains no elements; otherwise returns `false`.

#### `clear()`

```cpp
void clear();
```

Removes all elements from the list.

#### `contains(const T& value) const`

```cpp
template <typename T>
bool contains(const T& value) const;
```

Returns `true` if an element with the same decayed type and an equal value exists in the list. Returns `false` if no such element exists.

**Example**

```cpp
PythonList list;
list.append(42);

bool hasInt = list.contains(42);   // true
bool hasDouble = list.contains(42.0); // false if no matching double exists
```

**Exceptions**

- Throws `UnsupportedOperationError` if the requested type does not support equality comparison with itself.

#### `contains(const char* value) const`

```cpp
bool contains(const char* value) const;
```

Converts the C string to `std::string` and checks whether an equal stored string exists.

### Combining lists

#### `extend(const PythonList& other)`

```cpp
void extend(const PythonList& other);
```

Appends copies of all elements from `other` to the end of this list, preserving their order.

Self-extension is supported: extending a list with itself duplicates its current elements once.

**Example**

```cpp
PythonList first;
first.append(1);
first.append(2);

PythonList second;
second.append(3);

first.extend(second); // [1, 2, 3]
```

The operation copies the stored `std::any` elements. If an element contains a `std::reference_wrapper`, the wrapper is copied and still refers to the original object.

#### `operator+=(const PythonList& other)`

```cpp
PythonList& operator+=(const PythonList& other);
```

Appends the elements of `other` to the current list and returns a reference to the current list.

**Example**

```cpp
first += second;
```

#### `operator+(const PythonList& other) const`

```cpp
PythonList operator+(const PythonList& other) const;
```

Returns a new list containing copies of the left-hand list's elements followed by copies of the right-hand list's elements. The original lists are not modified.

**Example**

```cpp
PythonList combined = first + second;
```

### Slicing

#### `slice(std::ptrdiff_t start, std::ptrdiff_t stop, std::ptrdiff_t step = 1) const`

```cpp
PythonList slice(
    std::ptrdiff_t start,
    std::ptrdiff_t stop,
    std::ptrdiff_t step = 1
) const;
```

Returns a new `PythonList` containing elements selected from the half-open range determined by `start`, `stop`, and `step`. The `stop` position is excluded. Negative bounds are adjusted relative to the list size, and bounds are clamped according to the direction of the step.

- `step > 0`: selects positions while the index is less than `stop`.
- `step < 0`: selects positions while the index is greater than `stop`.
- `step == 0`: invalid.

**Examples**

```cpp
PythonList list;
for (int i = 0; i < 5; ++i) {
    list.append(i);
}

PythonList middle = list.slice(1, 4);    // [1, 2, 3]
PythonList everyOther = list.slice(0, 5, 2); // [0, 2, 4]
PythonList reversed = list.slice(4, -1, -1); // Empty with current bound normalization
PythonList backwards = list.slice(4, -6, -1); // [4, 3, 2, 1, 0]
```

**Exception**

- Throws `std::invalid_argument` if `step` is zero.

**Important negative-step detail:** negative `stop` values are first adjusted by the list size. Therefore, `slice(4, -1, -1)` adjusts `stop` to the last element's index and returns an empty list because the loop condition is `index > stop`. To include the element at index zero when stepping backward, use a stop value that adjusts to `-1`, such as `slice(4, -6, -1)` for a five-element list.

Slicing copies each selected `std::any` into the result. It does not create a view into the original list. Reference-wrapper elements remain wrappers referring to the same external objects.

### Printing

#### `operator<<(std::ostream&, const PythonList&)`

```cpp
friend std::ostream& operator<<(
    std::ostream& os,
    const PythonList& pl
);
```

Prints the list in square brackets, with elements separated by comma and space.

**Example**

```cpp
std::cout << list << '\n';
```

For example, a list containing an integer, a floating-point value, and a string can print as:

```text
[42, 3.14, Hello, World!]
```

Strings are printed using their stream insertion operator, without automatic quotes.

The implementation maintains a shared registry of printers for types that support `std::ostream << const T&`. If a stored type has no registered printer, the output contains `<unsupported type>` for that element. The registry is populated when values are appended or inserted, or when reference values are appended.

#### `PythonList::ProxyElement::print(std::ostream&) const`

```cpp
void print(std::ostream& os) const;
```

Prints the proxy's currently referenced element using the same printer registry. If no printer is registered for its stored type, prints `<unsupported type>`.

#### `operator<<(std::ostream&, const PythonList::ProxyElement&)`

```cpp
friend std::ostream& operator<<(
    std::ostream& os,
    const ProxyElement& proxy
);
```

Allows a single element proxy to be printed directly:

```cpp
std::cout << list[0];
```

## Examples

### Heterogeneous storage and access

```cpp
#include <PythonList>
#include <iostream>
#include <string>

int main() {
    PythonList list;
    list.append(42);
    list.append(3.14);
    list.append("hello");

    int number = list[0];
    double pi = list[1];
    std::string text = list[2].getValue<std::string>();

    std::cout << list << '\n';
    std::cout << number << ", " << pi << ", " << text << '\n';
}
```

### Insert, pop, and slice

```cpp
PythonList list;
list.append(10);
list.append(30);
list.insert(1, 20);

std::any removed = list.pop(); // 30
int value = std::any_cast<int>(removed);

PythonList part = list.slice(0, 2); // [10, 20]
```

### Combine lists

```cpp
PythonList left;
left.append(1);
left.append(2);

PythonList right;
right.append(3);
right.append(4);

PythonList combined = left + right;
left += right;
```

## Behavior notes and limitations

- **Type-sensitive search:** `contains()` and `remove()` require both the stored type and value to match. They do not perform cross-type numeric comparisons.
- **No const indexing overload:** `operator[]` is only declared as a non-`const` member function.
- **Proxy lifetime:** a `ProxyElement` refers to an element inside the vector. Structural modifications to the list may invalidate the proxy.
- **Reference lifetime:** objects passed to `appendReference()` must remain alive while the stored reference is used.
- **Printing:** strings are not quoted automatically. Unsupported stored types print as `<unsupported type>`.
- **Slicing:** returns a new list with copied `std::any` elements, not a view. Negative-step bounds follow the implementation's normalization rules, which are not identical to every Python slicing edge case.
- **Equality operators:** `PythonList` does not declare `operator==` or `operator!=` in the supplied header.
- **Custom printer registry:** printers are registered for types when appended or inserted through the corresponding APIs. A type that is not printable, or a type stored without a registered printer, may display as `<unsupported type>`.