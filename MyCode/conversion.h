#include <map>
#include <sstream>
#include <cstring>

using namespace std;

tuple<string, string, string> sepInput(const string &input,map<string,int>&labels,int line_number)
{
    
    if (input == "NOP"|input=="HALT") 
    {
        istringstream iss(input);
        string operation, discard;
        iss >> operation; //loading the string from variable to operation
        string reg1 = "0x00"; //since HALT and NOP has no operand1 and operand 2 so loading 
        string reg2 = "0x00"; // it with 0
        return make_tuple(operation, reg1, reg2);
    }

    else if (input.size() < 9 | input.size() == 10) //for CLR,SET,RESET 
    {
        istringstream iss(input);
        string operation, reg1, discard;
        iss >> operation >> reg1;
        string reg2 = "0x00"; //there is no operand1 we are loading it with zero
        if(operation.back()==':'){
            string label=operation.substr(0,operation.size()-1);
           labels[label]=line_number;
           return make_tuple("","","");
        }
        return make_tuple(operation, reg1, reg2);
    }
    else
    {
        istringstream iss(input);
        string operation, reg1, reg2, discard;
        iss >> operation >> reg1 >> reg2;
        return make_tuple(operation, reg1, reg2);
    }
}
tuple<uint8_t, uint8_t, uint8_t> convertto(const string &opcode, const string &reg1, const string &reg2,const map<string,int>labels)
{

    map<string, uint8_t> mapOpcode = {
        //mapping the opcode 
        {"MOV", 0x00},
        {"ADD", 0x01},
        {"SUB", 0x02},
        {"SWAP", 0x03},
        {"DEC", 0x04},
        {"INC", 0x05},
        {"ORL", 0x06},
        {"CLR", 0x07},
        {"SET", 0x08},
        {"RESET", 0x09},
        {"NOP", 0x0A},
        {"HALT", 0x0B},
        {"STR", 0x0C},
        {"LDR", 0x0D},
        {"JNZ", 0x0E},
        {"JZ", 0x0F},
        };
    map<string, uint8_t> mapOperand = {
        //mapping the operand1 and operand2
        {"REG0", 0x00},
        {"REG1", 0x01},
        {"REG2", 0x02},
        {"REG3", 0x03},
        {"REG4", 0x04},
        {"REG5", 0x05},
        {"REG6", 0x06},
        {"REG7", 0x07},
        {"ACC", 0x08}};
    uint8_t new_op;
try
{
    

if(opcode=="JNZ" || opcode=="JZ"){
    uint8_t label_address=labels.at(reg1);
    return {mapOpcode.at(opcode),label_address,0x00};
}
    //mapping when there is only opcode
   else if (reg1.substr(0, 2) == "0x" && reg2.substr(0, 2) == "0x")
        return {mapOpcode.at(opcode), 0x00, 0x00};


    //mapping when there is only opcode and operand1
    else if (reg2.substr(0, 2) == "0x" | reg2.substr(0, 2) == "0X") 
    {
        new_op = stoi(reg2, nullptr, 16);
        return {mapOpcode.at(opcode), mapOperand.at(reg1), new_op+9};
    }
    else //mapping for one opcode and 2 operand
        return {mapOpcode.at(opcode), mapOperand.at(reg1), mapOperand.at(reg2)};

}
catch(const out_of_range& e)
{
    cerr <<"Error:unkown operand or opcode entered"<<endl;
    cout<<opcode<<endl;
    cout<<reg1<<endl;
    cout<<reg2<<endl;
    
}
   
}