// Implement “Memory” class. This class should had function like “writeMem()”
// and “readMem()” to write and read the memory. You are allowed to create you
// own memory map as required. Also provide debug interface for direct access
// of the memory. You can also define opcode for all the instructions if
// required.
#include <iostream>
#include <vector>
using namespace std;

#ifndef MEMORY_H
#define MEMORY_H
class Memory
{
    vector<uint8_t> data;

public:
    Memory() {} // default constructor
    Memory(uint16_t size) : data(size, 0) {}
    void writeMem(uint16_t address, uint8_t value)
    {
        try
        {
            if (address > data.size())
                throw invalid_argument("Error:Attempting to read from invalid memory address");
            // appending data to memory
            data[address] = value;
        }
        catch (const invalid_argument &e)
        {
            cerr << e.what() << '\n';
        }
    }
    uint8_t readMem(uint16_t address)
    {

        try
        {
            if (address > data.size())
                throw invalid_argument("Error:Attempting to read from invalid memory address");

            return data[address];
        }
        catch (const invalid_argument &e)
        {
            cerr << e.what() << '\n';
        }
    }
    // direct memory access
    vector<uint8_t> &getMemory()
    {
        return data;
    }
    uint16_t getSize()
    { // getting the size of the data entered
        return data.size();
    }
};
#endif