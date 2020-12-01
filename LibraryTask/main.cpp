#include "Function.h"

#include <iostream>

int main() {
    TFuncFactory r;
    auto a1 = r.CreateFunction("exp", {1, 2, 3});
    auto a2 = r.CreateFunction("ident", {0, 0});
    auto a3 = r.CreateFunction("const", {1.5});
    auto a4 = r.CreateFunction("power", {4, 5, 3});
    auto a5 = r.CreateFunction("polynomial", {1, 2, 1});
    auto a6 = r.CreateFunction("exp", {8, 10.99, 3});
    
    {std::cout << (*a1 + *a2).ToString()<<std::endl;}
    {std::cout << (*a2 + *a2 + *a3 + *a4).ToString()<<std::endl;}
    {std::cout << ((*a1 * *a2 - *a5) / *a3).ToString()<<std::endl;}
    auto a7 = ((*a1 + *a3) * *a6) / *a1;
    std::cout << a7(0)<<std::endl;
    std::cout << a7.GetDerive(0)<<std::endl;
    std::cout << FindRoot(*a5, -0.5, 2) <<std::endl;
}
