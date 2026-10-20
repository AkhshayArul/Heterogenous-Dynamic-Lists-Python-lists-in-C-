# Phase 1: Implementation

## 1. append()

The append() method was implemented to add elements to the end of the list.

Two overloads were introduced to support different types of input.

### Template Overload

cpp
template<typename T>
void append(T&& data);


This overload allows users to append data without explicitly constructing a std::any object. It uses std::forward<T>() to preserve the value category of the argument when storing it in the internal vector.

Calls registerPrinter at last .

### Character Pointer Overload

cpp
void append(const char* data);


This overload converts the supplied character sequence into a std::string before storing it. This ensures that string literals and valid C-style strings passed through this overload are stored as std::string objects rather than character pointers.

Calls registerPrinter<std::string> at last .

*Example:*

cpp
list.append(10);
list.append(20.5);
list.append("Hello");


## 2. ProxyElement

The nested ProxyElement class was introduced to provide access to individual elements of the list. It stores a reference to the corresponding std::any object, allowing the stored value to be accessed without copying the entire element.

### getValue<T>()

The getValue<T>() method was updated to support both directly stored values and references added using appendReference().

cpp
template<typename T>
T& getValue();


The method first uses the pointer form of std::any_cast to check whether the stored object directly matches the requested type. If it does, a reference to the stored value is returned.

If the types do not match, the method checks whether the stored object is a std::reference_wrapper<T>. If so, it returns a reference to the original object using the wrapper's get() method.

If neither check succeeds, the method explicitly throws a custom TypeMismatchError exception.

This allows the same retrieval method to work with both values stored using append() and references stored using appendReference(), without requiring the user to handle the two cases separately.
