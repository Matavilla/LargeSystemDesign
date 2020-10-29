#include "MutationScheduling.h"
#include "SolutionScheduling.h"


void MutationScheduling::modifySolution(SolutionBase& solution) {
    SolutionScheduling& sol = dynamic_cast<SolutionScheduling&>(solution);

    std::mt19937 engine_1;
    std::mt19937 engine_2;

    engine_1.seed(std::random_device()());
    engine_2.seed(std::random_device()());

    std::uniform_int_distribution<size_t> dist_1(0, sol.scheduling.size() - 1);

    size_t proc_1 = dist_1(engine_1);
    size_t proc_2 = dist_1(engine_2);
    if (proc_1 == proc_2 || sol.scheduling[proc_1].size() == 0) {
        return;
    }
    
    std::uniform_int_distribution<size_t> dist_2(0, sol.scheduling[proc_1].size() - 1);
    
    size_t taskIt = dist_2(engine_2);
    
    size_t i = 0;
    while (i < sol.scheduling[proc_2].size() && 
            *(sol.Singleton)[sol.scheduling[proc_2][i]] < *(sol.Singleton)[sol.scheduling[proc_1][taskIt]]) {
        i++;
    }

    sol.scheduling[proc_2].insert(sol.scheduling[proc_2].begin() + i, taskIt);
    sol.scheduling[proc_1].erase(sol.scheduling[proc_1].begin() + taskIt);
}
