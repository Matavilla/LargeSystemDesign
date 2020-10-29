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


int main() {
    SaSolver<SolutionScheduling, MutationScheduling, Temperature3> t1("test.xml", 1000);
    auto begin = std::chrono::steady_clock::now();
    t1.start();
    auto end = std::chrono::steady_clock::now();

    SolutionScheduling& ans = dynamic_cast<SolutionScheduling&>(t1.getSolution());

    tinyxml2::XMLDocument doc;
    doc.LoadFile("test.xml");
    size_t answer = std::atoi(doc.FirstChildElement("Answer")->GetText());

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "Time: " << elapsed_ms.count() << " ms\n";
    std::cout << "Answer: " << ans.getEnergy() << "\n";
    std::cout << "Total min: " << answer << "\n";
    std::cout << "Precision: " << (size_t) ((answer / ans.getEnergy()) * 100) << std::endl;
    return 0;
}
