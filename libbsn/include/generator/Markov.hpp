#include <iostream>
#include <array> 
#include <random>
#include <vector>

#include "bsn/range/Range.hpp"

/*  OBS.: Para simplificação
    O vetor de transições contém as probabilidades de tal forma:
        
*/

class Markov {
    public:

    // 0 para low 1 para medium e 2 para high
    int current_state;
    // Contém a probabilidade de todas as transições
    std::array<float,25> transitions;
    // Contém os intervalos de cada estado
    std::array<bsn::range::Range, 5> ranges;

    Markov(std::array<float, 25> transitions, std::array<bsn::range::Range, 5> ranges, int initial_state);
    // Calcula o próximo estado da cadeia de markov
    void next_state();
    // Calcula um valor baseado no intervalo do estado atual
    double calculate_state();
};