#include "TemperatureBase.h"

class Temperature3 : public TemperatureBase {
public:
    double get_temp(const size_t& i) override {
        return temp * (std::log(1 + i) / (1 + i));
    }

    virtual ~Temperature3() = default;
};
