#ifndef SENSORNODE_H
#define SENSORNODE_H

#include "opendavinci/odcore/data/Container.h"

#include "../persistence/PersistenceModule.h"

using namespace std;
using namespace odcore::data;

namespace bsn {
    namespace entity {

        class SensorNode {

            public:
                SensorNode();
                ~SensorNode();

                void setSensorType(int32_t /*sensor type*/);
                int32_t getSensorType() const;

                void setStatus(string /*status*/);
                string getStatus() const;

            private:
                int32_t mSensorType;      // tipo do sensor associado
                string mStatus;           // estado atual
                persistence::SensorPersistence persist;

        };

    }
}
#endif