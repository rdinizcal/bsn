#include "bsn/generator/Markov.hpp"

using namespace std;
using namespace bsn::range;

// Gerador de seeds
random_device rd;
// Seed para os numeros aleatorios
mt19937 seed(rd());
// Gerador de inteiros aleatorios
uniform_int_distribution<int> probability_generator(1,100);

// Construtor
Markov::Markov(array<float,25> transitions, array<Range, 5> ranges, int initial_state){
        this->transitions = transitions;
        this->current_state = initial_state;
        this->ranges = ranges;
}

// Calcula o próximo estado da cadeia de markov
void Markov::next_state() {
    int random_number = probability_generator(seed);
    // Calcula o offset do vetor baseado no estado
    int offset = current_state * 5;

    // possibilities corresponde à chance da cadeia cair em um determinado estado
    array<float, 5> possibilities {0,0,0,0,0};
    
    // Inicializando possibilities
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j <= i; j++) {
            possibilities[i] += transitions[offset + j];     
        }
    }
    
    if(random_number <= possibilities[0]) {
        current_state = 0;                        
    }    
    else if(random_number <= possibilities[1]) {
        current_state = 1;             
    }    
    else if(random_number <= possibilities[2]) {
        current_state = 2;
    }    
    else if(random_number <= possibilities[3]) {
        current_state = 3;        
    }    
    else if(random_number <= possibilities[4]) {
        current_state = 4;
    }
}

// Calcula um valor baseado no intervalo do estado atual
double Markov::calculate_state() {
    Range this_range = ranges[current_state];
    // Cria um número aleatório baseado no range
    std::uniform_real_distribution<double> value_generator(this_range.getLowerBound(), this_range.getUpperBound());
    double val = value_generator(seed);
    return val;
}
