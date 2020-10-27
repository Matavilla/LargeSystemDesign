#pragma once

class SolutionBase {
    double e;
public:
    virtual void updateSolution(const SolutionBase&) = 0;

    virtual void consumeEnergy() const = 0;

    virtual void init() = 0;

    double getEnergy() const {
        return e;
    }

    void setEnergy(const double& r) {
        e = r;
    }

    virtual ~SolutionBase() = default;
};
