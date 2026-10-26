# Project Approach


## 1. Choosing the Implementation Method

The first step of the project was to figure out a way to implement the heterogeneous, Python-like list.

A few searches and discussions with AI bots led to three ways:

1. Use a vector of `std::any` objects to store different data types together.
2. Use a vector of custom classes which store data via a `void` pointer and have information about what data type is stored in that object.
3. Use a vector of custom classes (one class for one datatype derived from a single base class) and store and retrieve data polymorphically.

Initially, the plan was to implement both the first and third methods.

But after a little bit more pondering and researching, it turned out that retrieving the stored data wouldn't work in the way I thought was possible with the third method. The reason being, C++ rules and limitations [or the lack of my knowledge of them :( ].

So ultimately, the third method was scratched, and implementation with the first method was finalised.

## 2. Why `std::any` Was Chosen

The reason I wanted to choose the third option was that it would allow the most Python-like implementation of all methods [at least in my mind, it did so :( ]. But due to it being scratched, only the first and second options were left.

They both had very similar working at the low level. It just boiled down to choosing whether to use a built-in class and not worry about intricate memory management and safety, or create and use a custom class AND worry about memory management.

The goal of the project was to implement a Python-like list, so it was decided that we would proceed with the built-in class (`std::any` and `std::any_cast`). This would allow us to focus on making the list more and more Python-like instead of hitting our heads on internals that are very complex.

Be it as it may, this project will still require some extent of memory manipulation skills for proper function.

So that was the lore of why the third method was scratched.                                    

# Phase 1: Essential Functions and Methods

## Goal

Phase 1's goal is to complete the indispensable functions and methods needed for this library.

## Internal Storage and the append() Function

Following the main plan, a vector of std::any objects was created to store the data internally inside the list.

The first function that needed to be implemented was a way of putting data into the list, and the function append() was chosen for that purpose.

Initially, the signature of the function was:

```cpp
void PythonList::append(std::any& data);
```

However, this meant that users needed to send in the data in the form of std::any objects, which exposes the internals of the list and is usually not desirable, as append() is one of the most frequently used functions in a list.

So, in order to hide the implementation and allow for a more user-friendly append() method, we decided to use template functions.

This allowed users to add data to the list conveniently, as shown below:

```cpp
list.append(10);
list.append(20.5);
```

This is much more convenient than before and more Python-like.

## Handling Strings

But in this process, a small caveat was discovered. If we append a string literal, due to the usage of template functions, it can be interpreted as a character pointer (const char*) rather than a std::string, which was not desirable due to the inconvenience it posed while retrieving the data.

So, a separate overload for string literals was introduced to store string instances.

As a consequence, string literals are always stored in the form of std::string and retrieved in the same form. This is much less of an inconvenience, as strings can be converted into character arrays or accessed as C-style strings when needed, and std::string retains the ability to represent and manage character sequences conveniently.

## Data Retrieval and Size Checking

The stored data is retrieved using the `[]` operator and resolving the data from it. The `[]` operator returns a proxy class, `ProxyElement`, which has the method `getValue()` to obtain the data from the proxy.

Sample usage:

```cpp
int a = list[10].getValue<int>();
```

As we internally use a vector, checking the size is easy. We only need to find the size of the vector stored inside the class, which is done through the `size()` method.

## Storing Copies and References

The implementation so far only allows users to store copies of objects they append. This is because `std::any` stores its contained object by value, making a copy when an object is copied into it. This behaviour differs from Python's, where lists store references to objects. In C++, objects can be destroyed explicitly or go out of scope, which can invalidate references to them. Storing a copy allows the list to manage the lifetime of the stored object independently of the original object.

For immutable data types such as `int` or `double`, this copying behaviour is often almost indistinguishable from storing references, as their values are not modified in place in the same way as mutable objects. However, the project also aims to support nested lists and mutable data types. Therefore, a new method, `appendReference()`, was added to allow users to append references to existing objects instead of copies.

There are a few catches with this new method, and the reasons for them once again stem from the way C++ works.

1. The concept of references differs between Python and C++. A Python reference is closer to a C++ pointer than to a C++ reference. Reassigning a variable in Python makes it refer to a different object without changing the original object, whereas assigning a new value through a C++ reference modifies the original object. This difference is inherent to the languages, so the behaviour of the list differs slightly from that of a Python list.

2. The next difference stems from Python's automatic memory management, which uses garbage collection. In C++, objects can be destroyed explicitly or automatically when their scope ends. Consequently, an object whose reference is stored inside the list may be destroyed, leaving the stored reference invalid. Accessing such a reference results in undefined behaviour and may cause a crash. Therefore, users must ensure that the objects they reference remain alive for as long as those references are used.

3. Temporary values (rvalues) are prohibited from being appended using this method. Temporary objects are typically destroyed at the end of the full expression in which they are created, which can leave the stored references invalid. In contrast, `append()` stores the object itself, moving it when appropriate, so the stored value does not depend on the lifetime of the original object. Therefore, the following calls are not allowed:

```cpp
list.appendReference(10);
list.appendReference(std::vector<int>{1, 2, 3});
```

### Accommodating `getValue()`

As a result of the newly added `appendReference()` method, the list may now contain `std::any` objects that store values directly or contain a `std::reference_wrapper` referring to an existing object. Therefore, the `getValue()` method needed to be updated to return the appropriate value by reference when an element is accessed.

It was updated to handle both cases. First, it uses the pointer form of `std::any_cast` to check whether the stored object matches the requested type. If it does, the method returns a reference to that object. Otherwise, it checks whether the stored object is a `std::reference_wrapper` of the requested type and, if so, returns a reference to the original object.

If neither check succeeds, the method throws a `std::bad_any_cast` exception.

## `empty()` and `clear()` Methods

The vector's `empty()` method is used to check whether the list contains any elements. The `clear()` method is used to clear the list and destroy all stored objects. Stored `std::reference_wrapper` objects are also destroyed, but the original objects they refer to are not destroyed.

## Printing Contents

The next challenge was finding a way to display all the elements stored inside the list.

So far, retrieving data from `std::any` required a templated function in which the user specified the desired type. However, this approach is not suitable for printing the entire list, as the user cannot be expected to provide the types of all the elements stored in it. This called for a different approach.

Two options were considered:

### 1. Try-Catch Chaining (or Pointer Casting to Avoid Exception Handling)

In this method, the data stored in `std::any` is repeatedly cast to different types until the correct type is found. This can be done by checking whether `std::bad_any_cast` is thrown or, when using pointer-based casting, whether a non-null pointer is returned.

This method is relatively straightforward, but it is tedious and rigid. Supporting multiple data types can become cumbersome, and adding support for new types would require modifying the casting logic.

### 2. Mapping Each Type to Its Own Printing Function

In this method, the type of the data is known when it is appended to the list. This information is used to associate the type with a function capable of printing it, avoiding repeated casting attempts. If a type already has an associated printing function, the existing mapping is retained.

These functions are stored in a `static std::unordered_map`, with `std::type_index` as the key and the corresponding printing function as the value. The key is derived from `typeid` applied to the normalized data type of the template parameter, ensuring that the mapping uses the appropriate type.

The second method was chosen because it offers greater extensibility and a more Python-like feel, allowing support for additional types without having to modify a chain of casting operations.

Whenever data is appended, the implementation checks whether the expression `std::ostream << TYPE` is well-formed. This determines whether the type supports stream insertion, and therefore whether it can be printed using `operator<<`. It does not guarantee that the resulting output will be formatted perfectly; it only establishes that the type is printable through this mechanism.

This technique also allows custom classes to be printed seamlessly, provided they have an accessible and compatible stream insertion operator overload.

If the type is printable, a printing function is created using a lambda expression and inserted into the map using `try_emplace`. This ensures that an existing mapping is not overwritten.

Printing is performed through an overloaded `operator<<`. Each element is visited using a range-based `for` loop. If a printing function is registered for the element's type, that function is used; otherwise, `<unsupported type>` is printed.

## Custom Error Class

So far, attempting to retrieve an element using an incorrect data type would cause `std::any_cast` to throw `std::bad_any_cast`. Exposing this implementation-specific detail directly through the list's interface was considered undesirable, as it ties the error handling to the underlying storage mechanism.

To address this, a custom error class named `TypeMismatchError` was created. It is thrown when the requested type does not match the stored type, instead of allowing `std::bad_any_cast` to propagate directly to the user. It also provides a more descriptive error message, making it easier to understand what went wrong.

# Phase 2: Pythonic Behaviour

## Improved Data Retrieval Interface and `ProxyElement` Print Support

Previously, data retrieval was done using `getValue<T>()`. This required the user to specify the required type in the function call, making the interface a little cumbersome. To improve it, two features were added.

### 1. Type Conversion from `ProxyElement` to the Required Type

A templated type conversion operator was added to `ProxyElement`, allowing it to use the already implemented `getValue<T>()` method internally. This simplifies the interface, as users can now directly write:

```cpp
int a = list[0];
```

instead of:

```cpp
int a = list[0].getValue<int>();
```

This is a useful improvement over the previous approach. However, since the conversion operator still uses `getValue<T>()` internally, it throws `TypeMismatchError` when an incompatible data type is requested.

### 2. Print Support for `ProxyElement`

Previously, users had to know the type of an element to retrieve its value explicitly, or print the entire list without accessing individual elements directly through the stream insertion operator.

To improve this, print support was added for `ProxyElement`. It uses the print registry constructed when elements are appended and looks up the corresponding printing function whenever an element is printed. Just as when printing the entire list, if an element's type does not have a registered printing function, `<unsupported type>` is printed instead.

With this functionality, users can write:

```cpp
std::cout << list[0];
std::cout << list[5];
```

This allows individual elements to be printed without requiring the user to specify their data types explicitly.

## Negative Indexing Support

To improve the Python-like feel of `PythonList`, support for negative indexing was added, similar to Python's built-in lists.

This was achieved by adding the length of the list to the index whenever a negative index is detected, converting it into its corresponding position from the beginning of the list.

This allows users to use negative indices wherever indexing is supported, for example:

```cpp
list[-3]; // Retrieves the third-to-last element
list[-1]; // Retrieves the last element
```

A `std::out_of_range` exception is thrown when an index is out of bounds in operations where such indices are not accepted, such as element retrieval.

## Insertion at a Given Index and Popping by Index

Python-like `insert()` and `pop()` methods were added, both supporting negative indexing.

The `insert()` method uses the built-in `std::vector::insert()` function and `std::in_place_type` to construct the `std::any` object directly at the specified position in the vector. This is similar to using `emplace_back()`, but allows insertion at an arbitrary index. If the provided index is outside the list's bounds, it is adjusted to the beginning or end of the list, depending on its value.

The `pop()` method removes and returns the element at the specified index. If no index is provided, it defaults to the last element. The removed element is returned as a `std::any` object, which can be cast to the required data type using `std::any_cast`.

## Removing and Checking for Elements

The `remove()` and `contains()` methods were added to support basic Python-like list operations.

The `IsEqualityComparable` trait is used to check whether the given type supports an equality expression whose result is convertible to `bool`. If comparison is unsupported, a custom `UnsupportedOperationError` is thrown.

Both methods compare elements only when their stored types match the type of the given value. The `remove()` method removes the first matching element and throws `ArgumentNotPresentError` if no match is found. The `contains()` method returns `true` if a match is found and `false` otherwise, without modifying the list.

Overloads for `const char*` arguments were also added to support string literals consistently with the list's string-handling behavior.

## Extending and Concatenating Lists

The `extend()` method was added to append the elements of another `PythonList` to the current list. The `operator+=` overload reuses this method to provide a more convenient interface.

The `operator+` overload was added to concatenate two lists by copying the left-hand list and extending it with the right-hand list. This produces a new list without modifying the original lists.

The elements are copied according to their types' copy constructors, rather than guaranteeing a deep copy of all underlying data.