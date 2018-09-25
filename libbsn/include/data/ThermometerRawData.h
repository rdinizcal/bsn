#ifndef TRAWDATA_H
#define TRAWDATA_H

#include "bsn/data/RawData.h"

namespace bsn {
    namespace data {

        class ThermometerRawData : public RawData {
            public:
                ThermometerRawData(const double &sensor_data, const timespec &ts);
                ThermometerRawData();
                ~ThermometerRawData();

                int32_t getID() const;
                static int32_t ID();
        };
      
    }
}

#endif