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