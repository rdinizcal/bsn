#include <stdint.h>

#include "../include/DataHandler.hpp"

int32_t main(int32_t argc, char **argv) {
    DataHandler dh(argc, argv);
    
    return dh.runModule();
}