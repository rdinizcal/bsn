#include "../include/DataProcessor.hpp"

int32_t main(int32_t argc, char **argv) {
    DataProcessor dtr(argc, argv);

    return dtr.runModule();
}