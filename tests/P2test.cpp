
#include <iostream>
#include <cassert>
#include <any>
#include <string>
#include <stdexcept>
#include "List.h"

int main() {
    // Test 1: Insert at beginning, middle, and end
    {
        PythonList list;
        list.append(10);
        list.append(20);
        list.append(30);

        list.insert(0, 5);
        list.insert(2, 15);
        list.insert(100, 40);  // Clamp to end

        assert(list.size() == 6);
        assert(list[0].getValue<int>() == 5);
        assert(list[1].getValue<int>() == 10);
        assert(list[2].getValue<int>() == 15);
        assert(list[3].getValue<int>() == 20);
        assert(list[4].getValue<int>() == 30);
        assert(list[5].getValue<int>() == 40);
    }

    // Test 2: Negative indices for insert
    {
        PythonList list;
        list.append(10);
        list.append(20);
        list.append(30);
        list.append(40);

        list.insert(-1, 99);
        assert(list[3].getValue<int>() == 99);

        list.insert(-100, 5);  // Clamp to beginning
        assert(list[0].getValue<int>() == 5);
    }

    // Test 3: Insert a C-string
    {
        PythonList list;
        list.append(10);
        list.insert(1, "Hello");

        assert(list[1].getValue<std::string>() == "Hello");
        std::cout << list << '\n';
    }

    // Test 4: Pop from the end (default index = -1)
    {
        PythonList list;
        list.append(10);
        list.append(20);
        list.append(30);

        std::any removed = list.pop();
        assert(std::any_cast<int>(removed) == 30);
        assert(list.size() == 2);
        assert(list[1].getValue<int>() == 20);
    }

    // Test 5: Pop at a specific index
    {
        PythonList list;
        list.append(10);
        list.append(20);
        list.append(30);

        std::any removed = list.pop(1);
        assert(std::any_cast<int>(removed) == 20);
        assert(list.size() == 2);
        assert(list[0].getValue<int>() == 10);
        assert(list[1].getValue<int>() == 30);
    }

    // Test 6: Pop with negative index
    {
        PythonList list;
        list.append(10);
        list.append(20);
        list.append(30);

        std::any removed = list.pop(-2);
        assert(std::any_cast<int>(removed) == 20);
        assert(list.size() == 2);
    }

    // Test 7: Pop a string
    {
        PythonList list;
        list.append("Hello");
        list.append("World");

        std::any removed = list.pop(0);
        assert(std::any_cast<std::string>(removed) == "Hello");
        assert(list[0].getValue<std::string>() == "World");
    }

    // Test 8: Pop from an empty list
    {
        PythonList list;
        bool caught = false;

        try {
            list.pop();
        } catch (const std::out_of_range&) {
            caught = true;
        }

        assert(caught);
    }

    // Test 9: Invalid pop indices
    {
        PythonList list;
        list.append(10);
        list.append(20);

        bool lowCaught = false;
        bool highCaught = false;

        try {
            list.pop(-3);
        } catch (const std::out_of_range&) {
            lowCaught = true;
        }

        try {
            list.pop(2);
        } catch (const std::out_of_range&) {
            highCaught = true;
        }

        assert(lowCaught && highCaught);
        assert(list.size() == 2);
    }

    std::cout << "All insert() and pop() tests passed!\n";
}