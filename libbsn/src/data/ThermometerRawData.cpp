#include "bsn/data/ThermometerRawData.h"

namespace bsn {
    namespace data {

        ThermometerRawData::ThermometerRawData(const double &sensor_data) : RawData(sensor_data) {}
        
        ThermometerRawData::ThermometerRawData() {}

        ThermometerRawData::~ThermometerRawData() {}


        int32_t ThermometerRawData::ID() {
            return 881;
        }

        int32_t ThermometerRawData::getID() const {
            return ThermometerRawData::ID();
        }

    }
}