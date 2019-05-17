#include "ThermCollector.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::resource;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;

ThermCollector::ThermCollector(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "thermometer"),
    buffer(),
    type("thermometer"),
    battery("therm_batt",100,100,1),
    available(true),
    data_accuracy(1),
    comm_accuracy(1),
    active(true),
    params({{"freq",0.9},{"m_avg",5}}),
    markov(),
    filter(5),
    sensorConfig(),
    persist(1),
    path("thermometer_output.csv"),
    fp() {}

ThermCollector::~ThermCollector() {}

void ThermCollector::setUp() {
    //srand(time(NULL));
    addDataStoreFor(900, buffer);
    
    Operation op;
    
    std::vector<string> t_probs;
    std::array<float, 25> transitions;
    std::array<bsn::range::Range,5> ranges;

    for(uint32_t i = 0; i < transitions.size(); i++){
        for(uint32_t j = 0; j < 5; j++){
            t_probs = op.split(getKeyValueConfiguration().getValue<std::string>("thermometer.state"+to_string(j)), ',');
            for(uint32_t k = 0; k < 5; k++){
                transitions[i++] = stod(t_probs[k]);
            }
        }
    }
    
    { // Configure markov chain
        vector<string> lrs,mrs0,hrs0,mrs1,hrs1;

        lrs = op.split(getKeyValueConfiguration().getValue<string>("thermometer.LowRisk"), ',');
        mrs0 = op.split(getKeyValueConfiguration().getValue<string>("thermometer.MidRisk0"), ',');
        hrs0 = op.split(getKeyValueConfiguration().getValue<string>("thermometer.HighRisk0"), ',');
        mrs1 = op.split(getKeyValueConfiguration().getValue<string>("thermometer.MidRisk1"), ',');
        hrs1 = op.split(getKeyValueConfiguration().getValue<string>("thermometer.HighRisk1"), ',');

        ranges[0] = Range(stod(hrs0[0]), stod(hrs0[1]));
        ranges[1] = Range(stod(mrs0[0]), stod(mrs0[1]));
        ranges[2] = Range(stod(lrs[0]),  stod(lrs[1]));
        ranges[3] = Range(stod(mrs1[0]), stod(mrs1[1]));
        ranges[4] = Range(stod(hrs1[0]), stod(hrs1[1]));

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

    { // Configure sensor data_accuracy
        data_accuracy = getKeyValueConfiguration().getValue<double>("thermometer.data_accuracy") / 100;
        comm_accuracy = getKeyValueConfiguration().getValue<double>("thermometer.data_accuracy") / 100;
    }

}

void ThermCollector::tearDown() {
    if (persist)
        fp.close();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ThermCollector::body(){

    Container container;
    double data;
    double risk;
    uint32_t id = 0;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        

        /*
         * Module execution
         */
        if((rand() % 100)+1 < int32_t(params["freq"]*100)){
           
            { // TASK: Collect thermometer data with data_accuracy
                
                data = markov.calculate_state();
                
                double offset = (1 - data_accuracy + (double)rand() / RAND_MAX * (1 - data_accuracy)) * data;

                if (rand() % 2 == 0)
                    data = data + offset;
                else
                    data = data - offset;

                markov.next_state();
                battery.consume(0.1);

                //for debugging
                std::cout << "New data: " << data << endl << endl;
                
            }

            { // TASK: Filter data with moving average
                filter.setRange(params["m_avg"]);
                filter.insert(data, type);
                data = filter.getValue(type);
                battery.consume(0.1*params["m_avg"]);

                //for debugging 
                //cout << "Filtered data: " << data << endl;
            }
            
            { // TASK: Transfer information to CentralHub
                risk = sensorConfig.evaluateNumber(data);
            
                SensorData sdata(type, data, risk);
                Container sdataContainer(sdata);
                if((rand() % 100)+1 > int32_t(comm_accuracy*100)) getConference().send(sdataContainer);
                battery.consume(0.1);

                // for debugging
                //cout << "Risk: " << risk << "%"  << endl;
            }
            
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
