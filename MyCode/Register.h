// There should be a “Register” class, that represents a basic register.
//  It should have proper getters and setters for  all the register attributes.
//   It should also provide operator overloading of assignment operator.

#include <iostream>
using namespace std;

#ifndef REGISTER_H

#define REGISTER_H

class Register
{
    uint8_t data;
    uint8_t initial_val;

public:
    Register() {}
    Register(uint8_t initial_val) : data(initial_val), initial_val(initial_val) {}
    uint8_t get_Data()
    {
        return data;
    }
    void set_Data(uint8_t value) //setting the value to register  
    {
        data = value;
    }

    Register &operator=(uint8_t const &val) // operator overloading for assignmment operator
    {
        data = val;
        return *this;
    }
    void reset() //resetting the value of register to default value
    {
        data = initial_val;
    }
};
#endif // REGISTER_H
