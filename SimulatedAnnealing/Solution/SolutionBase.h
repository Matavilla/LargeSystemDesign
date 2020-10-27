#pragma once

class SolutionBase {
public:
    virtual void updateSolution(const SolutionBase&) = 0;

    virtual double getEnergy() const = 0;

    virtual ~SolutionBase() = default;
};
