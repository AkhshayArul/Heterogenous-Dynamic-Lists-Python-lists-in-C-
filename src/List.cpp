#include "List.h"

std::unordered_map<
        std::type_index,
        std::function<void(std::ostream&, const std::any&)>
    > PythonList::printers;
    
void PythonList::append(const char* data) {
    list.emplace_back(std::string(data));
    registerPrinter<std::string>() ;
}

PythonList::ProxyElement PythonList::operator[](std::size_t index) {
    return ProxyElement(list.at(index));
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