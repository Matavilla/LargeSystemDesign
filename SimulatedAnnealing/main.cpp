#include "SaSolver.h"
#include "SolutionScheduling.h"
#include "MutationScheduling.h"
#include "Temperature1.h"
#include "Temperature2.h"
#include "Temperature3.h"

#include <iostream>


int main() {
    SaSolver<SolutionScheduling, MutationScheduling, Temperature3> t1("test.xml", 1000);
    t1.start();
    SolutionScheduling& ans = dynamic_cast<SolutionScheduling&>(t1.getSolution());
    std::cout << ans.getEnergy() << std::endl;
    for(auto& i : ans.getAns()) {
        for(auto& j : i) {
            std::cout << j << ' ';
        }
        std::cout <<std::endl;
    }
    return 0;
}
