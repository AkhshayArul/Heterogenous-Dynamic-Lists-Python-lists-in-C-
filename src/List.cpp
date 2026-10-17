#include "List.h"

void PythonList::append(const char* data) {
    list.emplace_back(std::string(data));
}

PythonList::ProxyElement PythonList::operator[](std::size_t index) {
    return ProxyElement(list.at(index));
}

std::size_t PythonList::size() const {
    return list.size();
}