#include "SolutionScheduling.h"

#include "tinyxml2.h"


#include <sstream>
#include <cstdlib>


void SolutionScheduling::updateSolution(const SolutionBase& s) {
    const SolutionScheduling& sol = dynamic_cast<const SolutionScheduling&>(s);
    for(size_t i = 0; i < scheduling.size(); i++) {
        scheduling[i].clear();
        for(auto& j : sol.scheduling[i]) {
            scheduling[i].push_back(j);
        }
    }
}

void SolutionScheduling::init() {
}

double SolutionScheduling::getEnergy() const {
    double e = 0;
    for(size_t i = 0; i < scheduling.size(); i++) {
        size_t start = 0;
        for(auto& j : scheduling[i]) {
            start += tasksTime[j];
            e += start;
        }
    }
    return e;
}

SolutionScheduling::SolutionScheduling(const std::string& path, bool initFlag) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());

    size_t numProc = std::atoi(doc.FirstChildElement("Proc")->FirstChildElement("Num")->GetText());
    size_t numTask = std::atoi(doc.FirstChildElement("Task")->FirstChildElement("Num")->GetText());
    std::string time(doc.FirstChildElement("Task")->FirstChildElement("Time")->GetText());

    scheduling.resize(numProc);
    tasksTime.resize(numTask);

    std::stringstream ss(time);
    for(auto& i : tasksTime) {
         ss >> i;
    }

    if (initFlag) {
        init();
    }
}
