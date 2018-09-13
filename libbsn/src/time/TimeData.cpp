#include "bsn/time/TimeData.hpp"

namespace bsn {
    namespace time {

        TimeData::TimeData() {}

        TimeData::~TimeData() {}

        timespec TimeData::elapsedTime(timespec &now, timespec &ref) {

            timespec diff;

            if ((now.tv_nsec - ref.tv_nsec) < 0) {
                diff.tv_sec = now.tv_sec - ref.tv_sec - 1;
                diff.tv_nsec = now.tv_nsec - ref.tv_nsec + 1000000000L;
            }
            else {
                diff.tv_sec = now.tv_sec - ref.tv_sec;
                diff.tv_nsec = now.tv_nsec - ref.tv_nsec;
            }

            return diff;
        }

    }
}