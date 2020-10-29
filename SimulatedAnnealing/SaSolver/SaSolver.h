#include "SolutionBase.h"
#include "MutationBase.h"
#include "TemperatureBase.h"

#include <memory>
#include <random>
#include <chrono>
#include <cmath>
#include <iostream>

template<class S, class M, class T>
class SaSolver {
    std::unique_ptr<SolutionBase> tmpSolution;
    std::unique_ptr<SolutionBase> curSolution;
    std::unique_ptr<SolutionBase> bestSolution;

    std::unique_ptr<MutationBase> mutation;
    std::unique_ptr<TemperatureBase> temperature;
public:
    SaSolver() = delete;

    SaSolver(const std::string& path, const double& maxTemp) {
        curSolution = std::make_unique<S>(path, true);

        bestSolution = std::make_unique<S>(path, false);
        bestSolution->updateSolution(*curSolution.get());

        tmpSolution = std::make_unique<S>(path, false);

        mutation = std::make_unique<M>();

        temperature = std::make_unique<T>();
        temperature->set_temp(maxTemp);
    }

    void updateSolution(const SolutionBase& sol) {
        curSolution->updateSolution(sol);
        bestSolution->updateSolution(sol);
    }

    SolutionBase& getSolution() const {
        return *(bestSolution.get());
    }

    double P(const double& dE, const double& t) const {
        return std::exp(-dE / t);
    }

    void start() {
        std::mt19937 engine;
        engine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<double> dist(0, 1);

        const size_t MAX_ITERATION_OUT = 4000000000;
        const size_t MAX_TIME_UPDATE_SOLUTION = 100;
        const size_t MAX_ITERATION_IN = 100;

        size_t lastUpdateSolution = 0;
        size_t iteration = 0;
        while (lastUpdateSolution < MAX_TIME_UPDATE_SOLUTION && iteration < MAX_ITERATION_OUT) {
            for (size_t j = 0; j < MAX_ITERATION_IN; j++) {
                tmpSolution->updateSolution(*curSolution.get());
                mutation->modifySolution(*tmpSolution.get());

                double dE = tmpSolution->getEnergy() - curSolution->getEnergy();

                if (dE < 0 || dist(engine) < P(dE, temperature->get_temp(iteration))) {
                    curSolution->updateSolution(*tmpSolution.get());
                }

                if (curSolution->getEnergy() < bestSolution->getEnergy()) {
                    bestSolution->updateSolution(*curSolution.get());
                    lastUpdateSolution = 0;
                }
            }
            lastUpdateSolution++;
            iteration++;
        }
    }

    virtual ~SaSolver() = default;
};
