#include "OximeterTransferModule.hpp"
#include "bsn/msg/MessageQueueCodes.hpp"


using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::taskMsg;

OximeterTransferModule::OximeterTransferModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "oximeter"),
    buffer(),
    type("oximeter"),
    available(true),
    active(true),
    params({{"freq",0.90},{"m_avg",5}}),
    sensorConfig()
    {}

OximeterTransferModule::~OximeterTransferModule() {}

void OximeterTransferModule::setUp() {
    addDataStoreFor(OXIMETERTRANSFERMODULE_MSG_QUE, buffer);
    
        Operation op;
    
    std::vector<string> t_probs;
    std::array<float, 25> transitions;
    std::array<bsn::range::Range,5> ranges;

    { // Configure sensor configuration
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

        sensorConfig = SensorConfiguration(0,low_range,midRanges,highRanges,percentages);
    }
}

void OximeterTransferModule::tearDown() {

}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode OximeterTransferModule::body(){

    double filterResponse;
    double risk;
    Container filterContainer;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        
        while(!buffer.isEmpty()){ // Receive control command and module update
            
            // Recebe dados do Task anterior
            filterContainer = buffer.leave();
            filterResponse = FilterContainer.getData<OximeterFilterTaskMsg>().getActive();

            risk = sensorConfig.evaluateNumber(filterResponse);

            OximeterTransferTaskMsg cData(data);
            Container TransferContainer(cData);
            getConference().send(TransferContainer);
        }

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}