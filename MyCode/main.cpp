#include <iostream>
#include <vector>
#include <fstream>
#include <tuple>
#include "Core.h"
#include "conversion.h"
using namespace std;

int main()
{
    Core core;
    string value;
    ifstream my_file("file.txt");
    if (!my_file.is_open())
    {
        cerr << "Error: File is not opened" << endl;
        return 1;
    }
    string line;
    vector<tuple<uint8_t, uint8_t, uint8_t>> program;
    map<string,int>labels;
    int line_number=1;
    while (getline(my_file, line))
    {
        string operation, reg1, reg2;
        uint8_t opcode, operand1, operand2;
        // adding the 3 values into operation and operand 1 and operand 2
        tie(operation, reg1, reg2) = sepInput(line,labels,line_number);
        // adding the all the values from mapped values of opecode and operand 1 and operand 2
        if(operation.empty() && reg1.empty() && reg2.empty()){
            line_number++;
            continue;
                    }
        tie(opcode, operand1, operand2) = convertto(operation, reg1, reg2,labels);
        program.push_back({opcode, operand1, operand2});
        line_number++;
    }
    core.loadProgram(program); // LOADING THE PROGRAM

    cout<<"Press Y for step by step execution else Press N"<<endl;
    cin>>value;
    if(value=="Y")
        core.executeStepByStep();
    else{
        core.executeProgram();
        core.displayState();
    }
   if(core.halted)
        core.getDataMemory();
    return 0;
}
