#ifndef RESOURCE_DATA_H
#define RESOURCE_DATA_H

#include "opendavinci/odcore/data/SerializableData.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

#include "bsn/resource/Resource.hpp"

namespace bsn {
    namespace data {
        
        class ResourceData : public odcore::data::SerializableData, bsn::resource::Resource {
        
            public:
                //construtor parametrizado
                ResourceData(const bsn::resource::Resource &/*resource*/);
 
                ResourceData(); // construtor
                virtual ~ResourceData();// destrutor 
                
                //Boas praticas do manual do OpenDaVINCI
                ResourceData(const ResourceData &/*obj*/);
                ResourceData& operator=(const ResourceData &/*obj*/);
            
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
                void setResource(const bsn::resource::Resource &/*resource*/);
                bsn::resource::Resource getResource() const;
            
            private:
                bsn::resource::Resource mResource;
        };
    }
}

#endif