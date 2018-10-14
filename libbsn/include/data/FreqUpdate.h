#ifndef RESOURCE_UPDATE_H
#define RESOURCE_UPDATE_H

#include "opendavinci/odcore/data/SerializableData.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

namespace bsn {
    namespace data {
        
        class FreqUpdate : public odcore::data::SerializableData {
        
            public:
                FreqUpdate(const int &/*units*/);
 
                FreqUpdate(); // construtor
                virtual ~FreqUpdate();// destrutor 
                
                //Boas praticas do manual do OpenDaVINCI
                FreqUpdate(const FreqUpdate &/*obj*/);
                FreqUpdate& operator=(const FreqUpdate &/*obj*/);
            
            // Métodos abstratos
            public:
                virtual int32_t getID() const;
                virtual const std::string getShortName() const;
                virtual const std::string getLongName() const;

                static int32_t ID();
                static const std::string ShortName();
                static const std::string LongName();
        
            public:
                virtual std::ostream& operator<<(std::ostream &out) const;
                virtual std::istream& operator>>(std::istream &in);
        
                virtual const std::string toString() const;
                
            // setters e getters
            public:
                void setFreq(const int &/*freq*/);
                int getFreq() const;
            
            private:
                int freq;
        };
    }
}

#endif