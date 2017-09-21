#ifndef THERMOMETER_H_
#define THERMOMETER_H_

#include "Sensor.h"

#include "opendavinci/odcore/data/TimeStamp.h"
#include <random>
#include <stdint.h>

using namespace std;
using namespace odcore::data;

class Thermometer: public Sensor {

    private:
        Thermometer(const Thermometer &/*obj*/);
        Thermometer& operator=(const Thermometer &/*obj*/);

    public:
        Thermometer(const uint32_t &id,
                    const float &samplerate,
                    const bool &active, 
                    const double &mean, 
                    const double &stddev);
        ~Thermometer();
        
        void setID(const uint32_t &);
        uint32_t getID();

        void setSampleRate(const float &);
        float getSampleRate();

        void setActive(const bool &);
        bool isActive();

        void setName(const string &);
        string getName();

        double getData();

    private:
        uint32_t m_id;
        float m_samplerate;
        bool m_active;

        mt19937 m_gen;
        normal_distribution<> m_distr;
        TimeStamp m_now;

        double m_data;
        string m_name;
};

#endif