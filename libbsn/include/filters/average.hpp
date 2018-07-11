#ifndef AVERAGE_HPP
#define AVERAGE_HPP

class average { 
    private:
        double last_val;
        double count;
    public:
        average();
        double get_value();
        void insert(double value);
};

#endif 