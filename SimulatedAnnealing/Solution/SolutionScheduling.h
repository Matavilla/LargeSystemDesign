#include "SolutionBase.h"

#include <vector>
#include <string>


class SolutionScheduling : public SolutionBase {
    std::vector<double> tasksTime;
    std::vector<std::vector<size_t>> scheduling;

    void init();
public:
    SolutionScheduling() = delete;

    SolutionScheduling(const std::string& path, bool initFlag = true);

    virtual void updateSolution(const SolutionBase& s) override;

    virtual double getEnergy() const override;

    virtual ~SolutionScheduling() = default;
};
