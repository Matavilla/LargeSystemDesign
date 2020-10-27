#pragma once

#include <cmath>

class TemperatureBase {
public:
    double temp = 0.0;

    TemperatureBase() = default;

    virtual void change(const size_t& i) = 0;

    double get_temp() const {
        return temp;
    }

    void set_temp(const double& t) {
        temp = t;
    }

    virtual ~TemperatureBase() = default;
};
