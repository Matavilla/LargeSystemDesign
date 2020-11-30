#include "Function.h"

#include <iostream>

int main() {
    TFuncFactory r;
    auto a1 = r.CreateFunction("exp", {1, 2, 3});
    auto a2 = r.CreateFunction("ident", {0, 0});
    auto a3 = r.CreateFunction("const", {1.5});
    auto a4 = r.CreateFunction("power", {4, 5, 3});
    auto a5 = r.CreateFunction("polynomial", {1, 2, 3});
    auto a6 = r.CreateFunction("exp", {8, 10.99, 3});
    std::cout << a1->ToString();
    std::cout << a2->ToString();
    std::cout << a3->ToString();
    std::cout << a4->ToString();
    std::cout << a5->ToString();
    std::cout << a6->ToString();
}
