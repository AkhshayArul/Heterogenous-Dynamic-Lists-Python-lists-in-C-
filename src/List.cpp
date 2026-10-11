#include "../include/List.h"

void PythonList::append(const char* data) {
    list.emplace_back(std::string(data));
}
