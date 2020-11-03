#pragma once

class SolutionBase {
    double e;
public:
    virtual void updateSolution(SolutionBase*) = 0;

    virtual void consumeEnergy() = 0;

    virtual void init() = 0;

    long long getEnergy() const {
        return e;
    }

    void setEnergy(const long long& r) {
        e = r;
    }

    virtual ~SolutionBase() = default;
};
