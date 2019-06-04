#include "RandomSensorFault.hpp"

RandomSensorFault::RandomSensorFault(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gerador(seed);
    std::normal_distribution<double> distribuicaoErro(0.0,1.0);
}

RandomSensorFault::~RandomSensorFault(void){}


bool RandomSensorFault::seOcorreuFalha(void){
    
    double tentativa;
    tentativa << distribuicaoErro(gerador);
    return tentativa <= z;
}

