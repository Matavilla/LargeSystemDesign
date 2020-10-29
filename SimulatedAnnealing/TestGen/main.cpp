#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cout << "./TesGen numProc numTasks minTaskTime maxTimeTask" << std::endl;
        return 0;
    }
    size_t numProc = std::stoi(argv[1]);
    size_t numTasks = std::stoi(argv[2]);
    size_t t1 = std::stoi(argv[3]);
    size_t t2 = std::stoi(argv[4]);

    std::ofstream f("test.xml", std::ios_base::out | std::ios_base::trunc);
    f << "<Proc>\n<Num>\n" << numProc << "</Num>\n<\\Proc>\n";
    f << "<Task>\n<Num>\n" << numTasks << "</Num>\n";
    f << "<Time>\n"; 
    std::mt19937 engine;
    engine.seed(std::random_device()());
    std::uniform_int_distribution<size_t> dist(t1, t2);
    for (size_t i = 0; i < numTasks; i++) {
        f << dist(engine) << ' ';
    }
    f << "</Time>\n</Task>\n";

    return 0;
}
