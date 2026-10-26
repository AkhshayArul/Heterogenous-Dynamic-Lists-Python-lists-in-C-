#include "List.h"

int main() {
    PythonList list;

    for (int i : {10, 20, 30, 40, 50}) {
        list.append(i);
    }

    std::cout << list.slice(1, 4) << '\n';
    // Expected: [20, 30, 40]

    std::cout << list.slice(0, 5, 2) << '\n';
    // Expected: [10, 30, 50]

    std::cout << list.slice(-3, -1) << '\n';
    // Expected: [30, 40]

    std::cout << list.slice(4, -6, -1) << '\n';
    // Expected: [50, 40, 30, 20, 10]

    std::cout << list.slice(2, 2) << '\n';
    // Expected: []

    std::cout << list << '\n';
    // Expected: [10, 20, 30, 40, 50]
}