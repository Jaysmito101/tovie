#include "toviec.h"
#include "operations.h"
#define LEXPP_IMPLEMENTATION
#include "include/lexpp.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>



#if defined(WIN32) || defined(_WIN32) 
#define PATH_SEPARATOR "\\" 
#else 
#define PATH_SEPARATOR "/" 
#endif

bool is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

bool starts_with(const std::string& s, const std::string& prefix) {
    return s.substr(0, prefix.size()) == prefix;
}

bool ends_with(const std::string& s, const std::string& suffix) {
    return s.substr(s.size() - suffix.size(), suffix.size()) == suffix;
}

static void process_defs(std::string& token, std::unordered_map<std::string, std::string>& defs){
    // iterate over the keys of defs
    for (auto& kv : defs) {
        // if the token starts with the key
        if (token.find(kv.first) != std::string::npos) {
            // replace the key from the string to value
            token.replace(token.find(kv.first), kv.first.size(), kv.second);
            return;
        }
    }
}

static std::string read_lib(const std::string& lib_name, std::vector<std::string>& includePaths, bool* success) {
    for(std::string& includePath : includePaths){
        std::string path = includePath + PATH_SEPARATOR + lib_name;
        std::ifstream file(path);
        if(file.is_open()){
            std::stringstream buffer;
            buffer << file.rdbuf();
            *success = true;
            return buffer.str();
        }
        *success = false;
    }
    return "";
}

