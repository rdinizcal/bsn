#include "FilterModule.hpp"

using namespace bsn::filters;
using namespace bsn::msg::data;
using namespace bsn::msg::control;
using namespace odcore::data;
using namespace odcore::base;
using namespace odcore::base::module;
using namespace std;
using namespace bsn::time;

FilterModule::FilterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "FilterModule"),
    buffer(),
    active(true),
    params({{}}) {}

FilterModule::~FilterModule() {}

void FilterModule::setUp() {
    addDataStoreFor(901, buffer);
    addDataStoreFor(880, buffer);
}

void FilterModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode FilterModule::body(){
    MovingAverage filter(5);
    Container container;
    TimeData time_data;
    double data, filteredData;
    string type;
    // Variaveis de tempo
    string lastTime, nowTime;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        while (!buffer.isEmpty()) {
            
            container = buffer.leave();      

            if (container.getDataType() == 901) {

                active = container.getData<FilterModuleControlCommand>().getActive();
                params = container.getData<FilterModuleControlCommand>().getParams();

            } else if (container.getDataType() == 880) {

                if(!active){
                    continue;
                }

                data = container.getData<RawData>().getSensorData();
                type = container.getData<RawData>().getSensorType();
                lastTime = container.getData<RawData>().getTime();
                nowTime = lastTime + ',' + time_data.get_time();

                // Filtra o dado
                filter.insert(data, type);
                filteredData = filter.getValue(type);
                
                cout << "Dado recebido de um " << type << ": " << data << " filtrado para " << filteredData << endl;
                
                cout << "tempo " << nowTime << endl;
                FilteredData encapsulatedData(filteredData, type, nowTime);
                Container packet(encapsulatedData);
                getConference().send(packet);

                /*
                * Para cada execução do loop, contabilizar e enviar uma unidade de bateria consumida
                * */
                ResourceUpdate rUpdate(-1);
                Container rUpdContainer(rUpdate);
                getConference().send(rUpdContainer);
            }
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
