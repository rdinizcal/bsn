#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>

namespace bsn {
    namespace resource {

        class Resource {

            public:
                Resource(const std::string /*id*/, const double /*capacity*/, const double /*currentLevel*/, const double /*unit*/);
                
                Resource();
                virtual ~Resource();

                Resource(const Resource &/*obj*/);
                Resource& operator=(const Resource &/*obj*/);
                
                void consume(const int /*mult*/);
                void generate(const int /*mult*/);

                void setName(const std::string &/*name*/);
                std::string getName() const;

                void setCapacity(const double &/*capacity*/);
                double getCapacity() const; 

                void setCurrentLevel (const double &/*currentLevel*/);
                double getCurrentLevel() const;

                void setUnit(const double &/*unit*/);
                double getUnit() const;

            private:
                std::string name;
                double capacity;
                double currentLevel;
                double unit;
        };
    }  
}

#endif