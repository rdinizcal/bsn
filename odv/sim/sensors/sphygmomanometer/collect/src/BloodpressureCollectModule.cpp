#include "BloodpressureCollectModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation; 

using namespace bsn::msg::taskMsg;

BloodpressureCollectModule::BloodpressureCollectModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "bloodpressure"),
    buffer(),
    type("bloodpressure"),
    active(true),
    params({{"freq",0.90},{"m_avg",5}}),
    markovSystolic(),
    markovDiastolic(),
    falhaRand()
    {}

BloodpressureCollectModule::~BloodpressureCollectModule() {}

void BloodpressureCollectModule::setUp() {
    addDataStoreFor(BLOODPRESSURECOLLECTMODULE_MSG_QUE, buffer);
    
    Operation op;
    for(int32_t i = 0; i < 2; i++){
        std::vector<string> t_probs;
        std::array<float, 25> transitions;
        std::array<bsn::range::Range,5> ranges;
        vector<string> lrs,mrs,hrs;
        
        std::string x = (i==0)? "syst" : "dias";

        for(uint32_t i = 0; i < transitions.size(); i++){
            for(uint32_t j = 0; j < 5; j++){
                t_probs = op.split(getKeyValueConfiguration().getValue<std::string>("bloodpressure."+x+"state"+to_string(j)), ',');
                for(uint32_t k = 0; k < 5; k++){
                    transitions[i++] = stod(t_probs[k]);
                }
            }
        }

        { // Configure markov chain
            lrs = op.split(getKeyValueConfiguration().getValue<string>("bloodpressure."+x+"LowRisk"), ',');
            mrs = op.split(getKeyValueConfiguration().getValue<string>("bloodpressure."+x+"MidRisk"), ',');
            hrs = op.split(getKeyValueConfiguration().getValue<string>("bloodpressure."+x+"HighRisk"), ',');

            ranges[0] = Range(-1, -1);
            ranges[1] = Range(-1, -1);
            ranges[2] = Range(stod(lrs[0]), stod(lrs[1]));
            ranges[3] = Range(stod(mrs[0]), stod(mrs[1]));
            ranges[4] = Range(stod(hrs[0]), stod(hrs[1]));
            
            if(i==0){
                markovSystolic = Markov(transitions, ranges, 2);
            } else {
                markovDiastolic = Markov(transitions, ranges, 2);
            }
        }
    }
}
void BloodpressureCollectModule::tearDown() {
    
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BloodpressureCollectModule::body(){

    double dataS;
    double dataD;
    
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        

        if(falhaRand.seOcorreuFalha() ){
                usleep(40000);
        }
        
        // Apenas executa uma vez a cada segundo
        
            dataS = markovSystolic.calculate_state();
            dataD = markovDiastolic.calculate_state();

            markovSystolic.next_state();
            markovDiastolic.next_state();
            
            BloodpressureCollectTaskMessage collectMsg(dataS, dataD);
            Container collectContainer(collectMsg);
            getConference().send(collectContainer);
           
            
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
