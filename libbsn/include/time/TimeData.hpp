#ifndef TIMEDATA_HPP
#define TIMEDATA_HPP

#include <sys/time.h>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

namespace bsn {
    namespace time {

        class TimeData {
            public:
                TimeData();
                ~TimeData();
            
                timespec elapsedTime(timespec &, timespec &);
                std::string get_time();

        };

    }
}

#endif