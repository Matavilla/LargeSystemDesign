#pragma once

#include "SolutionBase.h"

class MutationBase {
public:
    MutationBase() = default;

    void void modifySolution(SolutionBase& solution) = 0;

    virtual ~MutationBase() = default;
};
