#include "openbasn/model/sensor/Sensor.h"

namespace openbasn {
    namespace model {
        namespace sensor {

            using namespace std;
            
            Sensor::Sensor(const int32_t &type, const float &samplerate, const bool &active, const double &mean, const double &stddev) :
                m_sensorType(type),
                m_samplerate(samplerate),
                m_active(active),
                m_gen((unsigned int)time(NULL)),
                m_distr(mean, stddev),
                m_now(odcore::data::TimeStamp()),
                m_data(m_active?m_distr(m_gen):0)
            {}
            
            Sensor::~Sensor() {}
            
            void Sensor::setSensorType(const int32_t &sensorType) {
                m_sensorType = sensorType;
            }
            
            int32_t Sensor::getSensorType() {
                return m_sensorType;
            }
            
            void Sensor::setSampleRate(const float &samplerate) {
                m_samplerate = samplerate;
            }
            
            float Sensor::getSampleRate() {
                return m_samplerate;
            }
            
            void Sensor::setActive(const bool &active){
                m_active = active;
            }
            
            bool Sensor::isActive() {
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
