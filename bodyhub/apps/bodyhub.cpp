#include <stdint.h>

#include "BodyHubModule.h"

int32_t main(int32_t argc, char **argv) {
    BodyHubModule bhmodule(argc, argv);
    
    return bhmodule.runModule();
}