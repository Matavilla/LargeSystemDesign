#include "TemperatureBase.h"

class Temperature2 : public TemperatureBase {
public:
    void change(const size_t& i) override {
        temp /= (1 + i);
    }

    virtual ~Temperature2() = default;
}
