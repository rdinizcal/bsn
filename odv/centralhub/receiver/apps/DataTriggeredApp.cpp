#include "../include/DataTriggeredReceiver.hpp"

int32_t main(int32_t argc, char **argv) {
    DataTriggeredReceiver dtr(argc, argv);

    return dtr.runModule();
}