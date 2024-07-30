// There should be a “Core” class that loads and execute the program.
// Compose the “Core” class from memory, registers, and instructions.
// This class should provide a function “load()” that loads the instructions
// from a text file into the memory. This class should also provide a function
//  “execute()” that executes the instructions form the memory. Also support
//  handling of invalid instructions.

#include <iostream>
#include <vector>
#include <tuple>
#define REG_COUNT 8
using namespace std;
#include "Register.h"
#include "Accumulator.h"
#include "Instruction.h"
#include "Memory.h"

#ifndef CORE_H
#define CORE_H

class Core
{
    Memory *code_memory;
    Memory data_memory;
    Accumulator accumulator;
    vector<Register> registers;
    Instruction instruction;
    uint8_t prog_size;
    uint8_t mem_size;

public:
    bool halted = false;
    uint16_t programcounter;
    Core() : code_memory(nullptr), data_memory(128), programcounter(0), accumulator(0)
    {
        for (int i = 0; i < REG_COUNT; i++)
        {
            if (i < 4)
                registers.push_back(0x00); // writting 0 to registers[0-3]
            else if (i >= 4 && i < 8)
                registers.push_back(0x0f); // writting 15 to registers[4-7]
        }
    }

    void loadProgram(vector<tuple<uint8_t, uint8_t, uint8_t>> &program)
    {
        code_memory = new Memory(program.size() * 3);
        for (uint16_t i = 0; i < program.size(); i++)
        {

            code_memory->writeMem(i * 3, get<0>(program[i])); // writting the program to memory
            code_memory->writeMem(i * 3 + 1, get<1>(program[i]));
            code_memory->writeMem(i * 3 + 2, get<2>(program[i]));
        }
        prog_size = program.size();
    }

    void executeProgram()
    {

        while (programcounter < code_memory->getSize())

        {

            if (programcounter >= code_memory->getSize()) // checking program counter if goes above defined  memory
            {

                cerr << "Error: Attempting read from invalid memory address" << endl;
                return;
            }
            if (programcounter > (3 * prog_size))
                break;

            uint8_t opcode = code_memory->readMem(programcounter++); // loading the opcode from memory

            uint8_t operand1 = code_memory->readMem(programcounter++); // loading the operand 1 from memory

            uint8_t operand2 = code_memory->readMem(programcounter++); // loading the operand 2 from memory

            instruction.setOpcode(opcode);     // setting the opcode
            instruction.setOperand1(operand1); // setting the operand 1
            instruction.setOperand2(operand2); // setting the operand 2
            execInstruction(instruction.getOpcode(), instruction.getoperand1(), instruction.getoperand2());
        }
    }

