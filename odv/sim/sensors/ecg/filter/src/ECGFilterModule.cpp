#include "ECGFilterModule.hpp"

#define TIMEOUT_PADRAO_ECG_FAULT_TOLERANCE 1

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;

using namespace bsn::msg::taskMsg;

ECGFilterModule::ECGFilterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ecgfilter"),
    buffer(),
    type("ecg"),
    active(true),
    params({{"freq",0.9},{"m_avg",5}}),
    filter(5),
    falhaRand()
    {}

ECGFilterModule::~ECGFilterModule() {}

void ECGFilterModule::setUp() {
    
    addDataStoreFor(ECGFILTERMODULE_MSG_QUE, buffer);

}

void ECGFilterModule::tearDown() {
   
}

bool ECGFilterModule::Oraculo(double dados){
    bool dados_validos = true;
    return dados_validos;
    
}

double ECGFilterModule::noise(void){
    srand (time(NULL));

    return ((rand() % 1000000 )/1000000) + 0.5;
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ECGFilterModule::body() {
  
    Container container;
    double data;
    double noise;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        /*
         * Module execution
         */

        if(falhaRand.seOcorreuFalha() ){
                usleep(41000);
        }

        while(!buffer.isEmpty()){ // Receive control command and module update
            container = buffer.leave();
            data = container.getData<ECGCollectTaskMessage>().getData();

             // TASK: Filter data with moving average
            filter.setRange(params["m_avg"]);
            filter.insert(data, type);
            data = filter.getValue(type);
            data = data*noise;
            bool passou = Oraculo(data);

            if(!passou)
                sleep(TIMEOUT_PADRAO_ECG_FAULT_TOLERANCE);

            ECGFilterTaskMessage sdata(data);
            Container filterContainer(sdata);
            getConference().send(filterContainer);

        }    

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
