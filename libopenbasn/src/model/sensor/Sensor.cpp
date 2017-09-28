#include "openbasn/model/sensor/Sensor.h"

namespace openbasn {
    namespace model {
        namespace sensor {

            using namespace std;

            Sensor::Sensor(const uint32_t &type, const float &samplerate, const bool &active, const double &mean, const double &stddev) :
                m_sensorType(type),
                m_samplerate(samplerate),
                m_active(active),
                m_mean(mean),
                m_stddev(stddev),
                m_gen((unsigned int)time(NULL)),
                m_distr(mean, stddev),
                m_now(odcore::data::TimeStamp()),
                m_data(m_active?m_distr(m_gen):0)
            {}
            
            Sensor::Sensor() :
                m_sensorType(),
                m_samplerate(),
                m_active(),
                m_mean(),
                m_stddev(),
                m_gen((unsigned int)time(NULL)),
                m_distr(),
                m_now(odcore::data::TimeStamp()),
                m_data() {}

            Sensor::~Sensor() {}
            
            Sensor::Sensor(const Sensor &obj) :
                m_sensorType(obj.getSensorType()),
                m_samplerate(obj.getSampleRate()),
                m_active(obj.isActive()),
                m_mean(obj.getMean()),
                m_stddev(obj.getStddev()),
                m_gen((unsigned int)time(NULL)),
                m_distr(obj.getMean(), obj.getStddev()),
                m_now(odcore::data::TimeStamp()),
                m_data(0) {}
        
            Sensor& Sensor::operator=(const Sensor &obj) {
                m_sensorType = obj.getSensorType();
                m_samplerate = obj.getSampleRate();
                m_active = obj.isActive();
                m_mean = obj.getMean();
                m_stddev = obj.getStddev();
                m_distr = normal_distribution<>(obj.getMean(), obj.getStddev());
                m_now = odcore::data::TimeStamp();
                m_data = 0;
                return (*this);
            }

            void Sensor::setSensorType(const uint32_t &sensorType) {
                m_sensorType = sensorType;
            }
            
            uint32_t Sensor::getSensorType() const {
                return m_sensorType;
            }
            
            void Sensor::setSampleRate(const float &samplerate) {
                m_samplerate = samplerate;
            }
            
            float Sensor::getSampleRate() const {
                return m_samplerate;
            }
            
            void Sensor::setMean(const double &mean) {
                m_mean = mean;
            }

            double Sensor::getMean() const {
                return m_mean;
            }

            void Sensor::setStddev(const double &stddev) {
                m_stddev = stddev;
            }

            double Sensor::getStddev() const {
                return m_stddev;
            }

            void Sensor::setActive(const bool &active){
                m_active = active;
            }
            
            bool Sensor::isActive() const {
                return m_active;
            }
            
            double Sensor::getData() {
                
                if(!m_active){
                    return 0;
                }
                
                if( (-1) * ( (m_now - odcore::data::TimeStamp())).toMicroseconds() > (m_samplerate * 1000000L) ) {
                    m_data = m_distr(m_gen);
                    m_now = odcore::data::TimeStamp();
                } else {
                    m_distr(m_gen);
                } 
                
                return m_data;
            }
        }
    }
}