    void executeStepByStep()
    {
        string input;
        while (!halted)
        {
            cout << "Press enter to continue of type 'q' to quite" << endl;
            getline(cin, input);
            if (input == "q")
            {
                cout << "Execution Halted." << endl;
                break;
            }
            displayState();
            executeNextInstruction();
        }
    }
    void executeNextInstruction()
    {
        if (programcounter < code_memory->getSize())
        {
            uint8_t opcode = code_memory->readMem(programcounter++);
            uint8_t operand1 = code_memory->readMem(programcounter++);
            uint8_t operand2 = code_memory->readMem(programcounter++);
            instruction.setOpcode(opcode);
            instruction.setOperand1(operand1);
            instruction.setOperand2(operand2);
            execInstruction(instruction.getOpcode(), instruction.getoperand1(), instruction.getoperand2());
        }
        else
        {
            cout << "End of program reachead" << endl;
            halted = true;
        }
    }
    void execInstruction(uint8_t opcode, uint8_t operand1, uint8_t operand2)
    {
        switch (opcode)
        {
        case 0x00: // MOV

            // MOV REG[0-7] REG[0-7]
            if ((operand1 >= 0x00 && operand1 <= 0x07) && (operand2 >= 0x00 && operand2 <= 0x07))
            {
                registers[operand1].set_Data(registers[operand2].get_Data());
            }

            else if (operand1 == 0x08) // Mov ACC
            {

                if (operand2 >= 0x00 && operand2 <= 0x07) // MOV ACC REG[0-7]
                    accumulator.set_Data(registers[operand2].get_Data());
                else
                {
                    operand2 -= 9;

                    accumulator.set_Data(operand2); // MOV ACC #imm
                }
            }
            else if (operand1 >= 0x00 && operand1 <= 0x07) // MOV REG[0-7]
            {
                if (operand2 == 0x08) // MOV REG[0-7] ACC
                {
                    registers[operand1].set_Data(accumulator.get_Data());
                }
                else
                {
                    operand2 -= 9;

                    registers[operand1].set_Data(operand2); // MOV REG[0-7] #imm
                }
            }

            break;

        case 0x01: // ADD
            // ADD REG[0-7] REG[0-7]

            if ((operand1 >= 0x00 && operand1 <= 0x07) && (operand2 >= 0x00 && operand2 <= 0x07))
                registers[operand1].set_Data(registers[operand1].get_Data() + registers[operand2].get_Data());
            else if (operand1 == 0x08)
            {
                if (operand2 >= 0x00 && operand2 <= 0x07) // ADD ACC REG[0-7]
                    accumulator.set_Data(accumulator.get_Data() + registers[operand2].get_Data());
                else
                {
                    operand2 -= 9;
                    accumulator.set_Data(accumulator.get_Data() + operand2); // ADD ACC #imm
                }
            }
            else if (operand1 >= 0x00 && operand1 <= 0x07) // ADD REG[0-7]
            {
                if (operand2 == 0x08) // ADD REG[0-7] ACC
                    registers[operand1].set_Data(registers[operand1].get_Data() + accumulator.get_Data());
                else
                {
                    operand2 -= 9;
                    registers[operand1].set_Data(registers[operand1].get_Data() + operand2); // ADD REG[0-5] #imm
                }
            }
            break;
        case 0x02:
            // SUB REG[0-7] REG[0-7]
            if ((operand1 >= 0x00 && operand1 <= 0x07) && (operand2 >= 0x00 && operand2 <= 0x07))
                registers[operand1].set_Data(registers[operand1].get_Data() - registers[operand2].get_Data());
            else if (operand1 == 0x08)
            {
                if (operand2 >= 0x00 && operand2 <= 0x07) // SUB ACC REG[0-7]
                    accumulator.set_Data(accumulator.get_Data() - registers[operand2].get_Data());
                else
                {
                    operand2 -= 9;

                    accumulator.set_Data((accumulator.get_Data() - operand2)); // SUB ACC #imm
                }
            }
            else if (operand1 >= 0x00 && operand1 <= 0x07) // SUB REG[0-7]
            {
                if (operand2 == 0x08) // SUB REG[0-7] ACC
                    registers[operand1].set_Data(registers[operand1].get_Data() - accumulator.get_Data());
                else
                {
                    operand2 -= 9;
                    registers[operand1].set_Data(registers[operand1].get_Data() - operand2); // SUB REG[0-7] #imm
                }
            }
            break;

        case 0x03:                //	SWAP
            if (operand1 == 0x08) //	SWAP ACC (swapping the lower 0-3 bit with upper 4-7 bit)
            {
                uint8_t lowerbit = accumulator.get_Data() & 0x0F; // only accessing the lower 4 bit, remaining will be zero
                uint8_t upperbit = accumulator.get_Data() & 0xF0; // only accessing the upper 4 bit ,remaining will be zero
                lowerbit = lowerbit << 4;                         // shifting 4 bit to left side
                upperbit = upperbit >> 4;                         // shifting 4 bit to right side
                accumulator.set_Data(lowerbit | upperbit);        // doing or operation will give the full 8 bit value
            }
            else if (operand1 >= 0x00 && operand1 <= 0x07) // SWAP REG[0-7] (swapping the register with upper 4-7 bit with lower 0-3 bit)
            {
                uint8_t lowerbit = registers[operand1].get_Data() & 0x0F;
                uint8_t upperbit = registers[operand1].get_Data() & 0xF0;
                lowerbit = lowerbit << 4;
                upperbit = upperbit >> 4;
                registers[operand1].set_Data(lowerbit | upperbit);
            }

            break;
        case 0x04: // DEC ACC
            accumulator--;
            break;
        case 0x05: // INC ACC
            accumulator++;
            break;
        case 0x06: // ORL

            if (operand2 >= 0x00 && operand2 <= 0x07) // ORL ACC REG[0-7]
                accumulator.set_Data(accumulator.get_Data() | registers[operand2].get_Data());
            else
            {
                operand2 -= 9;
                if (operand1 == 0x08)
                    accumulator.set_Data(accumulator.get_Data() | operand2); // ORL ACC #data
                else
                { // ORL REG[0-7] #data
                    registers[operand1].set_Data(registers[operand1].get_Data() | operand2);
                }
            }
            break;
        case 0x07: // CLR ACC
            accumulator.reset();
            break;
        case 0x08: // SET ACC #bit
            operand2 -= 9;
            accumulator.set_Data(accumulator.get_Data() | (1 << operand2));
            break;
        case 0x09: // RESET REG[0-7]
            registers[operand1].reset();
            break;
        case 0x0A: // NOP
            break;
        case 0x0B: // HALT getting out of the execution
            cout << "Program halted." << endl;
            halted = true;
            return;
        case 0x0C: // STR
            operand2 -= 9;
            // STR RE[0-7] #address
            if (operand1 >= 0 && operand1 <= 0x07)
                data_memory.writeMem(operand2, registers[operand1].get_Data());
            // STR ACC #address
            else if (operand1 == 0x08)
            {
                data_memory.writeMem(operand2, accumulator.get_Data());
            }
            break;
        case 0x0D: // LDR
            operand2 -= 9;

            // LDR REG[0-7] #address
            if (operand1 >= 0 && operand1 <= 0x07)
                registers[operand1].set_Data(data_memory.readMem(operand2));
            else if (operand1 == 0x08)
                accumulator.set_Data(data_memory.readMem(operand2)); // LDR ACC #address
            break;
        case 0x0E:
            if (accumulator.get_Data() > 0)
                programcounter = (operand1) * 3 - 3;

            break;
        case 0x0F:
            if (accumulator.get_Data() == 0)
                programcounter = (operand1) * 3 - 3;

            break;
        }
    }
    void displayState()
    {
        cout << " Program Counter: " << programcounter << endl;
        cout << " Accumulator: " << (int)accumulator.get_Data() << endl;
        cout << " Registers: " << endl;
        for (int i = 0; i < 8; i++)
            cout << "       REG" << i << ": " << (int)registers[i].get_Data() << endl;
    }
    uint8_t getRegisterValue(int index) // getting the data from registers
    {

        try
        {
            if (index < 0 | index > 7)
                throw invalid_argument("Error:Trying to access invalid register ");
            return registers[index].get_Data();
        }
        catch (const invalid_argument &e)
        {
            cerr << e.what() << endl;
        }
    }
    uint8_t getAccumulator() // getting the data from accumulator
    {
        return accumulator.get_Data();
    }
    void getDataMemory()
    {
        int count = 0;
        vector<uint8_t> data = data_memory.getMemory();
        cout << "Conent of Data Memory: " << endl;
        for (int i = 0; i < 16; i++)
        {
            cout<<"         ";
            for (int j = 0; j < 8; j++)
            {
                cout << (int)data[count] << "\t";
                count++;
            }
            cout << endl;
        }
    }
};
#endif
