// There should be a “Accumulator“ class, that represents Accumulator(ACC) 
// inherited from “Register” class. This class should provide operator 
// overloading for increment and decrement operations.

#include <iostream>
#include "Register.h"
using namespace std;

#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H


class Accumulator:public Register{
    public:
    Accumulator(){}
    Accumulator(uint8_t initial_val):Register(initial_val){}
    
    Accumulator& operator++(int){ //operator overloading for increment operator
            set_Data(get_Data()+1);
            return *this;
    }
    Accumulator& operator--(int){ //operator overloading for decrement operator
        set_Data(get_Data()-1);
        return *this;
    }
};
#endif