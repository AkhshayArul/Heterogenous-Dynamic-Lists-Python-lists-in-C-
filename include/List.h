#pragma once

#include <any>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>
#include <functional>

class PythonList {
private:
    std::vector<std::any> list;

public:
    class ProxyElement {
    private:
        std::any& data;

    public:
        ProxyElement(std::any& d) : data(d) {}

        template<typename T>
        T& getValue() {
            if (auto value = std::any_cast<T>(&data)) {
                return *value;
            }

            if (auto ref =
                    std::any_cast<std::reference_wrapper<T>>(&data)) {
                return ref->get();
            }

            throw std::bad_any_cast();
        }

    };

    template <typename T>
    void append(T&& data) {
        list.emplace_back(std::forward<T>(data));
    }

    template<typename T>
    void appendReference(T& data) {
        list.emplace_back(std::ref(data));
    }

    void append(const char* data);

    ProxyElement operator[](std::size_t index);

    std::size_t size() const;

    void clear() ;

    bool empty() const ;

};