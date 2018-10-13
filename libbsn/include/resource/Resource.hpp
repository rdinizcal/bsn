#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "opendavinci/odcore/serialization/Serializable.h"
#include "opendavinci/odcore/serialization/Serializer.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"

#include <string>

namespace bsn {
    namespace resource {

        class Resource {

            public:
                Resource();
                Resource(std::string id, double capacity, double currentLevel, double unit);
                ~Resource();
                
                void consume(const int /*mult*/);
                void generate(const int /*mult*/);

                std::string const getID();
                double const getCapacity();
                double const getCurrentLevel();
                double const getUnit();

            protected:
                std::string identifier;
                double capacity;
                double currentLevel;
                double unit;



        };
    }  
}

#endif