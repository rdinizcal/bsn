#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>

namespace bsn {
    namespace resource {

        class Resource {

            public:
                Resource();
                Resource(std::string /*identifier*/, double /*capacity*/, double /*currentLevel*/, double /*unit*/);
                ~Resource();
                
                void consume(const int /*mult*/);
                void generate(const int /*mult*/);

            protected:
                std::string identifier;
                double capacity;
                double currentLevel;
                double unit;

        };
    }  
}

#endif