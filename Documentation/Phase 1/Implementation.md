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

### Character Pointer Overload

```cpp
void append(const char* data);
```

This overload converts the supplied character sequence into a `std::string` before storing it. This ensures that string literals and valid C-style strings passed through this overload are stored as `std::string` objects rather than character pointers.

**Example:**

```cpp
list.append(10);
list.append(20.5);
list.append("Hello");
```

## 2. `ProxyElement`

The nested `ProxyElement` class was introduced to provide access to individual elements of the list. It stores a reference to the corresponding `std::any` object, allowing the stored value to be accessed without copying the entire element.

### `getValue<T>()`

```cpp
template<typename T>
T& getValue();
```

This method retrieves the stored value as the requested type using `std::any_cast<T&>()`. The requested type must match the type contained in the `std::any` object. If the types do not match, `std::bad_any_cast` is thrown.

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