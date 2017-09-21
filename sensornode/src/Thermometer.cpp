#include "Thermometer.h"

using namespace std;

Thermometer::Thermometer(const uint32_t &id, const float &samplerate, const bool &active, const double &mean, const double &stddev) :
    m_id(id),
    m_samplerate(samplerate),
    m_active(active),
    m_gen((unsigned int)time(NULL)),
    m_distr(mean, stddev),
    m_now(odcore::data::TimeStamp()),
    m_data(m_active?m_distr(m_gen):0),
    m_name("Thermometer#"+to_string(id))
{}

Thermometer::~Thermometer() {}

void Thermometer::setID(const uint32_t &id) {
    m_id = id;
}

uint32_t Thermometer::getID() {
    return m_id;
}

void Thermometer::setSampleRate(const float &samplerate) {
    m_samplerate = samplerate;
}

float Thermometer::getSampleRate() {
    return m_samplerate;
}

void Thermometer::setActive(const bool &active){
    m_active = active;
}

bool Thermometer::isActive() {
    return m_active;
}

string Thermometer::getName() {
    return m_name;
}

double Thermometer::getData() {
    
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
