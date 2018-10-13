#include "../include/ResourceMonitor.hpp"

int32_t main(int32_t argc, char **argv) {
    ResourceMonitor rm(argc, argv);

    return rm.runModule();
}