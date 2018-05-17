#include "bsn/entity/BodyHub.h"

namespace bsn {
    namespace entity {

        BodyHub::BodyHub() : mHealthStatus(), mSensorReg(), persist()  { }

        BodyHub::~BodyHub() {}

        void BodyHub::updateHealthStatus(data::SensorData sensordata){
            mSensorReg[sensordata.getSensorType()-1] = sensordata.getSensorStatus();
            mHealthStatus = statistics::calculateHealthStatus(mSensorReg);
        }

        void BodyHub::printHealthStatus(){
            std::cout << "----------------------------------------"<<std::endl;
            for(uint32_t i = 0; i < 3; i++){
                std::cout << ((i==0)?"Thermometer: ":(i==1)?"ECG: ":"Oximeter: ");
                std::cout << mSensorReg[i] << std::endl;
            }
            std::cout << "Health Status: " << mHealthStatus << std::endl;
            std::cout << "----------------------------------------"<<std::endl;
        }

        void BodyHub::callPersist(Container container){    
            persist.persistDataReceived(container.getData<data::SensorData>().getSensorID(),container.getData<data::SensorData>().getSensorStatus());
        }

        string BodyHub::getHealthStatus() const {
            return mHealthStatus;
        }

        void BodyHub::setHealthStatus(string healthStatus) {
            mHealthStatus = healthStatus;
        }

        map<uint32_t, string> BodyHub::getSensorReg() const {
            return mSensorReg;
        }

        void BodyHub::setSensorReg(map<uint32_t, string> sensorReg) {
            mSensorReg = sensorReg;
        }

    }
}