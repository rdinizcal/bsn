#include "DataProcessor.hpp"

// ecg termomther oximeter and blood pressure
#define number_sensors 5

using namespace bsn::msg::data;
using namespace bsn::msg::info;
using namespace bsn::range;
using namespace bsn::operation;
using namespace bsn::processor;
using namespace odcore::data;
using namespace odcore::base;
using namespace odcore::base::module;
using namespace bsn::configuration;


DataProcessor::DataProcessor(const int32_t &argc, char **argv) :
TimeTriggeredConferenceClientModule(argc, argv, "DataProcessor"),
	packets_received(number_sensors),
	data_buffer() {}
	
DataProcessor::~DataProcessor() {}

void DataProcessor::setUp() {
    addDataStoreFor(873, data_buffer);

    //initialize packets_received
    for(std::vector<std::list<double>>::iterator it = packets_received.begin();
            it != packets_received.end(); ++it) {
                (*it).push_back(0.0);
    }
}

void DataProcessor::tearDown(){}

void DataProcessor::print_packs(){
	int32_t i = 0;
	for(auto l : packets_received){
		cout << i << ": ";
		i++;
		for(auto el : l){
			cout << el << ' ';
		}
		cout << endl;
	}
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataProcessor::body(){
    Container container;    
    array<string, 2> types;
    array<double, 4> data;
    array<string, 8> times;
    int32_t sensor_id;
    double evaluated_packet;
    double patient_status;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING){

        while(!data_buffer.isEmpty()){
            // Retira o dado da FIFO
            container = data_buffer.leave();
            types = container.getData<SensorData>().getSensorType();
            data = container.getData<SensorData>().getSensorData();
            times = container.getData<SensorData>().getTimes();

            sensor_id = get_sensor_id(types[0]);

            if (types[0] == "bpms" or types[0] == "bpmd") {
         	// O mais discrepante é o que conta(Guideline brasileiro)
         		evaluated_packet = max(data[1],data[3]);
                if (evaluated_packet == data[3])
                    sensor_id = get_sensor_id(types[1]); 
            }
            else {
         	// Para os sensores que não são de pressão
        		evaluated_packet = data[1];
            }

            // Se o pacote for válido...
            if (evaluated_packet != -1) {                
		        packets_received[sensor_id].push_back(evaluated_packet);
		        print_packs();

                patient_status = data_fuse(packets_received);
            }

			/*
             * Para cada execução do loop, contabilizar e enviar duas unidades de bateria consumida
             */
            ResourceUpdate rUpdate(-1);
            Container rUpdContainer(rUpdate);
            getConference().send(rUpdContainer);

            /*
			 * Envia dados para persistencia
			 * */
            std::string sensor_risk_str;
            std::string bpr_risk;
			std::string oxi_risk;
			std::string ecg_risk;
			std::string trm_risk;

            for(int i = 0; i < 4; i++){
                double sensor_risk = packets_received[i].back();

                if(sensor_risk > 0 && sensor_risk <= 20) {
                    sensor_risk_str = "low risk";
                } else if (sensor_risk > 20 && sensor_risk <= 65) {
                    sensor_risk_str = "moderate risk";
                } else if (sensor_risk > 65 && sensor_risk <= 100) {
                    sensor_risk_str = "high risk";
                } else {
                    sensor_risk_str = "unknown";
                }

                if(i==0) {
                    trm_risk = sensor_risk_str;
                } else if (i == 1){
                    ecg_risk = sensor_risk_str;
                } else if (i == 2) {
                    oxi_risk = sensor_risk_str;
                } else {
                    bpr_risk = sensor_risk_str;
                }

            }           

			PatientStatusInfo psInfo(trm_risk, ecg_risk, oxi_risk, bpr_risk, (patient_status>=85)?"CRITICAL STATE":"NORMAL STATE");
			Container psInfoContainer(psInfo);
			getConference().send(psInfoContainer);

			std::cout << "Message sent:" << endl;
			std::cout << "*****************************************" << endl;
			std::cout << psInfo.toString();
			std::cout << "*****************************************" << endl;

        }

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
