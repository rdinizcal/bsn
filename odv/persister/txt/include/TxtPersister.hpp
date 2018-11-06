#ifndef TXT_PERSISTER_HPP
#define TXT_PERSISTER_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/data/ResourceInfo.h"

#include <string>
#include <vector>
#include <fstream>

class TxtPersister : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
    private:
        TxtPersister(const TxtPersister & /*obj*/);
        TxtPersister &operator=(const TxtPersister & /*obj*/);
        virtual void setUp();
        virtual void tearDown();

    public:
        TxtPersister(const int32_t &argc, char **argv);
        virtual ~TxtPersister();

        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        odcore::base::FIFOQueue data_buffer;
        std::ofstream fp;
};

#endif 