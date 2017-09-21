#include "SensorData.hpp"

#include <ostream>
#include <string>

#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

using namespace std;

SensorData::SensorData() : 
m_snode_id(0),
m_sname(""),
m_data(0)
{}

SensorData::SensorData(uint32_t snode_id, string sname, double data) : 
    m_snode_id(snode_id),
    m_sname(sname),
    m_data(data)
{}

SensorData::~SensorData() {}

SensorData::SensorData(const SensorData &obj) :
    SerializableData(),
    m_snode_id(obj.getSensorNodeID()),
    m_sname(obj.getSensorName()),
    m_data(obj.getData()) {}

SensorData& SensorData::operator=(const SensorData &obj) {
    m_snode_id = obj.getSensorNodeID();
    m_sname = obj.getSensorName();
    m_data= obj.getData();
    return (*this);
}

int32_t SensorData::ID() {
    return 7;
}
const string SensorData::ShortName() {
    return "SensorData";
}
const string SensorData::LongName() {
    return "SensorData";
}

int32_t SensorData::getID() const {
    return SensorData::ID();
}
const string SensorData::getShortName() const {
    return SensorData::ShortName();
}
const string SensorData::getLongName() const {
    return SensorData::LongName();
}

uint32_t SensorData::getSensorNodeID() const {
    return m_snode_id;
}

string SensorData::getSensorName() const {
    return m_sname;
}

double SensorData::getData() const {
    return m_data;
}

ostream& SensorData::operator<<(ostream &out) const {
    odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
    std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);

    s->write(1, m_snode_id);
    s->write(2, m_sname);
    s->write(3, m_data);

    return out;
}

istream& SensorData::operator>>(istream &in) {
    odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
    std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);

    d->read(1, m_snode_id);
    d->read(2, m_sname);
    d->read(3, m_data);

    return in;
}

const string SensorData::toString() const {
    stringstream sstr;

    sstr << SensorData::ShortName() << "#" << SensorData::ID() << " | ";
    sstr << m_snode_id << " " << m_sname << " : " << m_data;
    
    return sstr.str();
}