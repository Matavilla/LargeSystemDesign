#include "MutationScheduling.h"
#include "SolutionScheduling.h"

#include <iostream>
#include <random>
#include <chrono>

void MutationScheduling::modifySolution(SolutionBase& solution) {
    SolutionScheduling& sol = dynamic_cast<SolutionScheduling&>(solution);

    std::mt19937 engine;
    engine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    std::uniform_int_distribution<size_t> dist_1(0, sol.scheduling.size() - 1);

    size_t proc_1, proc_2;
    do {
        proc_1 = dist_1(engine);
        proc_2 = dist_1(engine);
    } while (proc_1 == proc_2 || sol.scheduling[proc_1].size() == 0);
    
    std::uniform_int_distribution<size_t> dist_2(0, sol.scheduling[proc_1].size() - 1);
    
    size_t taskIt = dist_2(engine);
    
    size_t i = 0;
    while (i < sol.scheduling[proc_2].size() && 
            (*sol.tasksTime)[sol.scheduling[proc_2][i]] < (*sol.tasksTime)[sol.scheduling[proc_1][taskIt]]) {
        i++;
    }

    sol.scheduling[proc_2].insert(sol.scheduling[proc_2].begin() + i, sol.scheduling[proc_1][taskIt]);
    sol.scheduling[proc_1].erase(sol.scheduling[proc_1].begin() + taskIt);
}
