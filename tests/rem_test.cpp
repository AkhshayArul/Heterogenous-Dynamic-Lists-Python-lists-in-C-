#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>

#include "List.h"

// A type with no operator==
struct NoEquality {
    int value;
};

// A user-defined type that supports equality
struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Verifies that the expected exception is thrown.
template <typename Exception, typename Function>
void expectThrows(Function&& function) {
    bool caught = false;

    try {
        function();
    }
    catch (const Exception&) {
        caught = true;
    }

    assert(caught && "Expected exception was not thrown");
}

int main() {
    // 1. Remove an ordinary integer
    {
        PythonList list;
        list.append(10);
        list.append(20);

        list.remove(10);

        assert(list.size() == 1);
        assert(list[0].getValue<int>() == 20);
    }

    // 2. Remove the first occurrence only
    {
        PythonList list;
        list.append(10);
        list.append(20);
        list.append(10);
        list.append(10);

        list.remove(10);

        assert(list.size() == 3);
        assert(list[0].getValue<int>() == 20);
        assert(list[1].getValue<int>() == 10);
        assert(list[2].getValue<int>() == 10);
    }

    // 3. Remove the last element
    {
        PythonList list;
        list.append(10);
        list.append(20);

        list.remove(20);

        assert(list.size() == 1);
        assert(list[0].getValue<int>() == 10);
    }

    // 4. Remove the only element
    {
        PythonList list;
        list.append(42);

        list.remove(42);

        assert(list.empty());
        assert(list.size() == 0);
    }

    // 5. Removing from an empty list
    {
        PythonList list;

        expectThrows<ArgumentNotPresentError>(
            [&] { list.remove(10); }
        );

        assert(list.empty());
    }

    // 6. Value not present in a non-empty list
    {
        PythonList list;
        list.append(10);
        list.append(20);

        expectThrows<ArgumentNotPresentError>(
            [&] { list.remove(30); }
        );

        assert(list.size() == 2);
        assert(list[0].getValue<int>() == 10);
        assert(list[1].getValue<int>() == 20);
    }

    // 7. Type-sensitive matching: int vs double
    {
        PythonList list;
        list.append(10.0);
        list.append(10);

        list.remove(10);

        assert(list.size() == 1);
        assert(list[0].getValue<double>() == 10.0);
    }

    // 8. Same numeric value, but wrong stored type
    {
        PythonList list;
        list.append(10.0);

        expectThrows<ArgumentNotPresentError>(
            [&] { list.remove(10); }
        );

        assert(list.size() == 1);
        assert(list[0].getValue<double>() == 10.0);
    }

    // 9. Remove a string
    {
        PythonList list;
        list.append(std::string("Hello"));
        list.append(std::string("World"));

        list.remove(std::string("Hello"));

        assert(list.size() == 1);
        assert(list[0].getValue<std::string>() == "World");
    }

    // 10. Remove using a C-string argument
    {
        PythonList list;
        list.append("Hello");
        list.append("World");

        list.remove("Hello");

        assert(list.size() == 1);
        assert(list[0].getValue<std::string>() == "World");
    }

    // 11. Missing C-string argument
    {
        PythonList list;
        list.append("Hello");

        expectThrows<ArgumentNotPresentError>(
            [&] { list.remove("Missing"); }
        );

        assert(list.size() == 1);
    }

    // 12. Unsupported equality type
    {
        PythonList list;
        list.append(10);

        expectThrows<UnsupportedOperationError>(
            [&] { list.remove(NoEquality{5}); }
        );

        // Unsupported comparison must not modify the list.
        assert(list.size() == 1);
        assert(list[0].getValue<int>() == 10);
    }

    // 13. Unsupported type on an empty list
    // The comparability check should happen before searching.
    {
        PythonList list;

        expectThrows<UnsupportedOperationError>(
            [&] { list.remove(NoEquality{5}); }
        );

        assert(list.empty());
    }

    // 14. User-defined type with operator==
    {
        PythonList list;
        list.append(Point{1, 2});
        list.append(Point{3, 4});
        list.append(Point{1, 2});

        list.remove(Point{1, 2});

        assert(list.size() == 2);
        assert(list[0].getValue<Point>().x == 3);
        assert(list[1].getValue<Point>().x == 1);
    }

    // 15. Remove a negative integer
    {
        PythonList list;
        list.append(-5);
        list.append(5);

        list.remove(-5);

        assert(list.size() == 1);
        assert(list[0].getValue<int>() == 5);
    }

    std::cout << "All remove() tests passed!\n";
}