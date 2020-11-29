#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <initializer_list>
#include <exception>
#include <utility>
#include <memory>
#include <type_traits>

class IFunction {
public:
    virtual double operator()(const double& x) const = 0;
    virtual double GetDerive(const double& x) const = 0;
    virtual std::string ToString() const = 0;
    virtual ~IFunction() = default;
};

class TPower : public IFunction {
    // a * x ^ b or a * e ^ bx
protected:
    double a;
    double b;

    enum class TType {
        POWER,
        EXP
    };
    TType t;
public:
    TPower(const std::initializer_list<double>& p) : t(TType::POWER) {
        if (p.size() == 1) {
            b = *p.begin();
        } else if (p.size() >= 2) {
            a = *p.begin();
            b = *(p.begin() + 1);
        } else {
            throw std::logic_error("bad init");
        }
    }

    TPower(const double& a_ = 0.0, const double& b_ = 1.0, TType t_ = TType::POWER) : a(a_), b(b_), t(t_) {
    }

    double operator()(const double& x) const override {
        return a * ((t == TType::POWER) ? std::pow(x, b) : std::exp(x * b));
    }

    double GetDerive(const double& x) const override {
        return a * b * ((t == TType::POWER) ? std::pow(x, b - 1) : std::exp(x * b));
    }

    std::string ToString() const override {
        if (std::abs(a - 0.0) < 0.000001) {
            return std::to_string(0);
        }
        std::string ans;
        if (std::abs(b - 0.0) < 0.000001) {
            return std::to_string(a);
        } else if (std::abs(a - 1.0) >= 0.000001) {
            ans += std::to_string(a) +"*";
        }
        ans += (t == TType::POWER) ? "x" : "e";
        if (std::abs(b - 1.0) >= 0.000001) {
            ans += "^" + std::to_string(b);
            if (t == TType::EXP) {
                ans += "x";
            }
        }
        return ans;
    }
};

class TExp : public TPower {
public:
    TExp(const std::initializer_list<double>& p) : t(TType::EXP) {
        if (p.size() == 1) {
            b = *p.begin();
        } else if (p.size() >= 2) {
            a = *p.begin();
            b = *(p.begin() + 1);
        } else {
            a = 1.0;
            b = 1.0;
        }
    }

    TExp() : TPower(1.0, 1.0, TType::EXP) {
    }
};

class TConst : public TPower {
public:
    TConst(const std::initializer_list<double>& p) {
        if (p.size() >= 1) {
            a = *p.begin();
        } else {
            throw std::logic_error("bad init");
        }
    }

    TConst(const double& x) : TPower(x, 0.0) {
    }
};

class TIdent : public TPower {
public:
    TIdent(const std::initializer_list<double>& p) : TPower(1.0, 1.0) {
    }

    TConst() : TPower(1.0, 1.0) {
    }
};

class TPolynomial : public IFunction {
    std::vector<TPower> vec;
public:
    TPolynomial(const std::initializer_list<double>& p) {
        unsigned i = 0;
        for (auto& j : p) {
            vec.emplace_back(j, i++);
        }
    }

    TPolynomial(const std::initializer_list<std::pair<double, double>>& p) {
        for (auto& i : p) {
            vec.emplace_back(i.first, i.second);
        }
    }

    double operator()(const double& x) const override {
        double ans = 0.0;
        for (auto& i : vec) {
            ans += i(x);
        }
        return ans;
    }

    double GetDerive(const double& x) const override {
        double ans = 0.0;
        for (auto& i : vec) {
            ans += i.GetDerive(x);
        }
        return ans;
    }

    std::string ToString() const {
        std::string ans;
        for (size_t i = 0; i < vec.size(); i++) {
            if (vec[i].ToString() != "0") {
                ans += "(" + vec[i].ToString() + ")";
                if (i + 1 != vec.size()) {
                    ans += " + ";
                }
            }
        }
        if (ans.empty()) {
            ans += "0";
        }
        return ans;
    }
};

using IFunctionPtr = std::shared_ptr<IFunction>;

class TComplexFunction : public IFunction {
    enum class TOper {
        ADD, MUL, DIV, SUB, END
    };
    std::vector<std::pair<IFunctionPtr, TOper>> vec;
public:
    double operator()(const double& x) const {
        auto i = vec.begin();
        double ans = i->first->(x);
        while (i->second != TOper::END) {
            if (i->second == TOper::ADD) {
                ans += (i + 1)->first->(x);
            } else if (i->second == TOper::SUB) {
                ans -= (i + 1)->first->(x);
            } else if (i->second == TOper::MUL) {
                ans *= (i + 1)->first->(x);
            } else if (i->second == TOper::DIV) {
                ans /= (i + 1)->first->(x);
            }
            i++;
        }
        return ans;
    }

    double GetDerive(const double& x) const {
        return ((*this)(x + 0.0000001) - (*this)(x)) / (0.0000001);
    }

    std::string ToString() const {
        auto i = vec.begin();
        std::string ans;
        for (size_t j = 0; j < vec.size(); j++) {
            ans += "(";
        }
        ans += i->first->ToString();
        while (i->second != TOper::END) {
            if (i->second == TOper::ADD) {
                ans += " + "; 
            } else if (i->second == TOper::SUB) {
                ans += " - "; 
            } else if (i->second == TOper::MUL) {
                ans += " * "; 
            } else if (i->second == TOper::DIV) {
                ans += " / "; 
            }
            ans += (i + 1)->first->ToString() + ")";
            i++;
        }
        return ans;
    }
};

class TFuncFactory {
    class TImpl;

    TImpl Impl;
public:
    IFunctionPtr CreateFunction(const std::string& type, const std::initializer_list<double> param) const {
        return impl.CreateFunction(type, param);
    }

    std::vector<std::string> GetAvailableFunctions() const {
        return impl.GetAvailableFunctions();
    }
};

template <class T>
double FindRoot(T f, double x0 = 2.0, unsigned it = 10000) {
    static_assert(std::is_base_of<IFunction, T>::value, "wrong type for FindRoot");
    auto f2 = f * f;
    for (unsigned i = 1; i <= it; i++) {
        double j = 1.0 / i;
        x0 = x0 - j * f2.GetDeriv(x0); 
    }
    return x0;
}
