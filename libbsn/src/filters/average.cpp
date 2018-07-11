#include "bsn/filters/average.hpp"
#include <iostream>

average::average(){
    last_val = 0.0;
    count    = 0.0;
}
double average::get_value(){
    return last_val;
}
void average::insert(double value) {   
    double result = (value - last_val)/(count+1);
    last_val += result;
    count++;    
}