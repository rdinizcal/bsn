#ifndef SRAWDATA_H
#define SRAWDATA_H

#include "bsn/data/RawData.h"

namespace bsn {
    namespace data {

        class SystolicRawData : public RawData {
            public:
                SystolicRawData(const double &sensor_data);
                SystolicRawData();
                virtual ~SystolicRawData();

                int32_t getID() const;
                static int32_t ID();
        };
      
    }
}

#endif