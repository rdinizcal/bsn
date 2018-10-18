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
                Resource(std::string /*id*/, double /*capacity*/, double /*currentLevel*/, double /*unit*/);
                ~Resource();
                
                void consume(const int /*mult*/);
                void generate(const int /*mult*/);

                void setName(const std::string /*name*/);
                std::string getName() const;

                void setCapacity(const double /*capacity*/);
                double getCapacity() const; 

                void setCurrentLevel (const double /*currentLevel*/);
                double getCurrentLevel() const;

                void setUnit(const double /*unit*/);
                double getUnit() const;

            protected:
                std::string name;
                double capacity;
                double currentLevel;
                double unit;
        };
    }  
}

#endif