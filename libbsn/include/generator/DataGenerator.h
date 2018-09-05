#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <string>
#include <random>
#include <unistd.h>

namespace bsn {
    namespace generator {

        class DataGenerator {

            public:
                DataGenerator();
                ~DataGenerator();

                // Aleatoriamente gera um status
                std::string generateDataByMarkovChain(std::string /*actual status*/);

                // Gera dados usando a distribuição Normal
                double generateDataByNormalDist(double /*variance*/, double /*mode*/);

            private:

        };

    }
}

#endif