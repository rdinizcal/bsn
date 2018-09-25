#ifndef OXIMETERRAWDATA_H
#define OXIMETERRAWDATA_H

#include "bsn/data/RawData.h"

namespace bsn {
    namespace data {

        class OximeterRawData : public RawData {
            public:
              OximeterRawData(const double &sensor_data, const timespec &ts);
              OximeterRawData();
              virtual ~OximeterRawData();

              int32_t getID() const;
              static int32_t ID();
        };
      
    }
}

#endif