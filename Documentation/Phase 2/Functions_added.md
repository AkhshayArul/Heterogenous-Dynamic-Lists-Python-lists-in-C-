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