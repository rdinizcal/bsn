#ifndef AVERAGE_HPP
#define AVERAGE_HPP

namespace bsn {
    namespace filters {

        class Average { 
        private:
            double last_val;
            double count;
        public:
            Average();
            double get_value();
            void insert(double value);
        };        

    }
}
#endif 