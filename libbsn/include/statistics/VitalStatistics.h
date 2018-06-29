#ifndef VITAL_STATISTICS_H
#define VITAL_STATISTICS_H

#include <map>
#include <deque>
#include <iostream>
#include <string>
#include <random>
#include <unistd.h>

using namespace std;

namespace bsn {
    namespace statistics {
        
        // Atualiza o estado atual baseado em sua fila de estados
        string statusAnalysis(string categorized_data, string actual_status, deque<string> &data_queue);

        // Aleatoriamente gera um status
        string generateData(string actual_status);

        // Controla os ciclos dado o estado do paciente
        bool controllerFSM(int t, string status);

        // Cálculo do estado do paciente
        string calculateHealthStatus(map<uint32_t,string> buffer );

        // Gera dados usando a distribuição Normal
        double generateDataByNormalDist(double, double);
    }
}

#endif