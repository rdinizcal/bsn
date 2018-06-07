#include <stdint.h>
#include <iostream>
#include <string>
#include <memory>
#include <string.h>
#include <opendavinci/odcore/base/Lock.h>
#include <opendavinci/odcore/base/Thread.h>
#include <opendavinci/odcore/wrapper/SharedMemory.h>
#include <opendavinci/odcore/wrapper/SharedMemoryFactory.h>

std::string get_package();