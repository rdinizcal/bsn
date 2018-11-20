#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <sstream>
#include <string>
#include <stdint.h>

namespace bsn {
    namespace resource {

        class Resource {

            public:
                Resource(const std::string /*id*/, const double /*capacity*/, const double /*currentLevel*/, const double /*unit*/);
                
                Resource();
                ~Resource();

                Resource(const Resource & /*obj*/);
                Resource &operator=(const Resource & /*obj*/);

                void consume(const int32_t /*mult*/);
                void generate(const int32_t /*mult*/);

                void setName(const std::string &/*name*/);
                std::string getName() const;

                void setCapacity(const double &/*capacity*/);
                double getCapacity() const; 

                void setCurrentLevel (const double &/*currentLevel*/);
                double getCurrentLevel() const;

                void setUnit(const double &/*unit*/);
                double getUnit() const;

                const std::string toString() const;

            private:
                std::string name;
                double capacity;
                double currentLevel;
                double unit;
        };
    }  
}

#endif