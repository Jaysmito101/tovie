#include "toviec.h"
#include "operations.h"
#define LEXPP_IMPLEMENTATION
#include "include/lexpp.hpp"
#include <string>
#include <vector>

bool is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

std::vector<Operation> parse(std::string& input) {
    std::vector<Operation> operations;
    std::vector<std::string> tokens = lexpp::lex(input, " \n");
    operations.push_back(Operation(OperationType::BEGIN));
    for (std::string& token : tokens) {
        if(is_number(token)) {
            operations.push_back(Operation(OperationType::PUSH, std::stoi(token)));
        }
        else if(token == "true"){
            operations.push_back(Operation(OperationType::PUSH, true));
        }
        else if(token == "false"){
            operations.push_back(Operation(OperationType::PUSH, false));
        }
        else if(token == "+"){
            operations.push_back(Operation(OperationType::ADD));
        }
        else if(token == "-"){
            operations.push_back(Operation(OperationType::SUB));
        }
        else if(token == "*"){
            operations.push_back(Operation(OperationType::MUL));
        }
        else if(token == "/"){
            operations.push_back(Operation(OperationType::DIV));
        }
        else if(token == ">"){
            operations.push_back(Operation(OperationType::GT));
        }
        else if(token == "<"){
            operations.push_back(Operation(OperationType::LT));
        }
        else if(token == "="){
            if(operations.back().op == OperationType::GT){
                operations.back().op = OperationType::GE;
            }
            else if(operations.back().op == OperationType::LT){
                operations.back().op = OperationType::LE;
            }
            else{
                operations.push_back(Operation(OperationType::EQ));
            }
        }
        else if(token == "<="){
            operations.push_back(Operation(OperationType::LE));
        }
        else if(token == ">="){
            operations.push_back(Operation(OperationType::GE));
        }
        else if(token == "println"){
            operations.push_back(Operation(OperationType::PRINTLN));
        }
        else if(token == "printlns"){
            operations.push_back(Operation(OperationType::PRINTLNS));
        }
        else if(token == "printlnns"){
            operations.push_back(Operation(OperationType::PRINTLNNS));
        }
        else if(token == "printlnn"){
            operations.push_back(Operation(OperationType::PRINTLNN));
        }
        else if(token == "print"){
            operations.push_back(Operation(OperationType::PRINT));
        }
        else if(token == "prints"){
            operations.push_back(Operation(OperationType::PRINTS));
        }
        else if(token == "printns"){
            operations.push_back(Operation(OperationType::PRINTNS));
        }
        else if(token == "printn"){
            operations.push_back(Operation(OperationType::PRINTN));
        }
        else if(token == "dump"){
            operations.push_back(Operation(OperationType::DUMP));
        }
        else if(token == "dumps"){
            operations.push_back(Operation(OperationType::DUMPS));
        }
        else if(token == "dumpns"){
            operations.push_back(Operation(OperationType::DUMPNS));
        }
        else if(token == "dumpn"){
            operations.push_back(Operation(OperationType::DUMPN));
        }
        else if(token == "receed"){
            operations.push_back(Operation(OperationType::RECEED));
        }
        else if(token == "proceed"){
            operations.push_back(Operation(OperationType::PROCEED));
        }
        else if(token == "puts"){
            operations.push_back(Operation(OperationType::PUTS));
        }
        else if(token == "putsln"){
            operations.push_back(Operation(OperationType::PUTSLN));
        }
        else if(token == "inputi"){
            operations.push_back(Operation(OperationType::INPUTI));
        }
        else if(token == "inputs"){
            operations.push_back(Operation(OperationType::INPUTS));
        }
        else if(token == "pop"){
            operations.push_back(Operation(OperationType::POP));
        }
        else if(token == "^"){
            operations.push_back(Operation(OperationType::POW));
        }
        else if(token == "%"){
            operations.push_back(Operation(OperationType::MOD));
        }
        else if(token == "exit"){
            operations.push_back(Operation(OperationType::EXIT));
        }
        else if(token[0] == '\"' && token[token.size() - 1] == '\"'){
            for(int i=1;i<token.size() -1;i++)
                operations.push_back(Operation(OperationType::PUSH, (int)token[i]));
        }
    }
    operations.push_back(Operation(OperationType::END));
    return operations;
}
