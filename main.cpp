#include <PythonList>
#include <iostream>
#include <string>

int main() {
    std::cout << "=== HETEROGENEOUS DYNAMIC LIST (C++17) DEMO ===\n\n";

    // 1. Heterogeneous Storage & Dynamic Printing
    PythonList list;
    list.append(42);                      // int
    list.append(3.14159);                 // double
    list.append("Hello, C++17!");         // string
    list.append(true);                    // bool

    std::cout << "1. Heterogeneous List:\n   " << list << "\n\n";

    // 2. Negative Indexing & Automatic Extraction (Proxy Pattern)
    int first = list[0];                  // Implicit conversion
    auto last = list[-1];                 // Negative indexing: -1 is the tail

    std::cout << "2. Indexing & Extraction:\n";
    std::cout << "   list[0]  (int)  = " << first << "\n";
    std::cout << "   list[-1] (tail) = " << last << "\n\n";

    // 3. Nested Lists (Multi-dimensional storage)
    PythonList sublist;
    sublist.append("nested_A");
    sublist.append("nested_B");
    list.append(sublist);

    std::cout << "3. Nested List Support:\n   " << list << "\n\n";

    // 4. Value vs. Reference Semantics
    int live_var = 100;
    list.appendReference(live_var);       // Tracks variable by reference

    std::cout << "4. Reference Semantics:\n";
    std::cout << "   Before mutation : list[-1] = " << list[-1] << "\n";
    live_var = 999;                       // Mutate original external variable
    std::cout << "   After mutation  : list[-1] = " << list[-1] << " (updated live!)\n\n";

    // 5. Pythonic Slicing with Steps & Reversal
    PythonList numbers;
    for (int i = 0; i < 6; ++i) numbers.append(i); // [0, 1, 2, 3, 4, 5]

    std::cout << "5. Slicing Capabilities:\n";
    std::cout << "   Original list       : " << numbers << "\n";
    std::cout << "   Slice [1:5:2]       : " << numbers.slice(1, 5, 2) << "\n";
    std::cout << "   Reverse slice [5:0:-1]: " << numbers.slice(5, 0, -1) << "\n\n";

    // 6. Common Python Operations: insert, pop, operators
    numbers.insert(0, "START");           // Insert at head
    auto popped = numbers.pop(-1);        // Pop last item
    PythonList merged = numbers + list.slice(0, 2); // Concatenation via operator+

    std::cout << "6. Manipulations & Operators:\n";
    std::cout << "   After insert & pop  : " << numbers << "\n";
    std::cout << "   Concatenated list (+): " << merged << "\n\n";

    // 7. Robust Type Safety & Domain Exceptions
    std::cout << "7. Exception Safety:\n";
    try {
        // list[0] stores an integer (42); attempting string cast triggers exception
        std::string bad_cast = list[0];
    } catch (const TypeMismatchError& e) {
        std::cout << "   Caught TypeMismatchError: \"" << e.what() << "\"\n";
    }

    std::cout << "\n=== DEMO COMPLETED SUCCESSFULLY ===\n";
    return 0;
<<<<<<< HEAD
}
=======
}
>>>>>>> 1a46b326169b54f9b1bb63df1e02ad4a38acaa53
