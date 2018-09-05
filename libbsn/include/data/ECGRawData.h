#ifndef ECGRAWDATA_H
#define ECGRAWDATA_H

#include "bsn/data/RawData.h"

namespace bsn {
    namespace data {

        class ECGRawData : public RawData {
            public:
                ECGRawData(const double &sensor_data);
                ECGRawData();
                ~ECGRawData();

                int32_t getID() const;
                static int32_t ID();
        };
      
    }
}

#endif