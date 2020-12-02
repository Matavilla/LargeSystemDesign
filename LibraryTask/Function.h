#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
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
    virtual IFunction* Clone() const = 0;
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

    TPower(const double& a_ = 1.0, const double& b_ = 0.0, TType t_ = TType::POWER) : a(a_), b(b_), t(t_) {
    }

    IFunction* Clone() const override {
        return new TPower(*this);
    }

    double operator()(const double& x) const override {
        double ans = a;
        if (std::abs(ans - 0) < 0.00001) {
            return ans;
        }
        return ans * ((t == TType::POWER) ? std::pow(x, b) : std::exp(x * b));
    }

    double GetDerive(const double& x) const override {
        double ans = a * b;
        if (std::abs(ans - 0) < 0.00001) {
            return ans;
        }
        return ans * ((t == TType::POWER) ? std::pow(x, b - 1) : std::exp(x * b));
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
        } else if (t == TType::EXP) {
            ans += "^x";
        }
        return ans;
    }

    virtual ~TPower() = default;
};

class TExp : public TPower {
public:
    TExp(const std::initializer_list<double>& p){
        t = TType::EXP;
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

    TIdent() : TPower(1.0, 1.0) {
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

    IFunction* Clone() const override {
        return new TPolynomial(*this);
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

using IFunctionPtr = std::unique_ptr<IFunction>;

class TComplexFunction : public IFunction {
public:
    enum class TOper {
        ADD, MUL, DIV, SUB, END
    } Type;
    IFunctionPtr A;
    IFunctionPtr B;

    TComplexFunction(const IFunction& a, const IFunction& b, const TOper t) : A(a.Clone()), B(b.Clone()), Type(t) {
    }

    double operator()(const double& x) const override {
        double ans = (*A)(x);
        if (Type == TOper::ADD) {
            ans += (*B)(x);
        } else if (Type == TOper::SUB) {
            ans -= (*B)(x);
        } else if (Type == TOper::MUL) {
            ans *= (*B)(x);
        } else if (Type == TOper::DIV) {
            ans /= (*B)(x);
        }
        return ans;
    }

    TComplexFunction(const TComplexFunction& a) {
        A = IFunctionPtr(a.A->Clone());
        B = IFunctionPtr(a.B->Clone());
        Type = a.Type; 
    }

    IFunction* Clone() const override {
        return new TComplexFunction(*this);
    }

    double GetDerive(const double& x) const override {
        double ans = 0.0;
        if (Type == TOper::ADD) {
            ans = A->GetDerive(x) + B->GetDerive(x); 
        } else if (Type == TOper::SUB) {
            ans = A->GetDerive(x) - B->GetDerive(x); 
        } else if (Type == TOper::MUL) {
            ans = (*A)(x) * B->GetDerive(x) + (*B)(x) * A->GetDerive(x);
        } else if (Type == TOper::DIV) {
            ans = (*A)(x) * B->GetDerive(x) - (*B)(x) * A->GetDerive(x);
            ans /= (*B)(x) * (*B)(x);
        }
        return ans;
    }

    std::string ToString() const override {
        std::string ans;
        ans += "(" + A->ToString();
        if (Type == TOper::ADD) {
            ans += " + "; 
        } else if (Type == TOper::SUB) {
            ans += " - "; 
        } else if (Type == TOper::MUL) {
            ans += " * "; 
        } else if (Type == TOper::DIV) {
            ans += " / "; 
        }
        ans += B->ToString() + ")";
        return ans;
    }

    virtual ~TComplexFunction() = default;
};

class TFuncFactory {
    class TImpl {
        class ICreator {
        public:
            virtual ~ICreator() = default;
            virtual IFunctionPtr Create(const std::initializer_list<double>& p) const = 0;
        };

        using ICreatorPtr = std::shared_ptr<ICreator>;
        using IRegisteredCreators = std::map<std::string, ICreatorPtr>;

        IRegisteredCreators RegisteredCreators;
    public:
        TImpl() {
            RegisterAll();
        }

        template<typename TCurrentFunction>
        class TCreator: public ICreator {
        public:
            IFunctionPtr Create(const std::initializer_list<double>& p) const override {
                return std::make_unique<TCurrentFunction>(p);
            }
        };

        template<typename T>
        void RegisterCreator(const std::string& type) {
            RegisteredCreators[type] = std::make_shared<TCreator<T>>();
        }

        void RegisterAll() {
            RegisterCreator<TPolynomial>("polynomial");
            RegisterCreator<TConst>("const");
            RegisterCreator<TIdent>("ident");
            RegisterCreator<TPower>("power");
            RegisterCreator<TExp>("exp");
        }

        IFunctionPtr CreateFunction(const std::string& type, const std::initializer_list<double>& p) const {
            auto creator = RegisteredCreators.find(type);
            if (creator == RegisteredCreators.end()) {
                return nullptr;
            }
            return creator->second->Create(p);
        }

        std::vector<std::string> GetAvailableFunctions() const {
            std::vector<std::string> result;
            for (const auto& type : RegisteredCreators) {
                result.push_back(type.first);
            }
            return result;
        }
    } Impl;
public:
    IFunctionPtr CreateFunction(const std::string& type, const std::initializer_list<double> param) const {
        return Impl.CreateFunction(type, param);
    }

    std::vector<std::string> GetAvailableFunctions() const {
        return Impl.GetAvailableFunctions();
    }
};

TComplexFunction operator+(const IFunction& a, const IFunction& b) {
    return TComplexFunction(a, b, TComplexFunction::TOper::ADD);
}

TComplexFunction operator-(const IFunction& a, const IFunction& b) {
    return TComplexFunction(a, b, TComplexFunction::TOper::SUB);
}

TComplexFunction operator*(const IFunction& a, const IFunction& b) {
    return TComplexFunction(a, b, TComplexFunction::TOper::MUL);
}

TComplexFunction operator/(const IFunction& a, const IFunction& b) {
    return TComplexFunction(a, b, TComplexFunction::TOper::DIV);
}

template <class T>
typename std::enable_if<!std::is_base_of<IFunction, T>::value, TComplexFunction>::type operator+(const IFunction& a, const T& b) {
    throw std::logic_error("This text the same strange as this operator");
}

template <class T>
typename std::enable_if<!std::is_base_of<IFunction, T>::value, TComplexFunction>::type operator-(const IFunction& a, const T& b) {
    throw std::logic_error("This text the same strange as this operator");
}

template <class T>
typename std::enable_if<!std::is_base_of<IFunction, T>::value, TComplexFunction>::type operator*(const IFunction& a, const T& b) {
    throw std::logic_error("This text the same strange as this operator");
}

template <class T>
typename std::enable_if<!std::is_base_of<IFunction, T>::value, TComplexFunction>::type operator/(const IFunction& a, const T& b) {
    throw std::logic_error("This text the same strange as this operator");
}

template <class T>
typename std::enable_if<!std::is_base_of<IFunction, T>::value, TComplexFunction>::type operator+(const T& b, const IFunction& a) {
    throw std::logic_error("This text the same strange as this operator");
}

template <class T>
typename std::enable_if<!std::is_base_of<IFunction, T>::value, TComplexFunction>::type operator-(const T& b, const IFunction& a) {
    throw std::logic_error("This text the same strange as this operator");
}

template <class T>
typename std::enable_if<!std::is_base_of<IFunction, T>::value, TComplexFunction>::type operator*(const T& b, const IFunction& a) {
    throw std::logic_error("This text the same strange as this operator");
}

template <class T>
typename std::enable_if<!std::is_base_of<IFunction, T>::value, TComplexFunction>::type operator/(const T& b, const IFunction& a) {
    throw std::logic_error("This text the same strange as this operator");
}

double FindRoot(IFunction& f, double x0 = 2.0, unsigned it = 1000) {
    double fx0 = f(x0);
    for (unsigned i = 1; i <= it && std::abs(fx0) > 0.00001; i++) {
        double j = 0.01; 
        x0 = x0 + j * ((fx0 > 0) ? -f.GetDerive(x0) : f.GetDerive(x0)); 
        fx0 = f(x0);
    }
    return x0;
}

