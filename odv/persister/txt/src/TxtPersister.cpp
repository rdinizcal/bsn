#include "../include/TxtPersister.hpp"

using namespace odcore::data; 
using namespace bsn::data;

TxtPersister::TxtPersister(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "TxtPersister"),
    data_buffer() {}

TxtPersister::~TxtPersister() {}

void TxtPersister::setUp() {
    addDataStoreFor(875,data_buffer);

    std::string path = "output/";    
    fp.open(path+"centralhub"+"_output.csv");
    fp << "Battery" << endl; // colunas separadas por virgula
}

void TxtPersister::tearDown(){
    fp.close();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode TxtPersister::body(){

    Container container;
    vector<ResourceInfo> rInfoVec;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        while (!data_buffer.isEmpty()) {
            
            container = data_buffer.leave();
            rInfoVec.push_back(container.getData<ResourceInfo>());
        }

        //persist lines
        for(vector<ResourceInfo>::iterator it = rInfoVec.begin(); it != rInfoVec.end(); it++) {
            fp << (*it).getResource().getCurrentLevel() << endl;
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
