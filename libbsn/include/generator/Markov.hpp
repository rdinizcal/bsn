#include <iostream>
#include <array> 
#include <random>
#include <vector>

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

    Markov(std::array<float,9> transitions,int initial_state);        
    void next_state();
};