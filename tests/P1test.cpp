#include "../include/List.h" 
#include <iostream>

using namespace std ;

int main() {
    PythonList list ;

    list.append(10);
    list.append(20.5);
    list.append("Wow") ;

    cout << list.size() << endl ;

    double& a = list[1].getValue<double>()  ;
    cout << a << endl ;
    a = 10.5;

    cout << list[2].getValue<string>() << endl ;

}