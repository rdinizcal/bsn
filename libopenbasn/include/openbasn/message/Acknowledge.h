#ifndef MESSAGE_ACKNOWLEDGE_H_
#define MESSAGE_ACKNOWLEDGE_H_

#include "opendavinci/odcore/data/SerializableData.h"

namespace openbasn {
    namespace message {

        using namespace std;
        using namespace odcore::data;

        class Acknowledge : public SerializableData {

        public:
            Acknowledge(const int32_t &/*type*/, const uint32_t &/*source_id*/);
            Acknowledge(const int32_t &/*type*/, const uint32_t &/*source_id*/, const uint32_t &/*destination_id*/);

            Acknowledge();
            virtual ~Acknowledge();
    
            Acknowledge(const Acknowledge &/*obj*/);
            Acknowledge& operator=(const Acknowledge &/*obj*/);

        public:
            enum TYPE {
                NOT_OK     = 0,
                OK         = 1
            };

        public:
            static int32_t ID();
            virtual int32_t getID() const;
            virtual const string getShortName() const;
            virtual const string getLongName() const;
            virtual const string toString() const;
                    
        public:
            virtual ostream& operator<<(ostream &out) const;
            virtual istream& operator>>(istream &in);
        
        public:
            int32_t getType() const;

            void setSourceID(const uint32_t &/*m_source_id*/);
            uint32_t getSourceID() const;

            void setDestinationID(const uint32_t &/*m_destination_id*/);
            uint32_t getDestinationID() const;

        private:
            int32_t m_type;
            uint32_t m_source_id;
            uint32_t m_destination_id;
        };
    }
}

#endif