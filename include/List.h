#pragma once

#include <any>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

class PythonList {
private:
    std::vector<std::any> list;

public:
    class ProxyElement {
    private:
        std::any& data;

    public:
        ProxyElement(std::any& d) : data(d) {}

        template <typename T>
        T& getValue() {
            return std::any_cast<T&>(data);
        }
    };

    template <typename T>
    void append(T&& data) {
        list.emplace_back(std::forward<T>(data));
    }

    void append(const char* data);

    ProxyElement operator[](std::size_t index);

    std::size_t size() const;

};