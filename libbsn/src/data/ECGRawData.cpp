#include "bsn/data/ECGRawData.h"

namespace bsn {
    namespace data {
        
        ECGRawData::ECGRawData(const double &sensor_data) : RawData(sensor_data) {}
        
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