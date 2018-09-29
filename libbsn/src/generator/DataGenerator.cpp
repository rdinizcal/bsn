#include "bsn/generator/DataGenerator.h"

using namespace std;

namespace bsn {
    namespace generator { 

        DataGenerator::DataGenerator() {}

        DataGenerator::~DataGenerator() {}

        double DataGenerator::generateDataByNormalDist(double mode, double variance) {
            std::default_random_engine generator;
            std::random_device rd;
            double generated_data;

            do {
                generator.seed(rd());
                std::normal_distribution<double> distribution(mode, variance); // mode é o valor esperado e variance é a variação do valor
                generated_data = distribution(generator);
            } while(generated_data < 0 || generated_data > 1024);

            return generated_data;
        }

    }
}