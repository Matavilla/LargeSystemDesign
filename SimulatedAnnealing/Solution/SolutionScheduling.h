#include "SolutionBase.h"

#include <vector>
#include <string>


class SolutionScheduling : public SolutionBase {
    std::vector<double> tasksTime;
    std::vector<std::vector<size_t>> scheduling;
public:
    SolutionScheduling() = delete;

    SolutionScheduling(const std::string& path, bool initFlag = true);

    virtual void consumeEnergy() const override;

    virtual void init() override;

    virtual void updateSolution(const SolutionBase& s) override;

    virtual ~SolutionScheduling() = default;
};
