#include "bsn/generator/Markov.hpp"

using namespace std;

// Gerador de seeds
random_device rd;
// Seed para os numeros aleatorios
mt19937 seed(rd());
// Gerador de inteiros aleatorios
uniform_int_distribution<int> probability_generator(1,101);

// Construtor
Markov::Markov(array<float,9> transitions,int initial_state){
        this->transitions = transitions;
        this->current_state = initial_state;
}

void Markov::next_state() {
    for(auto i = 0; i < 15; i++) {
        int random_number = probability_generator(seed);    
        // Calcula o offset do vetor baseado no estado
        int offset = current_state * 3;

        //cout << "(" << transitions[offset] << " - " << (transitions[offset + 1] + transitions[offset]) << ")\n";
        cout << current_state << " => ";

        // Probabilidade de ir pro low
        if(random_number < transitions[offset]) {
            current_state = 0;
        }
        // Probabilidade de ir pro medium
        else if(random_number < (transitions[offset + 1] + transitions[offset])) {
            current_state = 1;
        }
        // Probabilidade de ir pro high
        else {
            current_state = 2;
        }
        cout << current_state << endl;        
    }
}
