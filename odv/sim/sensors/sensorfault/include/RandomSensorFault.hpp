#include <chrono>
#include <random>

class RandomSensorFault {

public:
    RandomSensorFault::RandomSensorFault (void) {}
    bool RandomSensorFault::seOcorreuFalha(void) {}
    RandomSensorFault::~RandomSensorFault(void){}

private:
    std::normal_distribution<double> distribuicaoErro;
    std::default_random_engine gerador;
    double const chanceFalha = 0.05;    
    const double z = 1.64;
};