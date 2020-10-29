#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>

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
    f << "<Proc>\n<Num>" << numProc << "</Num>\n<\\Proc>\n";
    f << "<Task>\n<Num>" << numTasks << "</Num>\n";
    f << "<Time>"; 
    std::mt19937 engine;
    engine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<size_t> dist(t1, t2);
    std::vector<size_t> ans(numTasks);
    for (size_t i = 0; i < numTasks; i++) {
        ans[i] = dist(engine);
        f << ans[i] << ' ';

    }
    f << "</Time>\n</Task>\n";

    std::sort(ans.begin(), ans.end());
    size_t e = 0;
    for (size_t i = 0; i < numTasks; i += numProc) {
        for (size_t j = 0 ; j < numProc && i + j < numTasks; j++ ) {
            e += ans[i + j];
            if (i + j + numProc < numTasks) {
                ans[i + j + numProc] += ans[i + j];
            }
        }
    }
    f << "<Answer>" << e << "</Answer>\n";
    return 0;
}
