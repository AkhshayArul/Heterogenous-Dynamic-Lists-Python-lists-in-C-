#pragma once

#include <any>
#include <algorithm>
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

template <typename T, typename = void>
struct IsEqualityComparable : std::false_type {};

template <typename T>
struct IsEqualityComparable<
    T,
    std::void_t<
        decltype(std::declval<const T&>() ==
                 std::declval<const T&>())
    >
> : std::bool_constant<
        std::is_convertible_v<
            decltype(std::declval<const T&>() ==
                     std::declval<const T&>()),
            bool
        >
    > {};

class TypeMismatchError : public std::runtime_error {
    public:
        explicit TypeMismatchError(const std::string& message)
            : std::runtime_error(message) {}
};

class UnsupportedOperationError : public std::runtime_error {
    public:
        explicit UnsupportedOperationError(const std::string& message)
            : std::runtime_error(message) {}
};

class ArgumentNotPresentError : public std::runtime_error {
    public:
        explicit ArgumentNotPresentError(const std::string& message)
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

        friend std::ostream& operator<<(
            std::ostream& os,
            const ProxyElement& proxy
        ) ;
        
        void print(std::ostream& os) const ;
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

    ProxyElement operator[](std::ptrdiff_t index) ;

    std::size_t size() const;

    void clear() ;

    bool empty() const ;

    friend std::ostream& operator<<(std::ostream& os, const PythonList& pl);

    template <typename T>
    void insert(std::ptrdiff_t index, T&& data) {
        std::ptrdiff_t n = static_cast<std::ptrdiff_t>(list.size());

        if (index < 0) {
            index += n;
            if (index < 0)
                index = 0;
        } else if (index > n) {
            index = n;
        }

        using U = std::decay_t<T>;

        list.insert(
            list.begin() + index,
            std::any(std::in_place_type<U>, std::forward<T>(data))
        );

        registerPrinter<U>();
    }

    void insert(std::ptrdiff_t index , const char* data) ;

    std::any pop(std::ptrdiff_t index = -1);

    template <typename T>
    void remove(const T& value) {
        using U = std::decay_t<T>;

        if constexpr (!IsEqualityComparable<U>::value) {
            throw UnsupportedOperationError(
                "PythonList : Equality comparison is not supported for this type"
            );
        } else {
            const std::type_index targetType(typeid(U));

            for (auto it = list.begin(); it != list.end(); ++it) {
                if (std::type_index(it->type()) != targetType) {
                    continue;
                }

                if (std::any_cast<const U&>(*it) == value) {
                    list.erase(it);
                    return; 
                }
            }

            throw ArgumentNotPresentError(
                "PythonList : The specified value is not present in PythonList"
            );
        }
    }

    void remove(const char* value) ;

    template <typename T>
    bool contains(const T& value) const {
        using U = std::decay_t<T>;

        if constexpr (!IsEqualityComparable<U>::value) {
            throw UnsupportedOperationError(
                "PythonList: equality comparison is not supported for this type"
            );
        } else {
            const std::type_index targetType(typeid(U));

            for (const auto& element : list) {
                if (std::type_index(element.type()) != targetType) {
                    continue;
                }

                if (std::any_cast<const U&>(element) == value) {
                    return true;
                }
            }

            return false;
        }
    }

    bool contains(const char* value) const ;

    void extend(const PythonList& other);

    PythonList& operator+=(const PythonList& other) ;

    PythonList operator+(const PythonList& other) const;

    PythonList slice(
        std::ptrdiff_t start,
        std::ptrdiff_t stop,
        std::ptrdiff_t step = 1
    ) const;

};
