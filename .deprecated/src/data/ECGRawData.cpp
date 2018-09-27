#include "../../include/data/ECGRawData.h"

namespace bsn {
    namespace data {
        
        ECGRawData::ECGRawData(const double &sensor_data, const timespec &ts) : RawData(sensor_data, ts) {}
        
        ECGRawData::ECGRawData() {}

        ECGRawData::~ECGRawData() {}

        int32_t ECGRawData::ID() {
            return 882;
        }

        int32_t ECGRawData::getID() const {
            return ECGRawData::ID();
        }

    }
}