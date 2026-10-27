# Future Developments

## Overview

The `PythonList` project aims to provide a Python-like list interface in C++. The current implementation supports heterogeneous data storage using `std::any`, element access through `ProxyElement`, insertion and removal operations, list concatenation, negative indexing, slicing, and stream output.

Future development will focus on improving Python-like behavior, extending the public API, strengthening reliability, and making the library easier to use and integrate into other C++ projects.

## 1. Improvements to Python-Like Behavior

### 1.1 More Complete Slicing Support

The current implementation supports slicing with `start`, `stop`, and `step` parameters. Future work could improve compatibility with Python's slicing behavior, particularly for negative steps and boundary cases.

Potential improvements include:
- Matching Python's behavior for negative-step slices.
- Handling empty lists and out-of-range slice boundaries consistently.
- Adding more regression tests for slicing edge cases.

### 1.2 Enhanced Element Access

The existing `operator[]` supports negative indexing and returns a `ProxyElement`.

Possible improvements include:
- Adding a `const` overload of `operator[]`.
- Reviewing proxy lifetime and invalidation behavior.
- Improving the ergonomics of accessing stored values while preserving type safety.

### 1.3 Additional List Operations

Additional Python-inspired operations could make the library more convenient to use.

Possible additions include:
- `reverse()` to reverse the order of elements.
- `index()` to find the position of a value.
- `count()` to count occurrences of a value.
- `sort()` for lists whose elements support a suitable comparison operation.

Operations involving comparisons should account for the heterogeneous nature of the list and the possibility that different stored types cannot be compared meaningfully.

## 2. Improved Type Handling

### 2.1 Type Conversion and Retrieval

The current implementation provides `getValue<T>()` and an implicit conversion operator through `ProxyElement`.

Future improvements could include:
- Clearer diagnostics when the requested type does not match the stored type.
- Additional tests for retrieval of references and ordinary stored values.
- Documentation explaining when implicit conversions are valid and when explicit type retrieval is preferable.

### 2.2 Reference Storage

The `appendReference()` method allows an existing object to be stored through `std::reference_wrapper`.

Potential improvements include:
- More examples demonstrating reference storage and mutation.
- Clearer documentation of object lifetime requirements.
- Additional tests for reference retrieval and printing.

Reference storage must remain non-owning: the referenced object must outlive every use of the stored reference.

### 2.3 Custom Type Support

The current printing mechanism uses a registry of functions for supported printable types.

Future work could improve extensibility by:
- Providing a documented way to register printers for custom types.
- Improving the behavior when a type has no registered printer.
- Testing custom classes with overloaded stream insertion operators.
- Reviewing how printer registration interacts with copied lists and reference-wrapped values.

## 3. Reliability and Error Handling

Reliability is important when a container supports multiple data types and operations that modify its contents.

Potential improvements include:
- Expanding regression tests for empty lists, invalid indexes, and boundary conditions.
- Testing copy and move behavior.
- Verifying behavior after sequences of insertions, removals, and list concatenations.
- Reviewing exception guarantees for operations that modify the list.
- Improving consistency in exception messages and error handling.

## 4. API and Usability Improvements

The public API can be made easier to understand and use as the project evolves.

Possible improvements include:
- Adding convenient overloads where they improve readability.
- Reviewing whether additional operators, such as equality comparison, would be useful.
- Improving consistency between methods that accept C strings and those that accept other string types.
- Providing more examples for nested lists and common operations.
- Keeping the API documentation synchronized with the implementation.

Any new operator should have clearly defined semantics, especially when comparing or combining heterogeneous values.

## 5. Testing and Quality Assurance

The regression test suite should grow alongside the library.

Future testing work could include:
- Dedicated tests for every public method and operator.
- Tests for custom printable and non-printable types.
- Tests for type mismatches and unsupported comparisons.
- Tests for negative indexing and slicing boundaries.
- Tests for reference storage and object lifetime assumptions.
- Tests of the installed library from a separate consumer project.
- Automated builds and tests on additional compilers and platforms.

## 6. Packaging and Distribution

The project already provides CMake-based build and installation support. Future development could improve the experience for users who want to integrate the library into their own projects.

Potential improvements include:

Make installation straightforward through a package manager such as Conan or vcpkg, or provide a simple installation workflow. This involves package metadata, versioning, release management, and testing across supported platforms.

## Conclusion

Future development of `PythonList` will prioritize correctness, usability, and consistency with Python-like list behavior while respecting C++'s type system and memory model.

The goal is to evolve the project into a reliable and well-documented C++ library for heterogeneous lists, with a convenient public interface and predictable behavior.