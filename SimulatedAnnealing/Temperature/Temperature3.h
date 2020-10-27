#include "TemperatureBase.h"

class Temperature3 : public TemperatureBase {
public:
    void change(const size_t& i) override {
        temp *= std::log(1 + i) / (1 + i);
    }

    virtual ~Temperature3() = default;
}
