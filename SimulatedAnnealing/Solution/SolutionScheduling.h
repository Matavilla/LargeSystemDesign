#pragma once

#include "SolutionBase.h"

#include <vector>
#include <string>


class Singleton {
    Singleton(const std::string& path);

    static Singleton* singleton_;

    std::vector<long long> value;
public:
    Singleton() = delete;

    Singleton(Singleton& other) = delete;

    void operator=(const Singleton&) = delete;

    static Singleton* getInstance(const std::string& file) {
        if(singleton_ == nullptr) {
            singleton_ = new Singleton(file);
        }
        return singleton_;
    }

    size_t size() const {
        return value.size();
    }

    const long long& operator[](const size_t& i) {
        return value[i];
    }
};

class SolutionScheduling : public SolutionBase {
    Singleton* tasksTime;
    std::vector<std::vector<size_t>> scheduling;
public:
    SolutionScheduling() = delete;

    SolutionScheduling(const std::string& path, bool initFlag = true);

    auto& getAns() {
        return scheduling;
    }

    virtual void consumeEnergy() override;

    virtual void init() override;

    virtual void updateSolution(SolutionBase* s) override;

    virtual ~SolutionScheduling() = default;

    friend class MutationScheduling;
};
