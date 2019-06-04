#include "RandomSensorFault.hpp"

RandomSensorFault::RandomSensorFault() :    
    gerador(seed),
    distribuicaoErro(0.0,1.0)
{}

RandomSensorFault::~RandomSensorFault(void){}


bool RandomSensorFault::seOcorreuFalha(void){
    
    return distribuicaoErro(gerador) >= z;
}

