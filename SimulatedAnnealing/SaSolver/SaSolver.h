#include "SolutionBase.h"
#include "MutationBase.h"
#include "TemperatureBase.h"

#include <memory>
#include <random>
#include <cmath>

template<class S, class M, class T>
class SaSolver {
    std::unique_ptr<SolutionBase> tmpSolution;
    std::unique_ptr<SolutionBase> curSolution;
    std::unique_ptr<SolutionBase> bestSolution;

    std::unique_ptr<MutationBase> mutation;
    std::unique_ptr<TemperatureBase> temperature;

    const size_t minT;
public:
    SaSolver() = delete;

    SaSolver(const std::string& path, const double& minTemp, const double& maxTemp) : minT(minTemp) {
        curSolution = std::make_unique<S>(path, true);

        bestSolution = std::make_unique<S>(path, false);
        bestSolution->updateSolution(curSolution);

        tmpSolution = std::make_unique<S>(path, false);

        mutation = std::make_unique<M>();

        temperature = std::make_unique<T>();
        temperature->set_temp(maxTemp);
    }

    void updateSolution(const SolutionBase& sol) {
        curSolution->updateSolution(sol);
        bestSolution->updateSolution(sol);
    }

    const SolutionBase& getSolution() const {
        return *(bestSolution.get());
    }

    double P(double& dE, double& t) {
        return std::exp(-dE / t);
    }

    void start() {
        std::mt19937 engine;
        engine.seed(std::random_device()());
        std::uniform_real_distribution<double> dist(0, 1);

        const size_t MAX_ITERATION = 1000000;
        size_t iteration = 0;
        while(temperature->get_temp(iteration) > minT && iteration < MAX_ITERATION) {
            tmpSolution->updateSolution(curSolution);
            mutation->changeSolution(tmpSolution);

            double dE = tmpSolution->getEnergy() - curSolution->getEnergy();

            if(dE < 0 || dist(engine) < P(dE, temperature->get_temp(iteration))) {
                curSolution->updateSolution(tmpSolution);
            }

            if(curSolution->getEnergy() < bestSolution->getEnergy()) {
                bestSolution->updateSolution(curSolution);
            }
            iteration++;
        }
    }

    virtual ~SaSolver() = default;
};
