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
    double data, filtered_data;
    string type;
    timespec now_time;
    timespec back_time;
    array<timespec, 3> ts_v;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        while (!data_buffer.isEmpty()) {
            clock_gettime(CLOCK_REALTIME, &now_time);

            // retira o dado da FIFO
            container = data_buffer.leave();
            data = container.getData<RawData>().getSensorData();
            type = container.getData<RawData>().getSensorType();
            back_time = container.getData<RawData>().getTimespec();

            // Filtra o dado
            filter.insert(data, type);
            filtered_data = filter.getValue(type);

            cout << "Dado recebido de um " << type << ": " << data << " filtrado para " << filtered_data << endl;
            // Encapsula o dado como Filtered para manda-lo pela FIFO

            ts_v[0] = back_time;
            ts_v[1] = now_time;

            FilteredData encapsulated_data(filtered_data, type, ts_v);
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
