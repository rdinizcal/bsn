#include "bsn/generator/Markov.hpp"

using namespace std;

// Gerador de seeds
random_device rd;
// Seed para os numeros aleatorios
mt19937 seed(rd());
// Gerador de inteiros aleatorios
uniform_int_distribution<int> probability_generator(1,101);

// Construtor
Markov::Markov(array<float,9> transitions,array<range,3> ranges,int initial_state){
        this->transitions = transitions;
        this->current_state = initial_state;
        this->ranges = ranges;
}

// Calcula o próximo estado da cadeia de markov
void Markov::next_state() {    
    int random_number = probability_generator(seed);
    // Calcula o offset do vetor baseado no estado
    int offset = current_state * 3;

    // Probabilidade de ir pro low
    if(random_number <= transitions[offset]) {
        current_state = 0;
    }
    // Probabilidade de ir pro medium
    else if(random_number <= (transitions[offset + 1] + transitions[offset])) {
        current_state = 1;
    }
    // Probabilidade de ir pro high
    else {
        current_state = 2;
    }
}

// Calcula um valor baseado no intervalo do estado atual
double Markov::calculate_state() {
    range this_range = ranges[current_state];
    // Cria um número aleatório baseado no range
    std::uniform_real_distribution<double> value_generator(this_range.lower_bound, this_range.upper_bound);
    double val = value_generator(seed);
    return val;
}
