#include "bsn/entity/SensorNode.h"

namespace bsn {
    namespace entity { 

        SensorNode::SensorNode() : mSensorType(), mStatus(), persist() {}

        SensorNode::~SensorNode() {}

        void SensorNode::setSensorType(int32_t sensorType) {
            mSensorType = sensorType;
        }

        int32_t SensorNode::getSensorType() const {
            return mSensorType;
        }
        
        void SensorNode::setStatus(string status) {
            mStatus = status;
        }

        string SensorNode::getStatus() const {
            return mStatus;
        }
    }
}