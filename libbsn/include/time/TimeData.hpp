#ifndef TIMEDATA_HPP
#define TIMEDATA_HPP

#include <sys/time.h>

namespace bsn {
    namespace time {

        class TimeData {
            public:
                TimeData();
                ~TimeData();
            
                timespec elapsedTime(timespec &, timespec &);

        };

    }
}

#endif