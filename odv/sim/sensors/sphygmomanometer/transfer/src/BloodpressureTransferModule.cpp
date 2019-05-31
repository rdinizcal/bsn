#include "BloodpressureTransferModule.hpp"
#include "bsn/libbsn/include/msg/MessageQueueCodes.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::resource;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::taskMsg;

BloodpressureTransferModule::BloodpressureTransferModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "bloodpressure"),
    buffer(),
    type("bloodpressure"),
    active(true),
    params({{"freq",0.90},{"m_avg",5}}),
    sensorConfig()
    {}

BloodpressureTransferModule::~BloodpressureTransferModule() {}

void BloodpressureTransferModule::setUp() {
    //srand(time(NULL));
    addDataStoreFor(BLOODPRESSURETRANSFERMODULE_MSG_QUE, buffer);

    // Configure sensor configuration
    Range low_range = ranges[2];
    
    array<Range,2> midRanges;
    midRanges[0] = ranges[1];
    midRanges[1] = ranges[3];
    
    array<Range,2> highRanges;
    highRanges[0] = ranges[0];
    highRanges[1] = ranges[4];

    array<Range,3> percentages;

    vector<string> low_p = op.split(getKeyValueConfiguration().getValue<string>("global.lowrisk"), ',');
    percentages[0] = Range(stod(low_p[0]),stod(low_p[1]));

    vector<string> mid_p = op.split(getKeyValueConfiguration().getValue<string>("global.midrisk"), ',');
    percentages[1] = Range(stod(mid_p[0]),stod(mid_p[1]));

    vector<string> high_p = op.split(getKeyValueConfiguration().getValue<string>("global.highrisk"), ',');
    percentages[2] = Range(stod(high_p[0]),stod(high_p[1]));

    if(i==0){
        sensorConfigSystolic = SensorConfiguration(0,low_range,midRanges,highRanges,percentages);
    } else {
        sensorConfigDiastolic = SensorConfiguration(0,low_range,midRanges,highRanges,percentages);
    }
        

}

void BloodpressureTransferModule::tearDown() {
    
}


odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BloodpressureTransferModule::body(){

    Container container;
    double filterS;
    double filterD;
    double riskS;
    double riskD;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        

      

        while(!buffer.isEmpty()){ // Receive control command and module update
            container = buffer.leave();

            filterS = container.getData<BloodpressureFilterTaskMsg>().getDataS();
            filterD = container.getData<BloodpressureFilterTaskMsg>().getDataD();        

       
        /*
         * Module execution
         */        
            
        //TASK: Transfer information to CentralHub
            riskS = sensorConfigSystolic.evaluateNumber(filterS);
            BloodpressureTransferTaskMsg transferSMsg(riskS);
            Container transferSContainer(transferSMsg);
            getConference().send(transferSContainer);

            riskD = sensorConfigDiastolic.evaluateNumber(filterD);
            BloodpressureTransferTaskMsg transferDMsg(riskD);
            Container transferDContainer(transferDMsg);
            getConference().send(transferDContainer);

        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
