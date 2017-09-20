#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdint.h>

class Sensor {

    private:
        Sensor(const Sensor &/*obj*/);
        Sensor& operator=(const Sensor &/*obj*/);
    
    public:
        Sensor() {}
        virtual ~Sensor() {}

        virtual void setID(const uint32_t &) = 0;
        virtual uint32_t getID() = 0;

        virtual void setSampleRate(const float &) = 0;
        virtual float getSampleRate() = 0;

        virtual void setActive(const bool &) = 0;
        virtual bool isActive() = 0;

    private:
        uint32_t m_id;
        float m_samplerate;
        bool m_active;
};

#endif