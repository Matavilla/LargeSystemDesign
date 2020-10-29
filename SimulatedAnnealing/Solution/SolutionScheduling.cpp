#include "SolutionScheduling.h"

#include "tinyxml2.h"

#include <sstream>
#include <cstdlib>
#include <random>
#include <chrono>
#include <algorithm>


Singleton* Singleton::singleton_= nullptr;

Singleton::Singleton(const std::string& path) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());

    size_t numTask = std::atoi(doc.FirstChildElement("Task")->FirstChildElement("Num")->GetText());
    std::string time(doc.FirstChildElement("Task")->FirstChildElement("Time")->GetText());

    value.resize(numTask);

    std::stringstream ss(time);
    for (auto& i : value) {
         ss >> i;
    }

    std::sort(value.begin(), value.end());
}

void SolutionScheduling::updateSolution(const SolutionBase& s) {
    const SolutionScheduling& sol = dynamic_cast<const SolutionScheduling&>(s);
    for (size_t i = 0; i < scheduling.size(); i++) {
        scheduling[i].clear();
        for (auto& j : sol.scheduling[i]) {
            scheduling[i].push_back(j);
        }
    }
    setEnergy(s.getEnergy());
}

void SolutionScheduling::init() {
    std::mt19937 engine;
    engine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<size_t> dist(0, scheduling.size() - 1);

    for (size_t i = 0; i < tasksTime->size(); i++) {
        scheduling[dist(engine)].push_back(i);
    }

    consumeEnergy();
}

void SolutionScheduling::consumeEnergy() {
    double e = 0;
    for (size_t i = 0; i < scheduling.size(); i++) {
        size_t start = 0;
        for (auto& j : scheduling[i]) {
            start += (*tasksTime)[j];
            e += start;
        }
    }
    setEnergy(e);
}

SolutionScheduling::SolutionScheduling(const std::string& path, bool initFlag) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());

    size_t numProc = std::atoi(doc.FirstChildElement("Proc")->FirstChildElement("Num")->GetText());

    scheduling.resize(numProc);
 
    tasksTime = Singleton::getInstance(path);

    if (initFlag) {
        init();
    }
}
