# Phase 1: Implementation

## 1. `append()`

The `append()` method was implemented to add elements to the end of the list.

Two overloads were introduced to support different types of input.

### Template Overload

```cpp
template<typename T>
void append(T&& data);
```

This overload allows users to append data without explicitly constructing a `std::any` object. It uses `std::forward<T>()` to preserve the value category of the argument when storing it in the internal vector.

Calls `registerPrinter` at last .

### Character Pointer Overload

```cpp
void append(const char* data);
```

This overload converts the supplied character sequence into a `std::string` before storing it. This ensures that string literals and valid C-style strings passed through this overload are stored as `std::string` objects rather than character pointers.

Calls `registerPrinter<std::string>` at last .

**Example:**

```cpp
list.append(10);
list.append(20.5);
list.append("Hello");
```

## 2. `ProxyElement`

The nested `ProxyElement` class was introduced to provide access to individual elements of the list. It stores a reference to the corresponding `std::any` object, allowing the stored value to be accessed without copying the entire element.

### `getValue<T>()`

The `getValue<T>()` method was updated to support both directly stored values and references added using `appendReference()`.

```cpp
template<typename T>
T& getValue();
```

The method first uses the pointer form of `std::any_cast` to check whether the stored object directly matches the requested type. If it does, a reference to the stored value is returned.

If the types do not match, the method checks whether the stored object is a `std::reference_wrapper<T>`. If so, it returns a reference to the original object using the wrapper's `get()` method.

If neither check succeeds, the method explicitly throws a custom `TypeMismatchError` exception.

This allows the same retrieval method to work with both values stored using `append()` and references stored using `appendReference()`, without requiring the user to handle the two cases separately.

**Example:**

```cpp
int value = 10;
list.appendReference(value);

int& retrieved = list[0].getValue<int>();
retrieved = 20;
```

In this example, `retrieved` refers to the original variable, so modifying it also changes `value` to `20`.

## 3. `operator[]`

The subscript operator was overloaded to provide indexed access to the list.

```cpp
ProxyElement operator[](std::size_t index);
```

It returns a `ProxyElement` referring to the element at the specified index. This allows the user to retrieve an element using the `getValue<T>()` method.

**Example:**

```cpp
int a = list[0].getValue<int>();
```

The implementation uses the vector's `at()` method to access the element, providing bounds checking and throwing `std::out_of_range` if the index is invalid.

## 4. `size()`

```cpp
std::size_t size() const;
```

The `size()` method returns the number of elements currently stored in the list. Since the list uses a `std::vector` internally, the method obtains this value directly from the vector's `size()` method.

## 5. `appendReference()`

The `appendReference()` method was introduced to allow elements to be added to the list by reference rather than by copying them.

```cpp
template<typename T>
void appendReference(T& data);
```

This method uses `std::ref()` to create a `std::reference_wrapper<T>`, which is then stored inside the internal `std::any` object. This allows the list to refer to the original object instead of storing an independent copy.

Consequently, modifications made to the object through a reference retrieved from the list will also affect the original object.

Calls registerReferencePrinter at last.

**Example:**

```cpp
std::vector<int> values = {10, 20, 30};

list.appendReference(values);
list[0].getValue<std::vector<int>>()[0] = 100;
```

In this example, modifying the first element through the list also changes the original vector.

**Note:** The original object must remain alive for as long as its stored reference is used. Accessing the reference after the original object has been destroyed results in undefined behavior.

## 6. `clear()`

The `clear()` method was introduced to remove all elements from the list.

```cpp
void clear();
```

It uses the internal vector's `clear()` method to remove all stored elements. The objects owned by the list are destroyed, while objects referred to through `std::reference_wrapper` remain unaffected.

After calling this method, the list contains no elements and its size becomes zero.

**Example:**

```cpp
list.append(10);
list.append(20.5);

list.clear();
```

## 7. `empty()`

The `empty()` method was introduced to check whether the list contains any elements.

```cpp
bool empty() const;
```

It returns `true` if the list is empty and `false` otherwise. The method is declared `const` because checking the list's state does not modify it.

**Example:**

```cpp
if (list.empty()) {
    std::cout << "The list is empty.";
}
```

This provides a convenient way to check whether the list contains elements without explicitly comparing the result of `size()` with zero.

## 8. `registerPrinter<T>()`

This function registers a printing function for a datatype if it supports the `<<` operator with `std::ostream`. It uses the `IsPrintable` type trait to check whether the datatype can be printed.

The function uses `std::decay_t<T>` to normalize the datatype before registering it. This ensures that the registered type matches the type stored inside `std::any`.

The printing function is stored in an `std::unordered_map` using `std::type_index` as the key. This allows the appropriate printing function to be retrieved based on the datatype of an element.

If a printer for the datatype is already registered, `try_emplace()` leaves the existing entry unchanged.

## 9. `registerReferencePrinter<T>()`

This function registers a printing function for elements stored using `appendReference()`. Since `appendReference()` stores a `std::reference_wrapper<T>` inside `std::any`, the registered printer uses the reference wrapper type as its key.

When printing an element, the function extracts the stored reference wrapper using `std::any_cast` and accesses the original object using `get()`. The object is then printed using its overloaded `<<` operator.

As with `registerPrinter<T>()`, a printer is registered only if the referenced datatype supports printing.

## 10. `operator<<`

```cpp
friend std::ostream& operator<<(std::ostream& os, const PythonList& pl);
```

This operator allows a `PythonList` object to be printed directly using an output stream.

The operator iterates through the elements of the list and searches the printer registry for a printing function matching each element's stored datatype. If a printer is found, it is called to print the element. Otherwise, the operator prints `<unsupported type>`.

The elements are enclosed in square brackets and separated by commas, producing a representation similar to a Python list.

For example:

```cpp
PythonList list;
list.append(10);
list.append(20.5);
list.append("Hello");

std::cout << list;
```

Output:

```text
[10, 20.5, Hello]
```

The operator returns the output stream, allowing further stream operations to be chained.

This functionality also supports custom datatypes that provide a suitable `operator<<`, without requiring changes to the underlying storage structure.

## 11. `TypeMismatchError`

`TypeMismatchError` is a custom exception class created to handle cases where the requested datatype does not match the datatype stored in a list element.

It inherits from `std::runtime_error`, allowing it to carry an explanatory error message that can be retrieved using the `what()` function.

The `getValue<T>()` function in `ProxyElement` throws this exception when the requested datatype cannot be extracted from the stored element. This replaces the direct use of `std::bad_any_cast` in the public behavior of the library, hiding the underlying use of `std::any` from users.

**Example:**

```cpp
PythonList list;
list.append(10);

try {
    double value = list[0].getValue<double>();
}
catch (const TypeMismatchError& e) {
    std::cout << e.what() << '\n';
}
```

Output:

```text
PythonList: requested type does not match stored type
```

In this example, the element stores an `int`, but the user requests a `double`. Since the types do not match, `getValue<double>()` throws a `TypeMismatchError`, which can be caught and handled by the user.

This provides a clearer, library-specific error message and makes the interface more independent of the internal storage implementation.
