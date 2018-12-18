#include "../include/ControllerModule.hpp"

using namespace odcore::data;

using namespace bsn::data;
using namespace bsn::operation;
using namespace bsn::range;

ControllerModule::ControllerModule(const int32_t  &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ControllerModule"),
    dataBuffer(),
    rangesArray() {}

ControllerModule::~ControllerModule() {}

void ControllerModule::setUp() {
    addDataStoreFor(878, dataBuffer);
    
    vector<string> low, mid, high;
    vector<Range> ranges;
    Operation op;

    std::string sensorType = getKeyValueConfiguration().getValue<std::string>("global.type"+to_string(getIdentifier()));

    vector<std::string> highR0 = (op.split(getKeyValueConfiguration().getValue<string>("global." + sensorType + "HighRisk0"), ','));
    vector<std::string> midR0  = (op.split(getKeyValueConfiguration().getValue<string>("global." + sensorType + "MidRisk0"), ','));
    vector<std::string> lowR   = (op.split(getKeyValueConfiguration().getValue<string>("global." + sensorType + "LowRisk"), ','));
    vector<std::string> midR1  = (op.split(getKeyValueConfiguration().getValue<string>("global." + sensorType + "MidRisk1"), ','));
    vector<std::string> highR1 = (op.split(getKeyValueConfiguration().getValue<string>("global." + sensorType + "HighRisk1"), ','));

    Range lowRange   = Range(stod(lowR[0]), stod(lowR[1]));

    Range highRange0 = Range(stod(highR0[0]), stod(highR0[1])); 
    Range highRange1 = Range(stod(highR1[0]), stod(highR1[1]));

    Range midRange0  = Range(stod(midR0[0]), stod(midR0[1])); 
    Range midRange1  = Range(stod(midR1[0]), stod(midR1[1]));

    rangesArray[0] = lowRange;
    rangesArray[1] = midRange0;
    rangesArray[2] = midRange1;
    rangesArray[3] = highRange0;
    rangesArray[4] = highRange1;
}

void ControllerModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ControllerModule::body(){
   
    Container container;
    double rawVal;
    int32_t sensorStatus = 0;
    double freq = 0.1;
    double lowRiskFreq = 0.1;
    double medRiskFreq = 0.5;
    double highRiskFreq = 1;
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        while(!dataBuffer.isEmpty()) {
            
            //Receber o dado do sensor
            container = dataBuffer.leave();
            rawVal = container.getData<SensorStatusInfo>().getValue();

            //verificar o estado do sensor
            int32_t newStatus;
            if(rangesArray[0].in_range(rawVal)) {
                newStatus = 0;
            } else if(rangesArray[1].in_range(rawVal) || rangesArray[2].in_range(rawVal)) {
                newStatus = 1;
            } else if(rangesArray[3].in_range(rawVal) || rangesArray[4].in_range(rawVal)) {
                newStatus = 2;
            } else {
                std::cout << "Unknown state value (" << rawVal << ")" << endl;
            }
            

            //se necessario, enviar uma mensagem ao DataCollector com a freq
            if(sensorStatus != newStatus) {

                sensorStatus = newStatus;

                switch(sensorStatus){
                    case 0:
                        freq = lowRiskFreq;
                        break;
                    case 1:
                        freq = medRiskFreq;
                        break;
                    case 2: 
                        freq = highRiskFreq;
                        break;
                }
                FreqUpdate fUpdate(freq);
                Container fUpdContainer(fUpdate);
                getConference().send(fUpdContainer);

                std::cout << "Estado atual do sensor: " << sensorStatus << endl;
                std::cout << "Frequencia enviada ao collector: " << freq << " Hz" << endl ;
            }

        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
