#include "../include/FilterModule.hpp"

using namespace bsn::filters;
using namespace bsn::data;
using namespace odcore::base;
using namespace odcore::base::module;

FilterModule::FilterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "FilterModule"),
    data_buffer() {}

FilterModule::~FilterModule() {}

void FilterModule::setUp() {
    addDataStoreFor(875, data_buffer);
}

void FilterModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode FilterModule::body(){
    Average filter;
    Container container;
    double data, filtered_data;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        while(!data_buffer.isEmpty()){
            // retira o dado da FIFO
            container = data_buffer.leave();
            data = container.getData<ConvertedData>().getSensorData();
            // Filtra o dado
            filter.insert(data);
            filtered_data = filter.get_value();

            std::cout << "Dado recebido: " << data << " filtrado para " << filtered_data << std::endl;
            // Encapsula o dado como Filtered para manda-lo pela FIFO
            FilteredData encapsulated_data(filtered_data);

            Container packet(encapsulated_data);

            getConference().send(packet);

        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
