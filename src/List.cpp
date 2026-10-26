#include "List.h"

std::unordered_map<
        std::type_index,
        std::function<void(std::ostream&, const std::any&)>
    > PythonList::printers;
    
void PythonList::append(const char* data) {
    list.emplace_back(std::string(data));
    registerPrinter<std::string>() ;
}

PythonList::ProxyElement PythonList::operator[](std::ptrdiff_t index) {
    if (index < 0) {
        index += static_cast<std::ptrdiff_t>(list.size());
    }

    if (index < 0 ||
        static_cast<std::size_t>(index) >= list.size()) {
        throw std::out_of_range("PythonList: index out of range");
    }

    return ProxyElement(list.at(static_cast<std::size_t>(index)));
}

std::size_t PythonList::size() const {
    return list.size();
}

void PythonList::clear() {
    list.clear() ;
}

bool PythonList::empty() const {
    return list.empty() ;
}

std::ostream& operator<<(std::ostream& os, const PythonList& pl) {
    os << "[";

    for (std::size_t i = 0; i < pl.list.size(); ++i) {
        if (i > 0) {
            os << ", ";
        }

        const std::any& value = pl.list[i];

        auto it = pl.printers.find(std::type_index(value.type()));

        if (it != pl.printers.end()) {
            it->second(os, value);
        } else {
            os << "<unsupported type>";
        }
    }

    os << "]";
    return os;
}


std::ostream& operator<<(
        std::ostream& os,
        const PythonList::ProxyElement& proxy
) {
    proxy.print(os);
    return os;
}

void PythonList::ProxyElement::print(std::ostream& os) const {
    auto it = PythonList::printers.find(
        std::type_index(data.type())
    );

    if (it != PythonList::printers.end()) {
        it->second(os, data);
    } else {
        os << "<unsupported type>";
    }
}

void PythonList::insert(std::ptrdiff_t index , const char* data) {
    std::ptrdiff_t n = static_cast<std::ptrdiff_t>(list.size());

        if (index < 0) {
            index += n;
            if (index < 0)
                index = 0;
        } else if (index > n) {
            index = n;
        }

        list.insert(
            list.begin() + index,
            std::any(std::in_place_type<std::string>, data)
        );

        registerPrinter<std::string>();
}

std::any PythonList::pop(std::ptrdiff_t index) {
    if (list.empty())
        throw std::out_of_range("PythonList: pop from empty list");

    std::ptrdiff_t n = static_cast<std::ptrdiff_t>(list.size());

    if (index < 0)
        index += n;

    if (index < 0 || index >= n)
        throw std::out_of_range("PythonList: pop index out of range");

    auto it = list.begin() + index;

    std::any result = std::move(*it);
    list.erase(it);

    return result;
}

bool PythonList::contains(const char* value) const {
    return(contains(std::string(value))) ;
}

void PythonList::remove(const char* value) {
    remove(std::string(value)) ;
}

void PythonList::extend(const PythonList& other) {
    if (this == &other) {
        std::vector<std::any> copy = list;
        list.insert(list.end(), copy.begin(), copy.end());
        return;
    }

    list.insert(list.end(), other.list.begin(), other.list.end());
}

PythonList& PythonList::operator+=(const PythonList& other) {
    this->extend(other);
    return *this ;
}

PythonList PythonList::operator+(const PythonList& other) const {
    PythonList result = *this;
    result.extend(other);
    return result;
}

PythonList PythonList::slice(
    std::ptrdiff_t start,
    std::ptrdiff_t stop,
    std::ptrdiff_t step
) const {
    if (step == 0) {
        throw std::invalid_argument(
            "PythonList: slice step cannot be zero"
        );
    }

    std::ptrdiff_t n = static_cast<std::ptrdiff_t>(list.size());

    if (step > 0) {
        if (start < 0) start += n;
        if (stop < 0) stop += n;

        start = std::clamp(start, std::ptrdiff_t{0}, n);
        stop = std::clamp(stop, std::ptrdiff_t{0}, n);
    }
    else {
        if (start < 0) start += n;
        if (stop < 0) stop += n;

        start = std::clamp(start, std::ptrdiff_t{-1}, n - 1);
        stop = std::clamp(stop, std::ptrdiff_t{-1}, n - 1);
    }

    PythonList result;

    if (step > 0) {
        for (std::ptrdiff_t i = start; i < stop;) {
            result.list.emplace_back(list.at(
                static_cast<std::size_t>(i)
            ));

            if (step >= stop - i) break;
            i += step;
        }
    }
    else {
        for (std::ptrdiff_t i = start; i > stop;) {
            result.list.emplace_back(list.at(
                static_cast<std::size_t>(i)
            ));

            if (step <= stop - i) break;
            i += step;
        }
    }

    return result;
}