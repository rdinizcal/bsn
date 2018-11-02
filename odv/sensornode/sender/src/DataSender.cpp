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
    m_buffer() {}

DataSender::~DataSender() {}

TCPSend sender(8000);

void DataSender::setUp() {
    vector<string> sensors_types;
    map<string, vector<string>> all_configs;
    vector<string> lowR, midR0, midR1, highR0, highR1;
    Operation op;
    bool ok = true;
    int aux = 0;
    
    ip = getKeyValueConfiguration().getValue<std::string>("datasender.ip");

    
    try {
        while (ok) {
            sensors_types.push_back(getKeyValueConfiguration().getValue<string>("global.type" + to_string(aux)));
            aux++;
        }
    }
    catch (const odcore::exceptions::ValueForKeyNotFoundException &e) {
        ok = false;
    }

    sensors_types.erase(unique(sensors_types.begin(), sensors_types.end()), sensors_types.end());

    for (string str : sensors_types) {
        map<string, Range> auxiliar_map;

        highR0 = (op.split(getKeyValueConfiguration().getValue<string>("global." + str + "HighRisk0"), ','));
        midR0  = (op.split(getKeyValueConfiguration().getValue<string>("global." + str + "MidRisk0"), ','));
        lowR   = (op.split(getKeyValueConfiguration().getValue<string>("global." +  str + "LowRisk"), ','));
        midR1  = (op.split(getKeyValueConfiguration().getValue<string>("global." +  str + "MidRisk1"), ','));
        highR1 = (op.split(getKeyValueConfiguration().getValue<string>("global." +  str + "HighRisk1"), ','));

        auxiliar_map["highRange0"] = Range(stod(highR0[0]), stod(highR0[1]));
        auxiliar_map["midRange0"]  = Range(stod(midR0[0]), stod(midR0[1]));
        auxiliar_map["lowRange"]   = Range(stod(lowR[0]), stod(lowR[1]));
        auxiliar_map["midRange1"]  = Range(stod(midR1[0]), stod(midR1[1]));
        auxiliar_map["highRange1"] = Range(stod(highR1[0]), stod(highR1[1]));

        configs_map[str] = auxiliar_map;
    }

    // Recebe FilteredData
    addDataStoreFor(876, m_buffer);
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

        while (!m_buffer.isEmpty()) {            
            Container container = m_buffer.leave();
            data = container.getData<FilteredData>().getSensorData();
            type = container.getData<FilteredData>().getSensorType();
            last_time = container.getData<FilteredData>().getTime();
            now_time = last_time + ',' + time_data.get_time();

            std::cout << "Dado recebido de um " << type << ": " << data << std::endl;                        

            if (type == "bpms") {
                isOK = true;
                // eval_sys = configurations[sensor_id].evaluateNumber(systolic_value);
                package = type;
                package += '-';
                package += to_string(data);
                package += '/';
            }

            else if (type == "bpmd" and isOK) {                
                isOK = false;
                package += type;
                package += '-';
                package += to_string(data);
                
                // Adiciona os timestamps                
                package += '$';
                package += now_time + ',' + time_data.get_time();
                
                sender.send(package);
                cout << package << endl;
            }
            // Para os outros tipos apenas concatenar com '-'
            else if (type != "bmps" and type != "bpmd") {
                isOK = false;
                package = type;
                package += '-';
                package += to_string(data);
                
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
