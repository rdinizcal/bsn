#ifndef MODEL_SENSOR_THERMOMETER_H_
#define MODEL_SENSOR_THERMOMETER_H_

#include "opendavinci/odcore/data/TimeStamp.h"
#include <random>
#include <stdint.h>

namespace openbasn {
    namespace model {
        namespace sensor {

            using namespace std;
            using namespace odcore::data;

            class Sensor {
            
                public:
                    enum SENSORTYPE {
                        UNDEFINED               = 0,
                        THERMOMETER             = 1,
                        ECG                     = 2,
                        OXIMETER                = 3
                    };

                private:
                    Sensor(const Sensor &/*obj*/);
                    Sensor& operator=(const Sensor &/*obj*/);
            
                public:
                    Sensor(const int32_t &sensorType,
                                const float &samplerate,
                                const bool &active, 
                                const double &mean, 
                                const double &stddev);
                    ~Sensor();
                    
                    void setSensorType(const int32_t &);
                    int32_t getSensorType();
            
                    void setSampleRate(const float &);
                    float getSampleRate();
            
                    void setActive(const bool &);
                    bool isActive();
            
                    double getData();
            
                private:
                    int32_t m_sensorType;
                    float m_samplerate;
                    bool m_active;
            
                    mt19937 m_gen;
                    normal_distribution<> m_distr;
                    TimeStamp m_now;
            
                    double m_data;
            };
        }
    }
}

#endif