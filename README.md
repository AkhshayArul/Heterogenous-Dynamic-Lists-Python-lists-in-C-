# Heterogenous-Dynamic-Lists-Python-lists-in-C-
Implementation of Python like lists allowing heterogenous data type storage in C++.

The implementation can be done using two major ways .

Method 1 :
  Using a vector of std::any objects and using proxy classes to return the stored data which can then be resolved by the user . 
  This method allows the user to access the data as : 
    int a = Pylist[<index>].getValue<int>()

 Method 2 :
  Using a vector of custom Element derived classes for (one class for one data type) derived from a single class (Polymorphism)
  This method allows the user to access the data as :
    int a = Pylist[<index>]->getValue() ;
  
This project aims to implement the result majorly via method 2 .

The end product is an includable like #include <PythonList> and #include <PythonList_any>

For further information on detailed plan and thought processes refer Documentation folder . 
