# Phase 2 — Implementation

## 1. Simplified Element Retrieval

The `ProxyElement` class was updated to support implicit type conversion through a templated conversion operator. Previously, retrieving an element required explicitly calling `getValue<T>()` and specifying the desired data type. With this change, the type can be inferred from the destination variable, providing a more convenient and Python-like interface.

The conversion operator delegates to `getValue<T>()`, preserving the existing type-checking logic and custom `TypeMismatchError` exception handling.

**Example:**

```cpp
PythonList list;
list.append(10);
list.append(20.5);

int a = list[0];
double b = list[1];
```

Here, the compiler determines the required type from the destination variable and invokes the appropriate conversion operator.

The existing `getValue<T>()` function remains available when explicit type retrieval is preferred.

## 2. Printing Individual Elements

The `ProxyElement` class was updated to support printing individual elements directly through an overloaded `operator<<`. Previously, printing an element required accessing the list's printing functionality indirectly. With this change, an element can be printed using the familiar stream insertion syntax.

The overloaded operator delegates the printing operation to a member function of `ProxyElement`. This function looks up the element's type in the static printer registry maintained by `PythonList` and invokes the corresponding printing function if one is registered. If no printer is available, `<unsupported type>` is printed instead.

Looking up the printer when printing avoids storing a separate printing function in every proxy instance. It also keeps the type-specific printing logic centralized in the existing registry.

**Example:**

```cpp
PythonList list;
list.append(10);
list.append(20.5);
list.append(std::string("Hello"));

std::cout << list[0] << '\n';
std::cout << list[1] << '\n';
std::cout << list[2] << '\n';
```

**Output:**

```text
10
20.5
Hello
```

This change allows individual elements to be printed without requiring the user to specify their data types explicitly.

## 3. Negative Index Support

The `operator[]` implementation was updated to support negative indexing, bringing its behaviour closer to Python's built-in lists. Previously, the index parameter used `std::size_t`, which cannot represent negative values. It was changed to a signed integer type, `std::ptrdiff_t`, to allow negative indices to be handled correctly.

When a negative index is provided, the list's size is added to it to determine the corresponding position. The resulting index is validated before being converted to `std::size_t` for accessing the underlying `std::vector`. Invalid indices throw `std::out_of_range`.

**Example:**

```cpp
PythonList list;
list.append(10);
list.append(20);
list.append(30);

int a = list[-1]; // 30
int b = list[-2]; // 20
int c = list[-3]; // 10
```

An index outside the valid range, such as `list[-4]` for this list, throws an exception.

## 4. `insert()`

An `insert()` function was added to allow elements to be inserted at a specified position. It uses the underlying `std::vector` insertion mechanism to place the new element at the requested index while shifting subsequent elements to the right.

The function supports both negative indices and indices beyond the list's bounds. Negative indices are adjusted relative to the list's size, with indices that remain below zero clamped to the beginning. Indices greater than the list's size are clamped to the end.

The inserted element's type is preserved, and the appropriate printing function is registered so that the new element can be displayed using the existing printing functionality.

**Example:**

```cpp
PythonList list;
list.append(10);
list.append(20);
list.append(30);

list.insert(1, 99);
// [10, 99, 20, 30]

list.insert(-1, 25);
// [10, 99, 20, 25, 30]

list.insert(100, 40);
// [10, 99, 20, 25, 30, 40]
```

A `const char*` overload was also added to ensure that string literals are stored as `std::string`, rather than as character pointers.

## 5. `pop()`

A `pop()` function was added to remove an element at a specified index and return its value. If no index is provided, it defaults to `-1`, removing the last element. The function also supports negative indexing.

Before removing an element, the function checks whether the list is empty and whether the requested index is valid. An invalid index or an attempt to pop from an empty list throws `std::out_of_range`.

The removed element is moved into a `std::any` return value before being erased from the underlying vector. Returning `std::any` allows the removed value to be retrieved without depending on a `ProxyElement` that would otherwise reference an element no longer present in the list.

**Example:**

```cpp
PythonList list;
list.append(10);
list.append(20);
list.append(30);

std::any removed = list.pop(1);
int value = std::any_cast<int>(removed);

// value == 20
// list now contains [10, 30]

std::any last = list.pop();

// last contains 30
// list now contains [10]
```

This provides functionality similar to Python's `list.pop()`, while retaining the type-erased storage mechanism used by `PythonList`.