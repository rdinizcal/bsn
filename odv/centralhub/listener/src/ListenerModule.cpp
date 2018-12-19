#include "ListenerModule.hpp"

using namespace std;

using namespace odcore::base::module;
using namespace odcore::data;
using namespace bsn::msg::data;
using namespace bsn::operation;
using namespace bsn::communication;

TCPReceive server(8000);

ListenerModule::ListenerModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ListenerModule")
    {}

ListenerModule::~ListenerModule() {}

void ListenerModule::setUp() {
    // É usado o id do módulo para receber a porta desejada
    server.set_port(getIdentifier());
    server.start_connection();    
}

void ListenerModule::tearDown() {
    server.stop_connection();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ListenerModule::body() {
    vector<string> splitted_package;
    Operation op;
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        array<string, 2> types = {{"none", "none"}};
        array<double, 4> data = {{-1.0, -1.0, -1.0, -1.0}};
        array<string, 8> times = {{"none", "none", "none", "none", "none", "none", "none", "none"}};
        vector<string> aux, d, t;
        string package = server.get_package();
        if(package != ""){            
            cout << "Pacote: \'" << package << endl;
            splitted_package = op.split(package, '/');

            int32_t i = 0;
            int32_t j = 0;
            int32_t k = 0;
            for (auto str : splitted_package) {
                aux = op.split(str, '-');
                types[i] = aux[0];
                i++;
                aux = op.split(aux[1], '$');
                d = op.split(aux[0], '&');

                for (uint32_t x = 0; x < d.size(); x++) {
                    data[x + j] = stod(d[x]);
                }
                
                j = d.size();
                t = op.split(aux[1], ',');

                for (uint32_t x = 0; x < t.size(); x++) {
                    times[x + k] = t[x]; 
                }

                k = t.size();
            }

            SensorData sensor_data(types,data,times);
            Container c(sensor_data);
            getConference().send(c);
            
            /*
             * Para cada execução do loop, contabilizar e enviar duas unidades de bateria consumida
             * */
            ResourceUpdate rUpdate(-2);
            Container rUpdContainer(rUpdate);
            getConference().send(rUpdContainer);
            
            cout << "\' recebido e encaminhado para o processamento." << endl;            
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}