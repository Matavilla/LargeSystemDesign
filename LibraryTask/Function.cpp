#include "Function.h"

#include <algorithm>

class TFuncFactory::TImpl {
    class ICreator {
    public:
        virtual ~ICreator() = default;
        virtual IFunctionPtr Create(const std::initializer_list<double>& p) const = 0;
    };

    using ICreatorPtr = std::shared_ptr<ICreator>;
    using RegisteredCreators = std::map<std::string, ICreatorPtr>;

    RegisteredCreators RegisteredCreators;
public:
    TImpl() {
        RegisterAll();
    }

    template<typename TCurrentFunction>
    class TCreator: public ICreator {
    public:
        IFunctionPtr Create(const std::initializer_list<double>& p) const override {
            return std::make_shared<TCurrentFunction>(p);
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
};
