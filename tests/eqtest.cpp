#include "List.h"

int main() {
    static_assert(IsEqualityComparable<int>::value);
    static_assert(IsEqualityComparable<std::string>::value);

    struct NoEquality {};

    static_assert(!IsEqualityComparable<NoEquality>::value);

    std::cout << "All passed.\n" ;
}