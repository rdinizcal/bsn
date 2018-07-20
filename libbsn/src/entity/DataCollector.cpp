#include "bsn/entity/DataCollector.h"

namespace bsn {
    namespace entity { 

        DataCollector::DataCollector() {}

        DataCollector::~DataCollector() {}

        string DataCollector::generateDataByMarkovChain(string actual_status) {
            string category;
            int p = (rand() % 100) + 1;

            if(actual_status == "baixo"){
                if(1 <= p && p <= 5) {
                    category = "alto";
                } else if (5 < p && p <= 30) {
                    category = "moderado";
                } else {
                    category = actual_status;
                }
            } else if(actual_status == "moderado") {
                if(1 <= p && p <= 15) {
                    category = "alto";
                } else if (15 < p && p <= 30) {
                    category = "baixo";
                } else {
                    category = actual_status;
                }
            } else if(actual_status == "alto"){
                if(1 <= p && p <= 5) {
                    category = "baixo";
                } else if (5 < p && p <= 30) {
                    category = "moderado";
                } else {
                    category = actual_status;
                }
            } else {
                category = actual_status;
            }

            return category;
        }

        double DataCollector::generateDataByNormalDist(double mode, double variance) {
            std::default_random_engine generator;
            std::random_device rd;
            double generated_data;

            do{
                generator.seed(rd());
                std::normal_distribution<double> distribution(mode, variance); // mode é o valor esperado e variance é a variação do valor
                generated_data = distribution(generator);
            } while(generated_data < 0 || generated_data > 1024);

            return generated_data;
        }

    }
}