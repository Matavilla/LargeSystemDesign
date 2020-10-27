#include "SolutionBase.h"
#include "MutationBase.h"
#include "TemperatureBase.h"

#include <memory>


template<class S, class M, class T>
class SaSolver {
    std::unique_ptr<SolutionBase> curSolution;
    std::unique_ptr<SolutionBase> bestSolution;

    std::unique_ptr<MutationBase> mutation;
    std::unique_ptr<TemperatureBase> temperature;


    double initTemperature() {
    }

public:
    SaSolver() = delete;

    SaSolver(const std::string& path) {
        curSolution = std::make_unique<S>(path, true);
        bestSolution = std::make_unique<S>(path, false);
        mutation = std::make_unique<M>();
        temperature = std::make_unique<T>();
    }

    void updateSolution(const SolutionBase& sol) {
        curSolution->updateSolution(sol);
        bestSolution->updateSolution(sol);
    }

    const SolutionBase& getSolution() const {
        return *(bestSolution.get());
    }

    void start() {
    }

    virtual ~SaSolver() = default;
};
