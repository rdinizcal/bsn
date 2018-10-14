#include "../include/DataSender.hpp"

using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data;
using namespace bsn::data;
using namespace bsn::time;
using namespace std;

DataSender::DataSender(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "datasender"),
    m_buffer() {}

DataSender::~DataSender() {}

TCPSend sender(8000);

void DataSender::setUp() {
    // Recebe FilteredData
    ip = getKeyValueConfiguration().getValue<std::string>("datasender.ip");
    addDataStoreFor(876, m_buffer);
    sender.set_port(getIdentifier());
    sender.setIP(ip);
    sender.connect();
}

void DataSender::tearDown(){    
    sender.disconnect();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataSender::body(){    
    // Retira o id a partir da porta que lhe foi dada
    int id = getIdentifier() - 8080;
    string type;
    double data;
    array<timespec, 3> back_time;
    array<timespec, 4> ts_v;
    timespec now_time;
    bool isOK = false;
    string package;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) { 

        while (!m_buffer.isEmpty()) {
            clock_gettime(CLOCK_REALTIME, &now_time);

            Container container = m_buffer.leave();
            data = container.getData<FilteredData>().getSensorData();
            type = container.getData<FilteredData>().getSensorType();
            back_time = container.getData<FilteredData>().getTimespec();

            std::cout << "Dado recebido de um " << type << ": " << data << std::endl;

            ts_v[0] = back_time[0];
            ts_v[1] = back_time[1];
            ts_v[2] = now_time;

            if (type == "bpms") {
                isOK = true;
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
                sender.send(package);
                cout << package << endl;
            }
            // Para os outros tipos apenas concatenar com '-'
            else if (type != "bmps" and type != "bpmd") {
                isOK = false;
                package = type;
                package += '-';
                package += to_string(data);
                sender.send(package);
            }

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
