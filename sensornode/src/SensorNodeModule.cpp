#include "SensorNodeModule.h"

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/odcore/base/Thread.h"

#include <time.h>
#include <iostream>

using namespace std;

using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data;
using namespace odcore::data::dmcp;

using namespace openbasn::data;
using namespace openbasn::model::sensor;


SensorNodeModule::SensorNodeModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "sensornode"),
    m_id(getIdentifier()),
    m_sensor(),
    m_status("low"),
    m_data_queue(){}

    SensorNodeModule::~SensorNodeModule() {}

void SensorNodeModule::setUp() {
    SensorNodeModule::getSensorConfiguration();
}

void SensorNodeModule::tearDown() {}

void SensorNodeModule::getSensorConfiguration(){
    int32_t sensortypes = getKeyValueConfiguration().getValue<int32_t>("global.sensortypes");
    
    for(int32_t i = 0; i < sensortypes; i++) {
        string sensor_type;
        int32_t sensor_id = i+1;

        sensor_type = getKeyValueConfiguration().getValue<string>("global.sensortype."+ to_string(sensor_id));

        if((getKeyValueConfiguration().getValue<bool>("sensornode."+ sensor_type +".active") == 1)){
            bool   active = (getKeyValueConfiguration().getValue<bool>("sensornode."+ sensor_type +".active") == 1);
            float  samplerate = getKeyValueConfiguration().getValue<float>("sensornode."+ sensor_type +".samplerate");
            string mean_behavior = getKeyValueConfiguration().getValue<string>("sensornode."+ sensor_type +".mean.behavior");
            double stddev = getKeyValueConfiguration().getValue<double>("sensornode."+ sensor_type +".stddev");
            double mean = getKeyValueConfiguration().getValue<double>("sensornode."+ sensor_type +".mean."+mean_behavior);

            m_sensor = Sensor(sensor_id, samplerate, active, mean, stddev);
        }
    }
}

string SensorNodeModule::generateData(string actual_status){

    string category;
    int p = (rand() % 100) + 1;

    if(actual_status == "low"){
        if(1 <= p && p <= 5) {
            category = "high";
        } else if (5 < p && p <= 30) {
            category = "moderate";
        } else {
            category = actual_status;
        }
    } else if(actual_status == "moderate") {
        if(1 <= p && p <= 15) {
            category = "high";
        } else if (15 < p && p <= 30) {
            category = "low";
        } else {
            category = actual_status;
        }
    } else if(actual_status == "high"){
        if(1 <= p && p <= 5) {
            category = "low";
        } else if (5 < p && p <= 30) {
            category = "moderate";
        } else {
            category = actual_status;
        }
    } else {
        category = actual_status;
    }

    return category;
}

string SensorNodeModule::statusAnalysis(string categorized_data, string actual_status) {

    string new_status;
    int l=0, m=0, h=0;

    if(m_data_queue.size()>=5){
        m_data_queue.pop_front();
    }

    m_data_queue.push_back(categorized_data);

    for(uint32_t i = 0; i < m_data_queue.size(); i++) {
        if(m_data_queue[i]=="low"){
            l++;
        } else if(m_data_queue[i]=="moderate") {
            m++;
        } else if (m_data_queue[i]=="high"){
            h++;
        }
    }

    if(l>=3){
        new_status = "low";
    } else if (m>=3) {
        new_status = "moderate";
    } else if (h>=3) {
        new_status = "high";
    } else {
        new_status = actual_status;
    }

    return new_status;
}

void SensorNodeModule::sendSensorData(SensorData sensordata){
    Container container(sensordata);
    getConference().send(container);
    CLOG1 << sensordata.toString() << " sent at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorNodeModule::body() {

    srand(time(NULL));
    string categorized_data, new_m_status;
    int counter = 0, freq = 10;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        counter++;

        if(counter==freq){
            counter=0;

            /* 
             * MONITOR
             * Sample Data - obter novo dado do sensor
             * Process Data - processar dado do sensor
             * Categorize Data - categorizar dado do sensor de acordo com conhecimento de domínio
             */
            categorized_data = generateData(m_status);
            cout << "Actual status: " << m_status << " | Data sampled: " << categorized_data << " at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;

            /* 
             * ANALYZE
             * Analyze sensornode status 
             */
            new_m_status = statusAnalysis(categorized_data, m_status);

            //
            /* 
             * PLAN AND EXECUTE
             * alterar a frequencia de sampling e enviar estado atual
             */
            if(m_status != new_m_status) {
                m_status = new_m_status;

                if(m_status=="low"){
                    freq = 10;
                } else if (m_status=="moderate"){
                    freq = 5;
                } else if (m_status=="high"){
                    freq = 1;
                }
                
                SensorNodeModule::sendSensorData(SensorData(m_id, m_sensor.getType(), 0, m_status));
            }
        }
        
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}