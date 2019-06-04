#include <chrono>
#include <random>
namespace bsn {
    namespace sensorfault{
        class RandomSensorFault {

        public:
            RandomSensorFault (void);
            bool seOcorreuFalha(void) ;
            ~RandomSensorFault(void);

        private:
            std::default_random_engine gerador;
            std::normal_distribution<double> distribuicaoErro;
            const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            double const chanceFalha = 0.05;    
            const double z = 1.64;
        };
    }
}