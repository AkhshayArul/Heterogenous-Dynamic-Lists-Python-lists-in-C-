
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include "List.h"

int main() {
    std::cout << "=== PythonList Phase 2 Regression Tests ===\n";

    // 1. Append and negative indexing
    {
        PythonList list;
        list.append(10);
        list.append(20);
        list.append(30);

        assert(list.size() == 3);
        assert(static_cast<int>(list[0]) == 10);
        assert(static_cast<int>(list[-1]) == 30);
        assert(static_cast<int>(list[-2]) == 20);

        std::cout << "[PASS] Append and negative indexing\n";
    }

    // 2. Insert
    {
        PythonList list;
        list.append(10);
        list.append(30);

        list.insert(1, 20);
        assert(list.size() == 3);
        assert(static_cast<int>(list[1]) == 20);

        list.insert(-1, 25);
        assert(static_cast<int>(list[2]) == 25);

        std::cout << "[PASS] Insert\n";
    }

    // 3. Pop
    {
        PythonList list;
        list.append(10);
        list.append(20);
        list.append(30);

        std::any last = list.pop();
        assert(std::any_cast<int>(last) == 30);

        std::any middle = list.pop(0);
        assert(std::any_cast<int>(middle) == 10);

        assert(list.size() == 1);
        assert(static_cast<int>(list[0]) == 20);

        std::cout << "[PASS] Pop\n";
    }

    // 4. Remove and contains
    {
        PythonList list;
        list.append(10);
        list.append(20);
        list.append(10);
        list.append("Hello");

        assert(list.contains(20));
        assert(list.contains("Hello"));
        assert(!list.contains(99));

        list.remove(10);
        assert(list.size() == 3);
        assert(static_cast<int>(list[0]) == 20);
        assert(static_cast<int>(list[1]) == 10);

        bool missingThrown = false;
        try {
            list.remove(99);
        } catch (const ArgumentNotPresentError&) {
            missingThrown = true;
        }
        assert(missingThrown);

        std::cout << "[PASS] Remove and contains\n";
    }

    // 5. Extend, +=, and +
    {
        PythonList a;
        a.append(1);
        a.append(2);

        PythonList b;
        b.append(3);
        b.append(4);

        a.extend(b);
        assert(a.size() == 4);
        assert(static_cast<int>(a[2]) == 3);

        a += b;
        assert(a.size() == 6);
        assert(static_cast<int>(a[5]) == 4);

        PythonList c = a + b;
        assert(c.size() == 8);
        assert(static_cast<int>(c[6]) == 3);

        // + must not modify its operands.
        assert(a.size() == 6);
        assert(b.size() == 2);

        std::cout << "[PASS] Extend, +=, and +\n";
    }

    // 6. Self-extension
    {
        PythonList list;
        list.append(1);
        list.append(2);

        list.extend(list);

        assert(list.size() == 4);
        assert(static_cast<int>(list[0]) == 1);
        assert(static_cast<int>(list[1]) == 2);
        assert(static_cast<int>(list[2]) == 1);
        assert(static_cast<int>(list[3]) == 2);

        std::cout << "[PASS] Self-extension\n";
    }

    // 7. Slicing
    {
        PythonList list;
        for (int value : {10, 20, 30, 40, 50}) {
            list.append(value);
        }

        PythonList a = list.slice(1, 4);
        assert(a.size() == 3);
        assert(static_cast<int>(a[0]) == 20);
        assert(static_cast<int>(a[2]) == 40);

        PythonList b = list.slice(0, 5, 2);
        assert(b.size() == 3);
        assert(static_cast<int>(b[1]) == 30);

        PythonList c = list.slice(-3, -1);
        assert(c.size() == 2);
        assert(static_cast<int>(c[0]) == 30);
        assert(static_cast<int>(c[1]) == 40);

        PythonList reversed = list.slice(4, -6, -1);
        assert(reversed.size() == 5);
        assert(static_cast<int>(reversed[0]) == 50);
        assert(static_cast<int>(reversed[4]) == 10);

        // Original remains unchanged.
        assert(list.size() == 5);
        assert(static_cast<int>(list[0]) == 10);

        bool zeroStepThrown = false;
        try {
            list.slice(0, 5, 0);
        } catch (const std::invalid_argument&) {
            zeroStepThrown = true;
        }
        assert(zeroStepThrown);

        std::cout << "[PASS] Slicing\n";
    }

    // 8. Type mismatch
    {
        PythonList list;
        list.append(123);

        bool mismatchThrown = false;
        try {
            (void)list[0].getValue<std::string>();
        } catch (const TypeMismatchError&) {
            mismatchThrown = true;
        }
        assert(mismatchThrown);

        std::cout << "[PASS] Type mismatch handling\n";
    }

    // 9. Invalid indexing
    {
        PythonList list;
        list.append(10);

        bool invalidThrown = false;
        try {
            (void)list[5];
        } catch (const std::out_of_range&) {
            invalidThrown = true;
        }
        assert(invalidThrown);

        std::cout << "[PASS] Invalid indexing\n";
    }

    // 10. Nested PythonList
    {
        PythonList inner;
        inner.append(100);
        inner.append(200);

        PythonList outer;
        outer.append(inner);

        PythonList retrieved =
            outer[0].getValue<PythonList>();

        assert(retrieved.size() == 2);
        assert(static_cast<int>(retrieved[1]) == 200);

        std::cout << "[PASS] Nested PythonList\n";
    }

    // 11. appendReference
    {
        int value = 10;

        PythonList list;
        list.appendReference(value);

        value = 25;

        // This assumes reference retrieval uses std::reference_wrapper.
        int retrieved =
            list[0].getValue<std::reference_wrapper<int>>().get();

        assert(retrieved == 25);

        std::cout << "[PASS] appendReference\n";
    }

    std::cout << "\nAll Phase 2 regression tests passed!\n";
    return 0;
}
