#include "ECGFilterModule.hpp"
#include "bsn/libbsn/include/msg/MessageQueueCodes.hpp"
#define TIMEOUT_PADRAO_ECG_FAULT_TOLERANCE 200

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;
using namespace bsn::msg::info;
using namespace bsn::msg::control;

ECGFilterModule::ECGFilterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ecg"),
    buffer(),
    type("ecg"),
    active(true),
    params({{"freq",0.9},{"m_avg",5}}),
    filter(5)
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

float ECGFilterModule::noise(void){
    srand (time(NULL));

    return ((rand() % 1000000 )/1000000) + 0.5;
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ECGFilterModule::body() {
  
    Container container;
    double data;
    double type;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        /*
         * Module execution
         */
        while(!buffer.isEmpty()){ // Receive control command and module update
            container = buffer.leave();
            data = container.getData<ECGCollectTaskMsg>().getData();

             // TASK: Filter data with moving average
            filter.setRange(params["m_avg"]);
            filter.insert(data, type);
            data = filter.getValue(type);
            data = data*noise;
            bool passou = Oraculo(data);

            if(!passou)
                sleep(TIMEOUT_PADRAO_ECG_FAULT_TOLERANCE);

            ECGFilterTaskMsg sdata(data);
            Container filterContainer(sdata);
            getConference().send(filterContainer);

        }    

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
