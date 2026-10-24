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