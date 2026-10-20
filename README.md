# Heterogeneous Dynamic Lists (Python Lists in C++)

## Overview

Implementation of Python-like lists in C++ that allow heterogeneous data type storage.

## Implementation

The implementation uses a vector of std::any objects and an ElementProxy class to return the stored data, which can then be resolved by the user into the appropriate data type .

This method allows the user to access data using the following syntax:

cpp
int a = Pylist[index].getValue<int>();


## Usage

The end product is an includable library that can be used as follows:

cpp
#include <PythonList.h>


### Current Usage 

The project is currently under development . To test its features before the entire project is done and made into a package follow the following steps .

#### 1. Clone the Repository

Clone the project from GitHub using the following command:

bash
git clone https://github.com/AkhshayArul/Heterogenous-Dynamic-Lists-Python-lists-in-C-.git


#### 2. Navigate to the Project Directory

bash
cd Heterogenous-Dynamic-Lists-Python-lists-in-C-


#### 3. Compile the Project

Code the features you need to test in main.cpp .

Compile the source files along with your main program using a C++17-compatible compiler.

For example, using g++:

bash
g++ -std=c++17 -Iinclude main.cpp src/List.cpp -o PythonList


#### 4. Run the Program

On Windows:

bash
PythonList.exe


On Linux or macOS:

bash
./PythonList


#### 5. Use the Library

Include the header in your C++ program:

cpp
#include "List.h"


You can then create a PythonList object and use its available features. Refer to the project documentation for details about the supported operations and their usage.

## Requirements

Before using this project, ensure you have the following:

- *C++17-compatible compiler:* Required for features such as std::any.
- *C++ standard library:* A standard library implementation that supports C++17.
- *Terminal or command-line interface:* To compile and run the program.

No external libraries are required; the project uses the C++ standard library.

## Documentation

For further information on the detailed plan and thought processes, refer to the Documentation folder.
