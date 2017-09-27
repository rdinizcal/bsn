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

                public:
                    Sensor(const uint32_t &sensorType,
                        const float &samplerate,
                        const bool &active, 
                        const double &mean, 
                        const double &stddev);
                    
                    Sensor();
                    virtual ~Sensor();

                public:
                    Sensor(const Sensor &/*obj*/);
                    Sensor& operator=(const Sensor &/*obj*/);
                   
                public:        
                    void setSensorType(const uint32_t &);
                    uint32_t getSensorType() const;
                        
                    void setSampleRate(const float &);
                    float getSampleRate() const;
            
                    void setActive(const bool &);
                    bool isActive() const;

                    void setMean(const double &/*mean*/);
                    double getMean() const;

                    void setStddev(const double &/*stddev*/);
                    double getStddev() const;
                    
                    double getData();
            
                private:
                    uint32_t m_sensorType;
                    float m_samplerate;
                    bool m_active;

                    double m_mean;
                    double m_stddev;
                    mt19937 m_gen;
                    normal_distribution<> m_distr;
                    TimeStamp m_now;
            
                    double m_data;
            };
        }
    }
}

#endif