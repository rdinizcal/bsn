#ifndef DATACOLLECTORMODULE_CONTROL_COMMAND_H
#define DATACOLLECTORMODULE_CONTROL_COMMAND_H

#include "opendavinci/odcore/data/SerializableData.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

#include <stdint.h>
#include <map>
#include <sstream>

namespace bsn {
    namespace msg {
        namespace control {
            
            class ControlSignal : public odcore::data::SerializableData {
            
                public:
                    ControlSignal(const int32_t &/*id*/, const bool &/*active*/, const std::map<std::string,double> &/*params*/);
    
                    ControlSignal();        
                    virtual ~ControlSignal();
                    
                    ControlSignal(const ControlSignal &/*obj*/);
                    ControlSignal& operator=(const ControlSignal &/*obj*/);
                
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
                    void setRecipientID(const int32_t &/*id*/);
                    int32_t getRecipientID() const;

                    void setActive(const bool &/*active*/);
                    bool getActive() const;

                    void setParams(const std::map<std::string,double>  &/*params*/);
                    std::map<std::string,double> getParams() const;
                
                private:
                    int32_t recipient;

                    bool active;
                    std::map<std::string,double> params;
            };
        }
    }
}

#endif