#include "bsn/data/OximeterRawData.h"

namespace bsn {
    namespace data {

        OximeterRawData::OximeterRawData(const double &sensor_data) : RawData(sensor_data) {}
        
        OximeterRawData::OximeterRawData() {}

        OximeterRawData::~OximeterRawData() {}

        int32_t OximeterRawData::ID() {
            return 883;
        }

        int32_t OximeterRawData::getID() const {
            return OximeterRawData::ID();
        }

    }
}