std::vector<Operation> parse(std::string& input, std::string& includePath, std::vector<Operation>& ioperations, std::unordered_map<std::string, std::string>& idefs, bool isInclude) {
    std::vector<std::string> includePaths = lexpp::lex(includePath, ";");
    std::vector<Operation> operations;
    std::unordered_map<std::string, std::string> defs;
    std::vector<std::string> tokens = lexpp::lex(input, " \t\r\n");
    if(isInclude){
        defs = idefs;
        operations = ioperations;
    }
    else{
        defs["main"] = "0";
        operations.push_back(Operation(OperationType::BEGIN));
    }
    for (int l=0;l<tokens.size();l++) {
        std::string& token = tokens[l];
        if(token.size() == 0){
            continue;
        }
        // Preprocessor tokens
        if(starts_with(token, "include<\"")){
            while(!ends_with(token, "\">")){
                l++;
                token += " " + tokens[l];
            }
            bool isIncludeOK = false;
            std::string libSrc = read_lib(token.substr(9, token.size()-2), includePaths, &isIncludeOK);
            if(isIncludeOK){
                parse(libSrc, includePath, operations, defs, true);
            }else{
                throw std::runtime_error("include " + token.substr(9, token.size()-2) + " error");
            }
        }
        else if(starts_with(token, "def<")){
            std::string name = token.substr(4);
            token = "";
            while(token[token.size()-1] != '>'){
                token += " " + tokens[++l];
            }
            defs[name] = token.substr(0, token.size()-1);
        }

        process_defs(token, defs);

        // Operation tokens
        if(is_number(token)) {
            try{
                operations.push_back(Operation(OperationType::PUSH, std::stoi(token)));
            }catch(...){
                throw std::runtime_error(token + " - too big integer error!");
            }
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
        else if(token == "&&"){
            operations.push_back(Operation(OperationType::AND));
        }
        else if(token == "||"){
            operations.push_back(Operation(OperationType::OR));
        }
        else if(token == ">"){
            operations.push_back(Operation(OperationType::GT));
        }
        else if(token == "<"){
            operations.push_back(Operation(OperationType::LT));
        }
        else if(token == "=="){
            operations.push_back(Operation(OperationType::EQ));
        }
        else if(token == "<="){
            operations.push_back(Operation(OperationType::LE));
        }
        else if(token == ">="){
            operations.push_back(Operation(OperationType::GE));
        }
        else if(token == "!="){
            operations.push_back(Operation(OperationType::NEQ));
        }
        else if(token == "println"){
            operations.push_back(Operation(OperationType::PRINTLN));
        }
        else if(token == "dup"){
            operations.push_back(Operation(OperationType::DUP));
        }
        else if(token == "swap"){
            operations.push_back(Operation(OperationType::SWAP));
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
        else if(token == "loadlib"){
            operations.push_back(Operation(OperationType::LOADLIB));
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
        else if(token == "proc_end" || token == "end_proc"){
            operations.push_back(Operation(OperationType::PROC, -1));
        }
        else if(starts_with(token, "proc_")){
            operations.push_back(Operation(OperationType::PROC, std::stoi(token.substr(5))));
        }
        else if(token == "call"){
            operations.push_back(Operation(OperationType::CALL));
        }
        else if(token == "ncall"){
            operations.push_back(Operation(OperationType::NCALL));
        }
        else if(token == "malloc"){
            operations.push_back(Operation(OperationType::MALLOC));
        }
        else if(token == "memset"){
            operations.push_back(Operation(OperationType::MEMSET));
        }
        else if(token == "memget"){
            operations.push_back(Operation(OperationType::MEMGET));
        }
        else if(starts_with(token, "memset_")){
            operations.push_back(Operation(OperationType::MEMSET, std::stoi(token.substr(7))));
        }
        else if(starts_with(token, "memget_")){
            operations.push_back(Operation(OperationType::MEMGET, std::stoi(token.substr(7))));
        }
        else if(token == "if"){
            operations.push_back(Operation(OperationType::IF, 0));
        }
        else if(token == "end_if" || token == "if_end"){
            operations.push_back(Operation(OperationType::IF, 1));
        }
        else if(token == "end_for" || token == "for_end"){
            operations.push_back(Operation(OperationType::FOR, -1));
        }
        else if(starts_with(token, "for_")){
            operations.push_back(Operation(OperationType::FOR, std::stoi(token.substr(4))));
        }
        else if(starts_with(token, "for")){
            operations.push_back(Operation(OperationType::FOR, -2));
        }
        else if(starts_with(token, "pop_")){
            operations.push_back(Operation(OperationType::FOR, std::stoi(token.substr(4))));
            operations.push_back(Operation(OperationType::POP));
            operations.push_back(Operation(OperationType::FOR, -1));
        }
        else if(token == "do"){
            operations.push_back(Operation(OperationType::PUSH, true));
            operations.push_back(Operation(OperationType::WHILE, 0));
        }
        else if(token == "while"){
            operations.push_back(Operation(OperationType::WHILE, 0));
        }
        else if(token == "end_while" || token == "while_end" || token == "do_end" || token == "end_do"){
            operations.push_back(Operation(OperationType::WHILE, 1));
        }
	    else if(token == "ret"){
                operations.push_back(Operation(OperationType::RET));	
	    }
        else if(token[0] == '\"'){
            if(token[token.size() - 1] != '\"'){
                while(token[token.size() - 1] != '\"'){
                    token += " " + tokens[++l];
                }
            }
            operations.push_back(Operation(OperationType::PUSH, -1));
            bool isBkslash = false;
            for(int i=1;i<token.size() -1;i++){
                if(token[i] == '\\'){
                    isBkslash = true;
                }
                else{
                    if(isBkslash){
                        isBkslash = false;
                        switch(token[i]){
                            case 'n':
                                token[i] = '\n';
                                break;
                            case 't':
                                token[i] = '\t';
                                break;
                            case 'r':
                                token[i] = '\r';
                                break;
                            case '\\':
                                token[i] = '\\';
                                break;
                            case '\"':
                                token[i] = '\"';
                                break;
                            case '\'':
                                token[i] = '\'';
                                break;
                            case 'f':
                                token[i] = '\f';
                                break;
                            case 'b':
                                token[i] = '\b';
                                break;
                            default:
                                throw std::runtime_error("invalid escape sequence error!");
                        }
                    }
                    operations.push_back(Operation(OperationType::PUSH, (int)token[i]));
                }
            }

        }
    }
    if(!isInclude)
        operations.push_back(Operation(OperationType::END));
    ioperations = operations;
    idefs = defs;
    return operations;
}
