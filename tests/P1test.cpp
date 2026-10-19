#include "List.h"
#include <iostream>

int main() {
    PythonList list;

    std::vector<int> original = {10, 20};

    list.append(original);
    list.appendReference(original);

    original.push_back(30);

    auto& copy = list[0].getValue<std::vector<int>>();
    auto& reference = list[1].getValue<std::vector<int>>();

    std::cout << copy.size() << '\n';      // 2
    std::cout << reference.size() << '\n'; // 3

    reference.push_back(40);

    std::cout << original.size() << '\n';  // 4
}