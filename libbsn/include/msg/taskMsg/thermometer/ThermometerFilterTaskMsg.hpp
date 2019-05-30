#ifndef THERMOMETER_FILTER_TASK_MSG
#define THERMOMETER_FILTER_TASK_MSG

#include "opendavinci/odcore/data/SerializableData.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

#include <stdint.h>

namespace bsn {
    namespace msg { 
        namespace data {
        
            class  ThermometerFilterTaskMessage : public odcore::data::SerializableData {
            
                public:
                    ThermometerFilterTaskMessage(const double &/*data*/);
    
                    ThermometerFilterTaskMessage();
                    virtual ~  ThermometerFilterTaskMessage();
                    
                    ThermometerFilterTaskMessage(const ThermometerFilterTaskMessage &/*obj*/);
                    ThermometerFilterTaskMessage& operator=(const  ThermometerFilterTaskMessage &/*obj*/);
                
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