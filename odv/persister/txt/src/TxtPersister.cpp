#include "../include/TxtPersister.hpp"

using namespace odcore::data; 
using namespace bsn::data;

TxtPersister::TxtPersister(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "TxtPersister"),
    data_buffer() {}

TxtPersister::~TxtPersister() {}

void TxtPersister::setUp() {
    addDataStoreFor(875,data_buffer);
    addDataStoreFor(880,data_buffer);

    //std::string path = "./output/";    
    fp.open(/*path+*/"centralhub_output.csv");
    fp << "ID,SPO2_DATA,ECG_DATA,TEMP_DATA,BLOODPRESSURE_DATA,BATTERY,PATIENT_STATE" << endl; // colunas separadas por virgula
}

void TxtPersister::tearDown(){
    fp.close();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode TxtPersister::body(){

    Container container;
    ResourceInfo rInfo;
    PatientStatusInfo psInfo;
    int id = 0;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        bool flag = false;
        while (!data_buffer.isEmpty()) {
            
            container = data_buffer.leave();

            if(container.getDataType() == 875){
                rInfo = container.getData<ResourceInfo>();
                std::cout << "Message received:" << endl;
                std::cout << "*****************************************" << endl;
                std::cout << rInfo.toString();
                std::cout << "*****************************************" << endl;
            } else if (container.getDataType() == 880){
                psInfo = container.getData<PatientStatusInfo>();
                std::cout << "Message received:" << endl;
                std::cout << "*****************************************" << endl;
                std::cout << psInfo.toString();
                std::cout << "*****************************************" << endl;
            } else {
                std::cout << "ERROR: unrecognized consumed data." << endl;
            }

            flag = true;
        }

        if(flag){
            //persist lines
            fp << id++ << ",";
            fp << psInfo.getOximeterRisk() << ",";
            fp << psInfo.getECGRisk() << ",";
            fp << psInfo.getThermometerRisk() << ",";
            fp << psInfo.getBloodPressureRisk() << ",";
            fp << rInfo.getResource().getCurrentLevel() << ",";
            fp << psInfo.getPatientStatus() << "\n";
        }
        
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
