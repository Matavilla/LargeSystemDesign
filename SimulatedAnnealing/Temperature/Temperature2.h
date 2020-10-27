#include "TemperatureBase.h"

class Temperature2 : public TemperatureBase {
public:
    double get_temp(const size_t& i) override {
        return temp / (1 + i);
    }

    virtual ~Temperature2() = default;
}
