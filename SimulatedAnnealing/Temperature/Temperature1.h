#include "TemperatureBase.h"

class Temperature1 : public TemperatureBase {
public:
    double get_temp(const size_t& i) override {
        return temp / std::log(1 + i);
    }

    virtual ~Temperature1() = default;
};
