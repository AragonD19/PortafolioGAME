// include/print.h

#pragma once
#include <raylib.h>
#include <iostream>
#define vprint(var) print(#var ":", var)

// empty function to print newlines
inline void print() {
    std::cout << std::endl;
}

// base case function to end the recursion
inline void print(auto one, bool last = true) {
    std::cout << one;
    if (last) {
        std::cout << std::endl;
    }
}

// recursive variadic template function
inline void print(auto first, auto... args) {
    print(first, false);  // call the appropriate print function

    if constexpr (sizeof...(args) > 0) {
        std::cout << ' ';  // print a space only if there are more arguments
        print(args...);    // call print with remaining arguments
    } else {
        std::cout << std::endl;
    }
}

// Overload for Raylib's Vector2
inline void print(Vector2 v) {
    std::cout << "V2(" << v.x << ", " << v.y << ")" << std::endl;
}

// Overload for Raylib's Vector3
inline void print(Vector3 v) {
    std::cout << "V3(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
}
