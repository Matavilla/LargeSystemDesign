#include "SaSolver.h"
#include "SolutionScheduling.h"
#include "MutationScheduling.h"

#include "Temperature1.h"
#include "Temperature2.h"
#include "Temperature3.h"

#include "tinyxml2.h"

#include <iostream>
#include <chrono>
#include <cstdlib>
#include <thread>
#include <vector>
#include <unistd.h>

void printLog(SolutionScheduling* ans, long long elapsed_ms, bool flag) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile("test.xml");
    long long answer = std::atol(doc.FirstChildElement("Answer")->GetText());

    std::cout << "Time: " << elapsed_ms<< " ms\n";
    std::cout << "Answer: " << ans->getEnergy() << "\n";
    std::cout << "Total min: " << answer << "\n";
    std::cout << "Precision: " << (((double) answer / ans->getEnergy()) * 100) << " %\n";
    
    if (flag) {
        Singleton* tasks = Singleton::getInstance("");
        for (auto& i : ans->getAns()) {
            for (auto& j : i) {
                std::cout << (*tasks)[j] << " ";
            }
            std::cout << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    size_t numThreads = sysconf(_SC_NPROCESSORS_ONLN);
    if (argc == 2) {
        numThreads = std::atoi(argv[1]);
    }

    SolutionScheduling* ans = nullptr;
    
    const bool PRINT_ANSWER = false;
    if (numThreads <= 1) {
        const double INIT_TEMPERATURE = 10000;
        SaSolver<SolutionScheduling, MutationScheduling, Temperature2> solver("test.xml", INIT_TEMPERATURE);
        
        auto begin = std::chrono::steady_clock::now();
        solver.start();
        auto end = std::chrono::steady_clock::now();
        
        printLog(dynamic_cast<SolutionScheduling*>(solver.getSolution()), std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(), PRINT_ANSWER);
    } else {
        std::vector<SaSolver<SolutionScheduling, MutationScheduling, Temperature2>> solvers;
        for (size_t i = 0; i < numThreads; i++) {
            const double INIT_TEMPERATURE = 10000;
            solvers.emplace_back("test.xml", INIT_TEMPERATURE);
        }

        auto begin = std::chrono::steady_clock::now();

        const size_t MAX_ITERATION_WITHOUT_UPDATE = 10;
        size_t iterationWithoutUpdate = 0;
        while (iterationWithoutUpdate <= MAX_ITERATION_WITHOUT_UPDATE) {
            std::vector<std::thread> threadPool;

            for (size_t i = 0; i < numThreads; i++) {
                threadPool.emplace_back([&solvers](int i) {solvers[i].start();}, i);
            }

            for (auto& i : threadPool) {
                i.join();
            }

            if (!ans) {
                ans = dynamic_cast<SolutionScheduling*>(solvers[0].getSolution());
            }

            for (auto& i : solvers) {
                if (ans->getEnergy() > i.getSolution()->getEnergy()) {
                    iterationWithoutUpdate = 0;
                    ans = dynamic_cast<SolutionScheduling*>(i.getSolution());
                }
            }

            for (auto& i : solvers) {
                i.updateSolution(ans);
            }
            iterationWithoutUpdate++;
        }

        auto end = std::chrono::steady_clock::now();
        
        printLog(ans, std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(), PRINT_ANSWER);
    }

    return 0;
}
