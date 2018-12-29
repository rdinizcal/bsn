#include "CentralhubModule.hpp"

using namespace odcore::data;

using namespace bsn::processor;

using namespace bsn::msg::data;
using namespace bsn::msg::info;


CentralhubModule::CentralhubModule(const int32_t &argc, char **argv) :
TimeTriggeredConferenceClientModule(argc, argv, "centralhub"),
	buffer() {}
	
CentralhubModule::~CentralhubModule() {}

void CentralhubModule::setUp() {
    addDataStoreFor(873, buffer);
}

void CentralhubModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode CentralhubModule::body(){
    
    Container container;    
    double patient_status;
    bool received = false;

    vector<list<double>> data_list(5);
    for(std::vector<std::list<double>>::iterator it = data_list.begin();
        it != data_list.end(); ++it) {
            (*it).push_back(0.0);
    }

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING){

        while(!buffer.isEmpty()){
            
            container = buffer.leave();

            std::string type = container.getData<SensorData>().getType();
            double risk = container.getData<SensorData>().getRisk();
            int32_t sensor_id = get_sensor_id(type);

            if (int32_t(risk) == -1) { continue; }
            data_list[sensor_id].push_back(risk);

            received = true;
        }

        if (!received) { continue; }

        patient_status = data_fuse(data_list);

        // Envia dados para persistencia
        {
            std::string sensor_risk_str;
            std::string bpr_risk;
            std::string oxi_risk;
            std::string ecg_risk;
            std::string trm_risk;

            for(int i = 0; i < 4; i++){
                double sensor_risk = data_list[i].back();

                if(sensor_risk > 0 && sensor_risk <= 20) {
                    sensor_risk_str = "low risk";
                } else if (sensor_risk > 20 && sensor_risk <= 65) {
                    sensor_risk_str = "moderate risk";
                } else if (sensor_risk > 65 && sensor_risk <= 100) {
                    sensor_risk_str = "high risk";
                } else {
                    sensor_risk_str = "unknown";
                }

                if(i==0) {
                    trm_risk = sensor_risk_str;
                } else if (i == 1){
                    ecg_risk = sensor_risk_str;
                } else if (i == 2) {
                    oxi_risk = sensor_risk_str;
                } else {
                    bpr_risk = sensor_risk_str;
                }

            }           

            PatientStatusInfo psInfo(trm_risk, ecg_risk, oxi_risk, bpr_risk, (patient_status>=66)?"CRITICAL STATE":"NORMAL STATE");
            Container psInfoContainer(psInfo);
            getConference().send(psInfoContainer);

            std::cout << "\nMessage sent for persistence:" << endl;
            std::cout << "*****************************************" << endl;
            std::cout << psInfo.toString();
            std::cout << "*****************************************" << endl;

            received = false;
        }
        
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
