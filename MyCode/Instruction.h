// There should be a “Instruction” class that will provide all the necessary
//  attributes to support the instruction set.

#include <iostream>
using namespace std;

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

class Instruction
{
    uint8_t opcode; 
    uint8_t operand1;
    uint8_t operand2;

public:
    Instruction(){}
    Instruction(uint8_t opc, uint8_t oper1,uint8_t oper2) : opcode(opc), operand1(oper1),operand2(oper2) {}

    uint8_t getOpcode() 
    {
        return opcode;
    }
    uint8_t getoperand1()
    {
        return operand1;
    }
    uint8_t getoperand2()
    {
        return operand2;
    }
    void setOpcode(uint8_t opc){
        opcode=opc;
    }
    void setOperand1(uint8_t oper){
        operand1=oper;
    }
    void setOperand2(uint8_t oper){
        operand2=oper;
    }
};
#endif