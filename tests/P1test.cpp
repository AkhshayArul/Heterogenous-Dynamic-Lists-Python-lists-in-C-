#include "List.h"

int main() {
    PythonList list;

    PythonList original ;
    original.append(10);
    original.append(20.5);
    original.append("String") ;

    int a = original[0];

    std::cout << original[2] ;

    list.append(original);
    list.appendReference(original);

    std::vector<int> x = {1,2,3};
    original.appendReference(x);

    std::cout << list << std::endl ;

}