#ifndef OXIMETER_COLLECT_TASK_MSG
#define OXIMETER_COLLECT_TASK_MSG

#include "opendavinci/odcore/data/SerializableData.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

#include <stdint.h>

namespace bsn {
    namespace msg { 
        namespace data {
        
            class  OximeterCollectTaskMessage : public odcore::data::SerializableData {
            
                public:
                    OximeterCollectTaskMessage(const double &/*data*/);
    
                    OximeterCollectTaskMessage();
                    virtual ~  OximeterCollectTaskMessage();
                    
                    OximeterCollectTaskMessage(const OximeterCollectTaskMessage &/*obj*/);
                    OximeterCollectTaskMessage& operator=(const  OximeterCollectTaskMessage &/*obj*/);
                
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

                private:
                    double data;
            };
        }
    }
}

#endif