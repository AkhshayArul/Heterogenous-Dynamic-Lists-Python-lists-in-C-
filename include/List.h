#pragma once

#include <any>
#include <string>
#include <utility>
#include <vector>

class PythonList {
private:
    std::vector<std::any> list;

public:
    template<typename T>
    void append(T&& data) {
        list.emplace_back(std::forward<T>(data));
    }

    void append(const char* data);
};
