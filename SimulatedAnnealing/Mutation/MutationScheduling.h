#include "MutationBase.h"

class MutationScheduling : public MutationBase {
public:
    MutationScheduling() = default;

    virtual void modifySolution(SolutionBase& solution) override; 

    virtual ~MutationScheduling() = default;
};
