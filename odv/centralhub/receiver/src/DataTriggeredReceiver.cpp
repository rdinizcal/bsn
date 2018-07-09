#include "DataTriggeredReceiver.hpp"


using namespace std;

// We add some of OpenDaVINCI's namespaces for the sake of readability.
using namespace odcore::base::module;
using namespace odcore::data;
using namespace bsn::data;

DataTriggeredReceiver::DataTriggeredReceiver(const int32_t &argc, char **argv) :
    DataTriggeredConferenceClientModule(argc, argv, "DataTriggeredReceiver")
{}

DataTriggeredReceiver::~DataTriggeredReceiver() {}

void DataTriggeredReceiver::setUp() {
    cout << "This method is called before the component's body is executed." << endl;
}

void DataTriggeredReceiver::tearDown() {
    cout << "This method is called after the program flow returns from the component's body." << endl;
}

void DataTriggeredReceiver::nextContainer(Container &c) {
    cout << "Received container of type " << c.getDataType() <<
                              " sent at " << c.getSentTimeStamp().getYYYYMMDD_HHMMSSms() <<
                          " received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSSms() << endl;

    if (c.getDataType() == TimeStamp::ID()) {
        TimeStamp ts = c.getData<TimeStamp>();
        cout << "Received the following time stamp: " << ts.toString() << endl;
    }

    cout << "Recebi o container contendo: " << c.getData<SensorData>().getSensorStatus() << endl;
}