#pragma once

#include <cmath>

class TemperatureBase {
public:
    double temp = 0.0;

    TemperatureBase() = default;

    virtual double get_temp(const size_t& i) = 0;

    void set_temp(const double& t) {
        temp = t;
    }

    virtual ~TemperatureBase() = default;
};
