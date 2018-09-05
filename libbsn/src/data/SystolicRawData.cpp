#include "bsn/data/SystolicRawData.h"

namespace bsn {
    namespace data {

        SystolicRawData::SystolicRawData(const double &sensor_data) : RawData(sensor_data) {}
        
        SystolicRawData::SystolicRawData() {}

        SystolicRawData::~SystolicRawData() {}

        int32_t SystolicRawData::ID() {
            return 884;
        }

        int32_t SystolicRawData::getID() const {
            return SystolicRawData::ID();
        }

    }
}