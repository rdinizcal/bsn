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
    dataBuffer(),
    mGeneratedData(), 
    timeRef{},
    markovTransitions() {}

DataCollectorModule::~DataCollectorModule() {}

void DataCollectorModule::setUp() {
    addDataStoreFor(877, dataBuffer);

    int32_t probability;
    int32_t k = 0;
    vector<string> lrs, mrs0, hrs0, mrs1, hrs1;
    vector<Range> ranges;
    Operation operation;

    std::string sensorType = getKeyValueConfiguration().getValue<std::string>("global.type"+to_string(getIdentifier()));

    // Inicialização apenas necessária para transições espelhadas
    for(uint32_t i=0; i < markovTransitions.size(); i++) {
        markovTransitions[i] = 0;
    }

    for (int32_t i = 0; i < 3; i++) {
        for (int32_t j = 0; j < 3; j++) {
            probability = getKeyValueConfiguration().getValue<float>("datacollectormodule." + to_string(i) + "to" + to_string(j));
            if(i == 2){
                markovTransitions[k] += probability / 2;
                // função de espelho
                markovTransitions[24 - k] += probability / 2;
            }
            else {
                markovTransitions[k] = probability;
                // função de espelho
                markovTransitions[24 - k] = probability;
            }
            
            k++;
        }
        // Contador +=2 apenas necessário para transições espelhadas
        k += 2;
    }

  
    lrs = operation.split(getKeyValueConfiguration().getValue<string>("global." + sensorType + "LowRisk"), ',');
    mrs0 = operation.split(getKeyValueConfiguration().getValue<string>("global." + sensorType + "MidRisk0"), ',');
    hrs0 = operation.split(getKeyValueConfiguration().getValue<string>("global." + sensorType + "HighRisk0"), ',');
    mrs1 = operation.split(getKeyValueConfiguration().getValue<string>("global." + sensorType + "MidRisk1"), ',');
    hrs1 = operation.split(getKeyValueConfiguration().getValue<string>("global." + sensorType + "HighRisk1"), ',');

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

    for(int32_t i = 0; i < 5; i++) {
        rangesArray[i] = ranges[i];
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
    TimeData time_data;
    std::string sensorType = getKeyValueConfiguration().getValue<std::string>("global.type"+to_string(getIdentifier()));    

    Markov markovGenerator(markovTransitions, rangesArray, 2);

    Container container;
    int32_t freq = 10;
    int32_t nCycles = 0;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        //Receber nova freq para gerar dados
        while(!dataBuffer.isEmpty()) {
            container = dataBuffer.leave();
            freq = container.getData<FreqUpdate>().getFreq();
            std::cout<< "Recebi a nova frequencia: " << freq << endl; 
        }

        if(++nCycles >= freq){
            string now_time = time_data.get_time();
            cout << "Estado atual: " << markovGenerator.currentState << endl;
            mGeneratedData = markovGenerator.calculate_state();      
            markovGenerator.next_state();

            RawData rawdata(mGeneratedData, sensorType, now_time);
            Container container(rawdata);
            getConference().send(container);

            /*
             * Para cada execução do loop, contabilizar e enviar uma unidade de bateria consumida
             * */
            ResourceUpdate rUpdate(-1);
            Container rUpdContainer(rUpdate);
            getConference().send(rUpdContainer);

            nCycles = 0;
            std::cout << "Dado " << mGeneratedData << " gerado e enviado pelo sensor: " << sensorType << std::endl;
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
