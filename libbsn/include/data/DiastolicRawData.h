#ifndef DRAWDATA_H
#define DRAWDATA_H

#include "bsn/data/RawData.h"

namespace bsn {
    namespace data {

        class DiastolicRawData : public RawData {
            public:
                DiastolicRawData(const double &sensor_data);
                DiastolicRawData();
                virtual ~DiastolicRawData();

                int32_t getID() const;
                static int32_t ID();
        };
      
    }
}

#endif