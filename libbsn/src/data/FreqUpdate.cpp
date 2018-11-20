#include "bsn/data/FreqUpdate.h"

namespace bsn {
    namespace data {
        
        using namespace std;

        FreqUpdate::FreqUpdate() : freq() {}
        
        FreqUpdate::FreqUpdate(const int32_t &_freq) : 
            freq(_freq) {}
        
        FreqUpdate::~FreqUpdate() {}
        
        FreqUpdate::FreqUpdate(const FreqUpdate &obj) :
            SerializableData(),
            freq(obj.getFreq()){}
        
        FreqUpdate& FreqUpdate::operator=(const FreqUpdate &obj) {
            freq = obj.getFreq();        
            return (*this);
        }
        
        int32_t FreqUpdate::ID() {
            return 877;
        }
        const string FreqUpdate::ShortName() {
            return "FreqUpdate";
        }
        const string FreqUpdate::LongName() {
            return "data.FreqUpdate";
        }
        
        int32_t FreqUpdate::getID() const {
            return FreqUpdate::ID();
        }
        const string FreqUpdate::getShortName() const {
            return FreqUpdate::ShortName();
        }
        const string FreqUpdate::getLongName() const {
            return FreqUpdate::LongName();
        }
        
        void FreqUpdate::setFreq(const int32_t &_freq) {
            freq = _freq;
        }

        int32_t FreqUpdate::getFreq() const {
            return freq;
        }

        ostream& FreqUpdate::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
            
            s->write(1, freq);

            return out;
        }
        
        istream& FreqUpdate::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);

            d->read(1, freq);

            return in;
        }
        
        const string FreqUpdate::toString() const {
            stringstream sstr;

            sstr << "UpdatedFreq#" << freq << endl;

            return sstr.str();
        }
    }
}