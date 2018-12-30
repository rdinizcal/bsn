#include "OximeterModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;
using namespace bsn::msg::info;
using namespace bsn::msg::control;

OximeterModule::OximeterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "oximeter"),
    buffer(),
    type("oximeter"),
    battery(100),
    available(true),
    active(true),
    params({{"freq",0.1},{"m_avg",5}}),
    markov(),
    filter(5),
    sensorConfig() {}

OximeterModule::~OximeterModule() {}

void OximeterModule::setUp() {
    addDataStoreFor(901, buffer);
    
    Operation op;
    
    std::vector<string> t_probs;
    std::array<float, 25> transitions;
    std::array<bsn::range::Range,5> ranges;

    for(uint32_t i = 0; i < transitions.size(); i++){
        for(uint32_t j = 0; j < 5; j++){
            t_probs = op.split(getKeyValueConfiguration().getValue<std::string>("oximeter.state"+to_string(j)), ',');
            for(uint32_t k = 0; k < 5; k++){
                transitions[i++] = stod(t_probs[k]);
            }
        }
    }
    
    { // Configure markov chain
        vector<string> lrs,mrs,hrs;

        lrs = op.split(getKeyValueConfiguration().getValue<string>("oximeter.LowRisk"), ',');
        mrs = op.split(getKeyValueConfiguration().getValue<string>("oximeter.MidRisk"), ',');
        hrs = op.split(getKeyValueConfiguration().getValue<string>("oximeter.HighRisk"), ',');

        ranges[0] = Range(-1, -1);
        ranges[1] = Range(-1, -1);
        ranges[2] = Range(stod(lrs[0]),  stod(lrs[1]));
        ranges[3] = Range(stod(mrs[0]), stod(mrs[1]));
        ranges[4] = Range(stod(hrs[0]), stod(hrs[1]));

        markov = Markov(transitions, ranges, 2);
    }

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

void OximeterModule::tearDown(){}

void OximeterModule::sendContextInfo(const std::string &task_id, const double &cost, const double &reliability) {
    ContextInfo context(task_id,cost,reliability);
    Container contextContainer(context);
    getConference().send(contextContainer);
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode OximeterModule::body(){

    Container container;
    double data;
    double risk;
    double nCycles = 0;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        /*
         * Receive control command and module update
         */
        while(!buffer.isEmpty()){
            container = buffer.leave();

            active = container.getData<OximeterControlCommand>().getActive();
            params = container.getData<OximeterControlCommand>().getParams();
        }

        if(!active){ continue; }

        /*
         * Module execution
         */
        if(++nCycles >= int32_t(1.0/params["freq"])){
            
            { // TASK: Collect oximeter data
                data = markov.calculate_state();      
                markov.next_state();
                battery -= 0.001;
                
                sendContextInfo("T1.13",0.001,100);

                //for debugging 
                cout << "Dado gerado: " << data << endl;
            }
            
            { // TASK: Filter data with moving average
                filter.setRange(params["m_avg"]);
                filter.insert(data, type);
                data = filter.getValue(type);
                battery -= 0.005*params["m_avg"];

                sendContextInfo("T2.3",0.005*params["m_avg"],100);

                //for debugging 
                cout << "Dado filtrado: " << data << endl;
            }
            
            { // TASK: Transfer information to CentralHub
                risk = sensorConfig.evaluateNumber(data);
                
                SensorData sdata(type, data, risk);
                Container sdataContainer(sdata);
                getConference().send(sdataContainer);
                battery -= 0.01;

                sendContextInfo("T3.3",0.01,100);

                // for debugging
                cout << sdata.toString() << endl;
            }

            nCycles = 0;
        }

        // TODO: persist it here
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
