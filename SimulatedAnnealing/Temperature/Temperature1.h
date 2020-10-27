#include "TemperatureBase.h"

class Temperature1 : public TemperatureBase {
public:
    void change(const size_t& i) override {
        temp /= std::log(1 + i);
    }

    virtual ~Temperature1() = default;
};
