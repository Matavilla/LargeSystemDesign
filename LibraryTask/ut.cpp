#include "gtest/gtest.h"
#include "Function.h"

TEST(Function, Init) {
    TFuncFactory f;
    auto exp = f.CreateFunction("exp", {1.5, 2.7});
    auto ident = f.CreateFunction("ident", {});
    auto power1 = f.CreateFunction("power", {0, 4});
    auto power2 = f.CreateFunction("power", {1, 1});
    auto power3 = f.CreateFunction("power", {4, 0});
    auto power4 = f.CreateFunction("power", {2, 5});
    auto c = f.CreateFunction("const", {-14.3});
    auto pol = f.CreateFunction("polynomial", {0, 1, 2});

    ASSERT_STREQ(exp->ToString().c_str(), "1.500000*e^2.700000x");
    ASSERT_STREQ(ident->ToString().c_str(), "x"); 
    ASSERT_STREQ(power1->ToString().c_str(), "0");
    ASSERT_STREQ(power2->ToString().c_str(), "x");
    ASSERT_STREQ(power3->ToString().c_str(), "4.000000");
    ASSERT_STREQ(power4->ToString().c_str(), "2.000000*x^5.000000");
    ASSERT_STREQ(c->ToString().c_str(), "-14.300000");
    ASSERT_STREQ(pol->ToString().c_str(), "(x) + (2.000000*x^2.000000)");
};

TEST(Function, Operation) {
    TFuncFactory f;
    auto exp = f.CreateFunction("exp", {1.5, 2.7});
    auto ident = f.CreateFunction("ident", {});
    auto power1 = f.CreateFunction("power", {0, 4});
    auto power2 = f.CreateFunction("power", {1, 1});
    auto power3 = f.CreateFunction("power", {4, 0});
    auto power4 = f.CreateFunction("power", {2, 5});
    auto c = f.CreateFunction("const", {-14.3});
    auto pol = f.CreateFunction("polynomial", {0, 1, 2});

    ASSERT_NEAR((*exp)(0), 1.5, 0.00001);
    ASSERT_NEAR((*ident)(2), 2.0, 0.00001); 
    ASSERT_NEAR((*power1)(7), 0, 0.00001);
    ASSERT_NEAR((*power2)(7), 7, 0.00001);
    ASSERT_NEAR((*power3)(1), 4, 0.00001);
    ASSERT_NEAR((*power4)(1), 2, 0.00001);
    ASSERT_NEAR((*c)(0), -14.3, 0.00001);
    ASSERT_NEAR((*pol)(1), 3, 0.00001);

    ASSERT_NEAR(exp->GetDerive(0), 4.05, 0.00001);
    ASSERT_NEAR(ident->GetDerive(2), 1, 0.00001); 
    ASSERT_NEAR(power1->GetDerive(7), 0, 0.00001);
    ASSERT_NEAR(power2->GetDerive(7), 1, 0.00001);
    ASSERT_NEAR(power3->GetDerive(1), 0, 0.00001);
    ASSERT_NEAR(power4->GetDerive(1), 10, 0.00001);
    ASSERT_NEAR(c->GetDerive(0), 0, 0.00001);
    ASSERT_NEAR(pol->GetDerive(1), 5, 0.00001);

    auto complex1 = *exp + *pol;
    auto complex2 = *ident - *pol;
    auto complex3 = *c * *pol;
    auto complex4 = *power1 / *power2;
    auto complex5 = (complex1 + complex2);
    try {
        complex5 + std::string("");
    } catch(std::logic_error e) {
    }

    ASSERT_NEAR(complex1(1), 25.319597, 0.0001);
    ASSERT_NEAR(complex2(1), -2, 0.00001);
    ASSERT_NEAR(complex3(1), -42.9, 0.00001);
    ASSERT_NEAR(complex4(1), 0, 0.00001);
    ASSERT_NEAR(complex5(1), 23.319597, 0.0001);

    ASSERT_NEAR(complex1.GetDerive(0), 5.05, 0.00001);
    ASSERT_NEAR(complex2.GetDerive(1), -4, 0.00001);
    ASSERT_NEAR(complex3.GetDerive(0), -14.3, 0.00001);
    ASSERT_NEAR(complex4.GetDerive(1), 0, 0.0001);
};

TEST(Function, FindRoot) {
    TFuncFactory f;
    auto pol1 = f.CreateFunction("polynomial", {0, 1});
    auto pol2 = f.CreateFunction("polynomial", {1, 2, 1});

    ASSERT_NEAR(FindRoot(*pol1, -0.5, 80), -1, 0.00001);
    ASSERT_NEAR(FindRoot(*pol2, 10, 8), 0, 0.0001);
};
