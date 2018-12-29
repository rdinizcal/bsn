#ifndef SPEC_DATA_H
#define SPEC_DATA_H

#include "opendavinci/odcore/data/SerializableData.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

#include <stdint.h>
#include <sys/time.h>
#include <vector>

namespace bsn {
    namespace msg { 
        namespace data {
        
            class SpecData : public odcore::data::SerializableData {
            
                public:
                    SpecData(const double &/*data*/, const std::string &/*type*/);

                    SpecData();            
                    virtual ~SpecData();   

                    SpecData(const SpecData & /*obj*/);
                    SpecData& operator=(const SpecData &/*obj*/);
                
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
                    
                public:
                    void setData(const double &/*data*/);
                    double getData() const;

                    void setType(const std::string &/*type*/);
                    std::string getType() const;
                
                private:
                    double data;
                    std::string type;
            };
        }
    }
}

#endif