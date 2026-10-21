#pragma once

#include <any>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <unordered_map>
#include <typeindex> 
#include <type_traits>
#include <iostream>
#include <stdexcept>

template<typename T, typename = void>
struct IsPrintable : std::false_type {};

template<typename T>
struct IsPrintable<T, std::void_t<
    decltype(
        std::declval<std::ostream&>()
            << std::declval<const T&>()
    )
>> : std::true_type {};

class TypeMismatchError : public std::runtime_error {
public:
    explicit TypeMismatchError(const std::string& message)
        : std::runtime_error(message) {}
};

class PythonList {
private:
    std::vector<std::any> list;
    static std::unordered_map<
        std::type_index,
        std::function<void(std::ostream&, const std::any&)>
    > printers;

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

            throw TypeMismatchError(
                "PythonList: requested type does not match stored type"
            );
        }

        template <typename T>
        operator T() {
            return getValue<T>() ;
        }

        void print(std::ostream& os) const {
            auto it = PythonList::printers.find(
                std::type_index(data.type())
            );

            if (it != PythonList::printers.end()) {
                it->second(os, data);
            } else {
                os << "<unsupported type>";
            }
        }

        friend std::ostream& operator<<(
            std::ostream& os,
            const ProxyElement& proxy
        ) ;
        
    };

    template <typename T>
    void append(T&& data) {
        list.emplace_back(std::forward<T>(data));
        registerPrinter<T>() ;
    }

    template<typename T>
    void appendReference(T& data) {
        list.emplace_back(std::ref(data));
        registerReferencePrinter<T>();
    }

    void append(const char* data);

    template<typename T>
    void registerPrinter() {
        using U = std::decay_t<T>;

        if constexpr (IsPrintable<U>::value) {
            printers.try_emplace(
                std::type_index(typeid(U)),
                [](std::ostream& os, const std::any& value) {
                    os << std::any_cast<const U&>(value);
                }
            );
        }
    }

    template<typename T>
    void registerReferencePrinter() {
        using RefType = std::reference_wrapper<T>;

        if constexpr (IsPrintable<T>::value) {
            printers.try_emplace(
                std::type_index(typeid(RefType)),
                [](std::ostream& os, const std::any& value) {
                    os << std::any_cast<const RefType&>(value).get();
                }
            );
        }
    }

    ProxyElement operator[](std::size_t index);

    std::size_t size() const;

    void clear() ;

    bool empty() const ;

    friend std::ostream& operator<<(std::ostream& os, const PythonList& pl);

};
