#include "../include/DataSender.hpp"

using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data;
using namespace bsn::data;
using namespace bsn::time;
using namespace bsn::configuration;
using namespace bsn::operation;
using namespace bsn::range;
using namespace std;

DataSender::DataSender(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "datasender"),
    mBuffer() {}

DataSender::~DataSender() {}

TCPSend sender(8000);

void DataSender::setUp() {
    vector<string> sensorsTypes;
    map<string, vector<string>> all_configs;
    vector<string> lowR, midR0, midR1, highR0, highR1;
    vector<string> lowPercentage, midPercentage, highPercentage;
    Operation op;
    bool ok = true;
    int32_t aux = 0;

    // Configurações para SensorConfiguration
    Range low_range;
    array<Range,2> midRanges, highRanges;
    array<Range,3> percentages;
    
    ip = getKeyValueConfiguration().getValue<std::string>("datasender.ip");

    
    try {
        while (ok) {
            sensorsTypes.push_back(getKeyValueConfiguration().getValue<string>("global.type" + to_string(aux)));
            aux++;
        }
    }
    catch (const odcore::exceptions::ValueForKeyNotFoundException &e) {
        ok = false;
    }

    sensorsTypes.erase(unique(sensorsTypes.begin(), sensorsTypes.end()), sensorsTypes.end());    

    for (string str : sensorsTypes) {

        highR0 = (op.split(getKeyValueConfiguration().getValue<string>("global." + str + "HighRisk0"), ','));
        midR0  = (op.split(getKeyValueConfiguration().getValue<string>("global." + str + "MidRisk0"), ','));
        lowR   = (op.split(getKeyValueConfiguration().getValue<string>("global." +  str + "LowRisk"), ','));
        midR1  = (op.split(getKeyValueConfiguration().getValue<string>("global." +  str + "MidRisk1"), ','));
        highR1 = (op.split(getKeyValueConfiguration().getValue<string>("global." +  str + "HighRisk1"), ','));
        lowPercentage  = (op.split(getKeyValueConfiguration().getValue<string>("datasender.lowPercentage"), ','));
        midPercentage  = (op.split(getKeyValueConfiguration().getValue<string>("datasender.midPercentage"), ','));
        highPercentage = (op.split(getKeyValueConfiguration().getValue<string>("datasender.highPercentage"), ','));

        low_range      = Range(stod(lowR[0]), stod(lowR[1]));

        highRanges[0] = Range(stod(highR0[0]), stod(highR0[1])); 
        highRanges[1] = Range(stod(highR1[0]), stod(highR1[1]));

        midRanges[0]  = Range(stod(midR0[0]), stod(midR0[1])); 
        midRanges[1]  = Range(stod(midR1[0]), stod(midR1[1]));

        percentages[0] = Range(stod(lowPercentage[0]), stod(lowPercentage[1]));
        percentages[1] = Range(stod(midPercentage[0]), stod(midPercentage[1]));
        percentages[2] = Range(stod(highPercentage[0]), stod(highPercentage[1]));        

        configsVet.push_back(SensorConfiguration(0,low_range,midRanges,highRanges,percentages));
    }


    configsVet[0].print();

    // Recebe FilteredData
    addDataStoreFor(876, mBuffer);
    // Conecta o sender em uma porta
    sender.set_port(getIdentifier());
    sender.setIP(ip);
    sender.connect();
}

void DataSender::tearDown(){    
    sender.disconnect();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataSender::body(){
    string type;
    double data;    
    bool isOK = false;
    TimeData time_data;
    string package;
    string last_time, now_time;   

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) { 

        while (!mBuffer.isEmpty()) {            
            Container container = mBuffer.leave();
            data = container.getData<FilteredData>().getSensorData();
            type = container.getData<FilteredData>().getSensorType();
            last_time = container.getData<FilteredData>().getTime();
            now_time = last_time + ',' + time_data.get_time();

            std::cout << "Dado recebido de um " << type << ": " << data << std::endl;                        

            if (type == "bpms") {
                double evaluated = configsVet[0].evaluateNumber(data);                
                isOK = true;
                // eval_sys = configurations[sensor_id].evaluateNumber(systolic_value);
                package = type;
                package += '-';
                package += to_string(data) + '&';
                package += to_string(evaluated);

                // Adiciona os timestamps                
                package += '$';
                package += now_time + ',' + time_data.get_time();

                // Add o separador de pacote
                package += '/';
            }

            else if (type == "bpmd" and isOK) {
                double evaluated = configsVet[1].evaluateNumber(data);                
                isOK = false;
                package += type;
                package += '-';
                package += to_string(data) + '&';
                package += to_string(evaluated);
                
                // Adiciona os timestamps                
                package += '$';
                package += now_time + ',' + time_data.get_time();
                
                sender.send(package);
                cout << package << endl;
            }
            // Para os outros tipos apenas concatenar com '-'
            else if (type != "bmps" and type != "bpmd") {
                double evaluated = configsVet[0].evaluateNumber(data);                
                isOK = false;
                package = type;
                package += '-';
                package += to_string(data) + '&';
                package += to_string(evaluated);

                
                // Adiciona os timestamps                
                package += '$';
                package += now_time + ',' + time_data.get_time();
                
                sender.send(package);
                cout << package << endl;
            }
 
            SensorStatusInfo sStatusInfo(data);
            Container sStatusContainer(sStatusInfo);
            getConference().send(sStatusContainer);

            /*
             * Para cada execução do loop, contabilizar e enviar duas unidades de bateria consumida
             * */
            ResourceUpdate rUpdate(-2);
            Container rUpdContainer(rUpdate);
            getConference().send(rUpdContainer);
        }

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
