#include "SaSolver.h"
#include "SolutionScheduling.h"
#include "MutationScheduling.h"
#include "Temperature1.h"

int main() {
    SaSolver<SolutionScheduling, MutationScheduling, Temperature1>("test.xml", 0.000001, 100);
    return 0;
}
