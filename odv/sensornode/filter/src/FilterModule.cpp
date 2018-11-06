#include "../include/FilterModule.hpp"

using namespace bsn::filters;
using namespace bsn::data;
using namespace odcore::data;
using namespace odcore::base;
using namespace odcore::base::module;
using namespace std;
using namespace bsn::time;

FilterModule::FilterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "FilterModule"),
    data_buffer() {}

FilterModule::~FilterModule() {}

void FilterModule::setUp() {
    addDataStoreFor(880, data_buffer);
}

void FilterModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode FilterModule::body(){
    MovingAverage filter(5);
    Container container;
    TimeData time_data;
    double data, filtered_data;
    string type;
    // Variaveis de tempo
    string last_time, now_time;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        while (!data_buffer.isEmpty()) {
            // retira o dado da FIFO
            container = data_buffer.leave();            
            data = container.getData<RawData>().getSensorData();
            type = container.getData<RawData>().getSensorType();
            last_time = container.getData<RawData>().getTime();
            now_time = last_time + ',' + time_data.get_time();

            // Filtra o dado
            filter.insert(data, type);
            filtered_data = filter.getValue(type);
            
            cout << "Dado recebido de um " << type << ": " << data << " filtrado para " << filtered_data << endl;
            
            cout << "tempo " << now_time << endl;
            FilteredData encapsulated_data(filtered_data, type, now_time);
            Container packet(encapsulated_data);
            getConference().send(packet);

            /*
             * Para cada execução do loop, contabilizar e enviar uma unidade de bateria consumida
             * */
            ResourceUpdate rUpdate(-1);
            Container rUpdContainer(rUpdate);
            getConference().send(rUpdContainer);
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
