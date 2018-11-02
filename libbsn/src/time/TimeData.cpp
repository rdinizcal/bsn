#include "bsn/time/TimeData.hpp"

using namespace std;

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

        string TimeData::get_time(){
            timeval curTime;
            gettimeofday(&curTime, NULL);
            int milli = curTime.tv_usec / 1000;

            char buffer [80];
            strftime(buffer, 80, "%M:%S", localtime(&curTime.tv_sec));    

            char currentTime[84] = "";
            sprintf(currentTime, "%s:%d", buffer, milli);
            string out = currentTime;
            return out;
        }

    }
}