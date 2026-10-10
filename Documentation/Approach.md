# Project Approach


## 1. Choosing the Implementation Method

The first step of the project was to figure out a way to implement the heterogeneous, Python-like list.

A few searches and discussions with AI bots led to three ways:

1. Use a vector of `std::any` objects to store different data types together.
2. Use a vector of custom classes which store data via a `void` pointer and have information about what data type is stored in that object.
3. Use a vector of custom classes (one class for one datatype derived from a single base class) and store and retrieve data polymorphically.

Initially, the plan was to implement both the first and third methods.

But after a little bit more pondering and researching, it turned out that retrieving the stored data wouldn't work in the way I thought was possible with the third method. The reason being, C++ rules and limitations [or the lack of my knowledge of them :( ].

So ultimately, the third method was scratched, and implementation with the first method was finalised.

## 2. Why `std::any` Was Chosen

The reason I wanted to choose the third option was that it would allow the most Python-like implementation of all methods [at least in my mind, it did so :( ]. But due to it being scratched, only the first and second options were left.

They both had very similar working at the low level. It just boiled down to choosing whether to use a built-in class and not worry about intricate memory management and safety, or create and use a custom class AND worry about memory management.

The goal of the project was to implement a Python-like list, so it was decided that we would proceed with the built-in class (`std::any` and `std::any_cast`). This would allow us to focus on making the list more and more Python-like instead of hitting our heads on internals that are very complex.

Be it as it may, this project will still require some extent of memory manipulation skills for proper function.

So that was the lore of why the third method was scratched.