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
    timeRef() {}

DataCollectorModule::~DataCollectorModule() {}

void DataCollectorModule::setUp() {
    std::string sensorType = getKeyValueConfiguration().getValue<std::string>("datacollectormodule.type");
    int probability;
    int k = 0;
    vector<string> low, mid, high;
    vector<Range> ranges;
    Operation operation;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            probability = getKeyValueConfiguration().getValue<float>("datacollectormodule." + to_string(i) + "to" + to_string(j));
            markov_transitions[k] = probability;
            k++;
        }
    }

    low = operation.split(getKeyValueConfiguration().getValue<string>("datacollectormodule." + sensorType + "lowRange"), ',');
    mid = operation.split(getKeyValueConfiguration().getValue<string>("datacollectormodule." + sensorType + "midRange"), ',');
    high = operation.split(getKeyValueConfiguration().getValue<string>("datacollectormodule." + sensorType + "highRange"), ',');

    Range lowRange(stod(low[0]), stod(low[1]));
    Range midRange(stod(mid[0]), stod(mid[1]));
    Range highRange(stod(high[0]), stod(high[1]));

    ranges_array[0] = lowRange;
    ranges_array[1] = midRange;
    ranges_array[2] = highRange;
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

    Markov markov_generator(markov_transitions, ranges_array, 1);

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        clock_gettime(CLOCK_REALTIME, &ts);
        cout << "Estado atual: " << markov_generator.current_state << endl;
        mGeneratedData = markov_generator.calculate_state();      
        markov_generator.next_state();

        RawData rawdata(mGeneratedData, sensorType, ts);
        Container container(rawdata);
        getConference().send(container);

        /*
         * Para cada execução do loop, contabilizar e enviar uma unidade de bateria consumida
         * */
        ResourceUpdate rUpdate(-1);
        Container rUpdContainer(rUpdate);
        getConference().send(rUpdContainer);

        std::cout << "Dado " << mGeneratedData << " gerado e enviado pelo sensor: " << sensorType << std::endl;
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
