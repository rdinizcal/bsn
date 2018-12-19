#include "DataSender.hpp"

int32_t main(int32_t argc, char **argv) {
    DataSender dh(argc, argv);
    
    return dh.runModule();
}