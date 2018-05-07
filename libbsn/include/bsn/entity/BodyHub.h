#ifndef BODYHUB_H
#define BODYHUB_H

#include "opendavinci/odcore/data/Container.h"

#include "../data/SensorData.h"
#include "../statistics/VitalStatistics.h"
#include "../persistence/PersistenceModule.h"

#include <map>
#include <iostream>

using namespace std;
using namespace odcore::data;

namespace bsn {
    namespace entity {

        class BodyHub {

            public:
                BodyHub();
                ~BodyHub();

                /**
                 * Atribui estado do nó sensor recebido no registro e
                 * Atualiza o estado do paciente
                 * 
                 * @param mensagem com os dados do sensor
                 * @return 
                */
                void updateHealthStatus(data::SensorData /*sensordata*/);

                /**
                 * Imprime na tela o estado atual do registro de sensores 
                 * 
                 * @param 
                 * @return 
                */
                void printHealthStatus();

                /**
                 * Persiste um snapshot dos dados do registro e 
                 * informações do dado recebido dado
                 * 
                 * @param identificador do nó sensor remetente, instante 
                 * de envio 
                 * @return 
                */
                void persistHealthStatus(uint32_t /*sensor_id*/, timespec /*sent_timespec*/, timespec /*received_timestamp*/);
                
                /*
                 * Printa num arquivo texto os pacotes que chegaram
                 * para comparar se tudo que se enviou chegou.
                */
                void persistDataReceived(Container /*container*/);

                void callPersist(Container container);

                string getHealthStatus() const;
                void setHealthStatus(string /*health status*/);

                map<uint32_t, string> getSensorReg() const;
                void setSensorReg(map<uint32_t, string> /*sensors map*/);

            private:
                string mHealthStatus;
                map<uint32_t, string> mSensorReg; 
                persistence::BodyHubPersistence persist;
        };

    }
}
#endif