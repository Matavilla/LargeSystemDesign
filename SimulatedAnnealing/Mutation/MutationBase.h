#pragma once

#include "SolutionBase.h"

class MutationBase {
public:
    MutationBase() = default;

    virtual void modifySolution(SolutionBase& solution) = 0;

    virtual ~MutationBase() = default;
};
