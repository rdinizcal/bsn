#include <iostream>
#include <array> 
#include <random>
#include <vector>

#include "bsn/range/Range.hpp"

/*  OBS.: Para simplificação
    O vetor de transições contém as probabilidades de tal forma:
        0 = low->low, 1 = low->medium, 2 = low->high
        3 = medium->low, 4 = medium->medium, 5 = medium->high
        6 = high->low, 7 = high->medium, 8 = high->high
*/

class Markov {
    public:

    // 0 para low 1 para medium e 2 para high
    int current_state;
    // Contém a probabilidade de todas as transições
    std::array<float,9> transitions;
    // Contém os intervalos de cada estado
    std::array<bsn::range::Range, 3> ranges;

    Markov(std::array<float, 9> transitions, std::array<bsn::range::Range, 3> ranges, int initial_state);
    // Calcula o próximo estado da cadeia de markov
    void next_state();
    // Calcula um valor baseado no intervalo do estado atual
    double calculate_state();
};