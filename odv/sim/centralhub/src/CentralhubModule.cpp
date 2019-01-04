#include "CentralhubModule.hpp"

using namespace odcore::data;

using namespace bsn::processor;

using namespace bsn::msg::data;
using namespace bsn::msg::info;

using namespace bsn::communication;


CentralhubModule::CentralhubModule(const int32_t &argc, char **argv) :
TimeTriggeredConferenceClientModule(argc, argv, "centralhub"),
	buffer(),
    connect(1),
    port(6060),
    ip("localhost"),
    persist(1),
    fp(),
    path("centralhub_output.csv") {}
	
CentralhubModule::~CentralhubModule() {}

void CentralhubModule::setUp() {
    addDataStoreFor(873, buffer);
    connect = getKeyValueConfiguration().getValue<int>("centralhub.connect");
    port = getKeyValueConfiguration().getValue<int>("centralhub.port");
    ip = getKeyValueConfiguration().getValue<std::string>("centralhub.ip");
    persist = getKeyValueConfiguration().getValue<int>("centralhub.persist");
    path = getKeyValueConfiguration().getValue<std::string>("centralhub.path");

    if (persist) {
        fp.open(path);
        fp << "ID,SPO2_DATA,ECG_DATA,TEMP_DATA,BLOODPRESSURE_DATA,OVERRAL_STATUS,PATIENT_STATE" << endl;
    }
}

void CentralhubModule::tearDown() {
    if (persist)
        fp.close();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode CentralhubModule::body() {
    Container container;    
    double patient_status;
    bool received = false;
    std::string packet;
    std::array<double, 5> data;
    int id = 0;

    TCPSend sender;
    if (connect) {
        sender.set_port(port);
        sender.setIP(ip);
        sender.connect();
    }

    vector<list<double>> data_list(5);
    for(std::vector<std::list<double>>::iterator it = data_list.begin();
        it != data_list.end(); ++it) {
            (*it).push_back(0.0);
    }

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        while (!buffer.isEmpty()) {
            
            container = buffer.leave();

            std::string type = container.getData<SensorData>().getType();
            double risk = container.getData<SensorData>().getRisk();
            int32_t sensor_id = get_sensor_id(type);
            data[sensor_id] = container.getData<SensorData>().getData();

            if (int32_t(risk) == -1) { continue; }
            data_list[sensor_id].push_back(risk);

            received = true;
        }

        if (!received) { continue; }

        patient_status = data_fuse(data_list);

        // Envia dados ao servidor, se configurado para tal
        {   
            if (connect) {
                packet = "";
                int i = 0;
                for (list<double> li : data_list) {
                    if (!li.empty()) {
                        double element = li.front();
                        packet += to_string(element) += "=";
                        packet += to_string(data[i]) + "/";
                    }
                    i++;                    
                }
                packet += to_string(patient_status);
                sender.send(packet);
            }
        }

        // Envia dados para persistencia
        {
            std::string sensor_risk_str;
            std::string bpr_risk;
            std::string oxi_risk;
            std::string ecg_risk;
            std::string trm_risk;

            for (int i = 0; i < 4; i++) {
                double sensor_risk = data_list[i].back();

                if (sensor_risk > 0 && sensor_risk <= 20) {
                    sensor_risk_str = "low risk";
                } else if (sensor_risk > 20 && sensor_risk <= 65) {
                    sensor_risk_str = "moderate risk";
                } else if (sensor_risk > 65 && sensor_risk <= 100) {
                    sensor_risk_str = "high risk";
                } else {
                    sensor_risk_str = "unknown";
                }

                if (i==0) {
                    trm_risk = sensor_risk_str;
                } else if (i == 1){
                    ecg_risk = sensor_risk_str;
                } else if (i == 2) {
                    oxi_risk = sensor_risk_str;
                } else {
                    bpr_risk = sensor_risk_str;
                }

            }           

            if (persist) {
                fp << id++ << ",";
                fp << oxi_risk << ",";
                fp << ecg_risk << ",";
                fp << trm_risk << ",";
                fp << bpr_risk << ",";
                fp << patient_status << ",";
                fp << ((patient_status>=66)?"CRITICAL STATE":"NORMAL STATE") << endl;
            }

            cout << endl << "*****************************************" << endl;
            cout << "PatientStatusInfo#" << endl;
            cout << "| THERM_RISK: " << trm_risk << endl;
            cout << "| ECG_RISK: " << ecg_risk << endl;
            cout << "| OXIM_RISK: " << oxi_risk << endl;
            cout << "| BPRESS_RISK: " << bpr_risk << endl;
            cout << "| PACIENT_STATE:" << ((patient_status>=66)?"CRITICAL STATE":"NORMAL STATE") << endl;
            cout << "*****************************************" << endl;
        }

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
