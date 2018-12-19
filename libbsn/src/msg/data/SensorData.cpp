/*
 * Mensagem com dados do sensor remetente
 * 
 * @author Ricardo Diniz Caldas
 * @version v1.0
 */

#include "bsn/msg/data/SensorData.h"
#include <iostream>

namespace bsn {
    namespace msg { 
        namespace data {
        
            using namespace std;

            SensorData::SensorData() : sensorType(), sensorData(), times() {}
            
            SensorData::SensorData(const array<string, 2> &type, const array<double, 4> &data, const array<string, 8> &t) : 
                sensorType(type),
                sensorData(data),
                times(t) {}
            
            SensorData::~SensorData() {}
            
            SensorData::SensorData(const SensorData &obj) :
                SerializableData(),
                sensorType(obj.getSensorType()),
                sensorData(obj.getSensorData()),
                times(obj.getTimes()) {}
            
            SensorData& SensorData::operator=(const SensorData &obj) {
                sensorType = obj.getSensorType();
                sensorData = obj.getSensorData();
                times = obj.getTimes();           
                return (*this);
            }
            
            int32_t SensorData::ID() {
                return 873;
            }
            const string SensorData::ShortName() {
                return "SensorData";
            }
            const string SensorData::LongName() {
                return "data.SensorData";
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

            void SensorData::setSensorType(const array<string, 2> type ) {
                sensorType = type;
            }

            array<string, 2> SensorData::getSensorType() const {
                return sensorType;
            }

            void SensorData::setSensorData(const array<double, 4> d /*sensor_status*/) {
                sensorData = d;
            }

            array<double, 4> SensorData::getSensorData() const {
                return sensorData;
            }

            void SensorData::setTimes(const array<string, 8> t) {
                times = t;
            }

            array<string, 8> SensorData::getTimes() const {
                return times;
            }

            ostream& SensorData::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, sensorType[0]);
                s->write(2, sensorType[1]);

                s->write(3, sensorData[0]);
                s->write(4, sensorData[1]);
                s->write(5, sensorData[2]);
                s->write(6, sensorData[3]);

                s->write(7, times[0]);
                s->write(8, times[1]);
                s->write(9, times[2]);
                s->write(10, times[3]);
                s->write(11, times[4]);
                s->write(12, times[5]);
                s->write(13, times[6]);
                s->write(14, times[7]);

                return out;
            }
            
            istream& SensorData::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, sensorType[0]);
                d->read(2, sensorType[1]);

                d->read(3, sensorData[0]);
                d->read(4, sensorData[1]);
                d->read(5, sensorData[2]);
                d->read(6, sensorData[3]);

                d->read(7, times[0]);
                d->read(8, times[1]);
                d->read(9, times[2]);
                d->read(10, times[3]);
                d->read(11, times[4]);
                d->read(12, times[5]);
                d->read(13, times[6]);
                d->read(14, times[7]);

                return in;
            }
            
            const string SensorData::toString() const {
                stringstream sstr;

                sstr << endl;

                return sstr.str();
            }
        }
    }
}
