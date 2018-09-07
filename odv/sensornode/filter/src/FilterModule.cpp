#include "../include/FilterModule.hpp"

using namespace bsn::filters;
using namespace bsn::data;
using namespace odcore::data;
using namespace odcore::base;
using namespace odcore::base::module;
using namespace std;

FilterModule::FilterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "FilterModule"),
    data_buffer() {}

FilterModule::~FilterModule() {}

void FilterModule::setUp() {
    addDataStoreFor(875, data_buffer);
}

void FilterModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode FilterModule::body(){
    MovingAverage filter(5);
    Container container;
    double data, filtered_data;
    string type;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        while (!data_buffer.isEmpty()) {
            // retira o dado da FIFO
            container = data_buffer.leave();
            data = container.getData<ConvertedData>().getSensorData();
            type = container.getData<ConvertedData>().getSensorType();

            // Filtra o dado
            filter.insert(data);
            filtered_data = filter.getValue();

            cout << "Dado recebido de um " << type << ": " << data << " filtrado para " << filtered_data << endl;
            // Encapsula o dado como Filtered para manda-lo pela FIFO
            FilteredData encapsulated_data(filtered_data, type);

            Container packet(encapsulated_data);

            getConference().send(packet);

        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
