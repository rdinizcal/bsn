#include "../include/DataCollectorModule.hpp"

using namespace odcore::base::module;
using namespace bsn::data;
using namespace bsn::generator;
using namespace odcore::data;
using namespace bsn::time;
using namespace bsn::operation;
using namespace bsn::range;

DataCollectorModule::DataCollectorModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "DataCollectorModule"),
    mGeneratedData(), 
    timeRef{},
    markov_transitions() {}

DataCollectorModule::~DataCollectorModule() {}

void DataCollectorModule::setUp() {
    std::string sensorType = getKeyValueConfiguration().getValue<std::string>("datacollectormodule.type");
    int probability;
    int k = 0;
    vector<string> lrs, mrs0, hrs0, mrs1, hrs1;
    vector<Range> ranges;
    Operation operation;
    
    // Inicialização apenas necessária para transições espelhadas
    for(unsigned int i=0; i < markov_transitions.size(); i++) {
        markov_transitions[i] = 0;
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            probability = getKeyValueConfiguration().getValue<float>("datacollectormodule." + to_string(i) + "to" + to_string(j));
            if(i == 2){
                markov_transitions[k] += probability / 2;
                // função de espelho
                markov_transitions[24 - k] += probability / 2;
            }
            else {
                markov_transitions[k] = probability;
                // função de espelho
                markov_transitions[24 - k] = probability;
            }
            
            k++;
        }
        // Contador +=2 apenas necessário para transições espelhadas
        k += 2;
    }
    k=0;
    for(auto i : markov_transitions) {         
        cout << k << ": " << i << endl;
        k++;
        if(k % 5 == 0) {
            cout << endl;
        }
    }

    lrs = operation.split(getKeyValueConfiguration().getValue<string>("datacollectormodule." + sensorType + "LowRisk"), ',');
    mrs0 = operation.split(getKeyValueConfiguration().getValue<string>("datacollectormodule." + sensorType + "MidRisk0"), ',');
    hrs0 = operation.split(getKeyValueConfiguration().getValue<string>("datacollectormodule." + sensorType + "HighRisk0"), ',');
    mrs1 = operation.split(getKeyValueConfiguration().getValue<string>("datacollectormodule." + sensorType + "MidRisk1"), ',');
    hrs1 = operation.split(getKeyValueConfiguration().getValue<string>("datacollectormodule." + sensorType + "HighRisk1"), ',');

    Range r(stod(hrs0[0]), stod(hrs0[1]));
    ranges.push_back(r);
    Range r1(stod(mrs0[0]), stod(mrs0[1]));
    ranges.push_back(r1);
    Range r2(stod(lrs[0]),  stod(lrs[1]));
    ranges.push_back(r2);
    Range r3(stod(mrs1[0]), stod(mrs1[1]));
    ranges.push_back(r3);
    Range r4(stod(hrs1[0]), stod(hrs1[1]));
    ranges.push_back(r4);

    for(int i = 0; i < 5; i++) {
        ranges_array[i] = ranges[i];
    }
}

void DataCollectorModule::tearDown(){}


// Retorna os parametros necessários à distribuição normal
std::tuple<double,double> DataCollectorModule::get_normal_params(string sensorType) {
    tuple<double,double> result;
    if (sensorType == "thermometer") { // termometro
        result = std::make_tuple(36.5, 4.0);
    }
    else if (sensorType == "ecg") { // ecg
        result = std::make_tuple(80.0, 30.0);
    }
    else if (sensorType == "oximeter") { // oximetro
        result = std::make_tuple(95, 5);
    }
    else if (sensorType == "bpms") { // monitor de pressao sistolica
        result = std::make_tuple(120.0, 50.0);
    }
    else if (sensorType == "bpmd") { // monitor de pressao diastolica
        result = std::make_tuple(80.0, 50.0);
    }    
    return result;
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataCollectorModule::body(){
    DataGenerator dataGenerator;
    std::string sensorType = getKeyValueConfiguration().getValue<std::string>("datacollectormodule.type");
    timespec ts;    

    Markov markov_generator(markov_transitions, ranges_array, 2);

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        clock_gettime(CLOCK_REALTIME, &ts);
        cout << "Estado atual: " << markov_generator.current_state << endl;
        mGeneratedData = markov_generator.calculate_state();      
        markov_generator.next_state();

        RawData rawdata(mGeneratedData, sensorType, ts);
        Container container(rawdata);
        getConference().send(container);

        std::cout << "Dado " << mGeneratedData << " gerado e enviado pelo sensor: " << sensorType << std::endl;
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
