#include "bsn/data/DiastolicRawData.h"

namespace bsn {
    namespace data {

        DiastolicRawData::DiastolicRawData(const double &sensor_data, const timespec &ts) : RawData(sensor_data, ts){}
        
        DiastolicRawData::DiastolicRawData(){}

        DiastolicRawData::~DiastolicRawData() {}

        int32_t DiastolicRawData::ID() {
            return 885;
        }

        int32_t DiastolicRawData::getID() const {
            return DiastolicRawData::ID();
        }

    }
}