#include "operations.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

std::string to_string(OperationType type){
    switch(type){
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case MUL:
            return "MUL";
        case DIV:
            return "DIV";
        case MOD:
            return "MOD";
        case POW:
            return "POW";
        case AND:
            return "AND";
        case OR:
            return "OR";
        case NEQ:
            return "NEQ";
        case EQ:
            return "EQ";
        case GE:
            return "GE";
        case LE:
            return "LE";
        case GT:
            return "GT";
        case LT:
            return "LT";
        case PUSH:
            return "PUSH";
        case POP:
            return "POP";
        case PRINTLN:
            return "PRINTLN";
        case PRINTLNN:
            return "PRINTLNN";
        case PRINTLNS:
            return "PRINTLNS";
        case PRINTLNNS:
            return "PRINTLNNS";
        case PRINT:
            return "PRINT";
        case PRINTN:
            return "PRINTN";
        case PRINTS:
            return "PRINTS";
        case PRINTNS:
            return "PRINTNS";
        case DUMP:
            return "DUMP";
        case DUMPS:
            return "DUMPS";
        case DUMPN:
            return "DUMPN";
        case DUMPNS:
            return "DUMPNS";
        case PUTS:
            return "PUTS";
        case PUTSLN:
            return "PUTSLN";
        case RECEED:
            return "RECEED";
        case PROCEED:
            return "PROCEED";
        case DUP:
            return "DUP";
        case SWAP:
            return "SWAP";
        case INPUTI:
            return "INPUTI";
        case INPUTS:
            return "INPUTS";
        case BEGIN:
            return "BEGIN";
        case END:
            return "END";
        case EXIT:
            return "EXIT";
        case PROC:
            return "PROC";
        case CALL:
            return "CALL";
        case NCALL:
            return "NCALL";
        case MALLOC:
            return "MALLOC";
        case MEMSET:
            return "MEMSET";
        case MEMGET:
            return "MEMGET";
        case RET:
            return "RET";
        case IF:
            return "IF";
        case WHILE:
            return "WHILE";
        case FOR:
            return "FOR";
        case LOADLIB:
            return "LOADLIB";
        case DECL:
            return "DECL";
        case VAR:
            return "VAR";
        default:
            return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& os, OperationType type)
{
    os << to_string(type);
    return os;
}

std::string to_string(DataType type)
{
    switch(type){
        case I8:
            return "I8";
        case I16:
            return "I16";
        case I32:
            return "I32";
        case I64:
            return "I64";
        case U8:
            return "U8";
        case U16:
            return "U16";
        case U32:
            return "U32";
        case U64:
            return "U64";
        case F32:
            return "F32";
        case F64:
            return "F64";
        case STR:
            return "STR";
        case BOOL:
            return "BOOL";
        case DATA_TYPE_COUNT:
            return "DATA_TYPE_COUNT";
        default:
            return "UNKNOWN";
    }
}

static std::string to_upper(std::string& s){
    std::string result;
    for(char c : s){
        result += toupper(c);
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, DataType type)
{
    os << to_string(type);
    return os;
}

DataType to_data_type(std::string token){
    token = to_upper(token);
    if(token == "I8"){
        return I8;
    }
    if(token == "I16"){
        return I16;
    }
    if(token == "I32"){
        return I32;
    }
    if(token == "I64"){
        return I64;
    }
    if(token == "U8"){
        return U8;
    }
    if(token == "U16"){
        return U16;
    }
    if(token == "U32"){
        return U32;
    }
    if(token == "U64"){
        return U64;
    }
    if(token == "F32"){
        return F32;
    }
    if(token == "F64"){
        return F64;
    }
    if(token == "STR"){
        return STR;
    }
    if(token == "BOOL"){
        return BOOL;
    }
    return UNKNOWN;
}

DataType to_data_type(int type)
{
    std::cout << "to_data_type(" << type << ")" << std::endl;
    switch(type){
        case 1:
            return I8;
        case 2:
            return I16;
        case 3:
            return I32;
        case 4:
            return I64;
        case 5:
            return U8;
        case 6:
            return U16;
        case 7:
            return U32;
        case 8:
            return U64;
        case 9:
            return F32;
        case 10:
            return F64;
        case 11:
            return STR;
        case 12:
            return BOOL;
        default:
            return UNKNOWN;
    }
}

static int max_str_size_i = 4096;

void* allocate_data_type(DataType type, int max_str_size){
    max_str_size_i = max_str_size;
    switch(type){
        case I8:
            return new char;
        case I16:
            return new short;
        case I32:
            return new int;
        case I64:
            return new long long;
        case U8:
            return new unsigned char;
        case U16:
            return new unsigned short;
        case U32:
            return new unsigned int;
        case U64:
            return new unsigned long long;
        case F32:
            return new float;
        case F64:
            return new double;
        case STR:
            return new char[max_str_size];
        case BOOL:
            return new bool;
        default:
            return nullptr;
    }
}

void deallocate_data_type(void* data, DataType type){
    switch(type){
        case I8:
            delete (char*)data; break;
        case I16:
            delete (short*)data; break;
        case I32:
            delete (int*)data; break;
        case I64:
            delete (long long*)data; break;
        case U8:
            delete (unsigned char*)data; break;
        case U16:
            delete (unsigned short*)data; break;
        case U32:
            delete (unsigned int*)data; break;
        case U64:
            delete (unsigned long long*)data; break;
        case F32:
            delete (float*)data; break;
        case F64:
            delete (double*)data; break;
        case STR:
            delete[] (char*)data; break;
        case BOOL:
            delete (bool*)data; break;
    }
}

int get_data_type_size(DataType type)
{
    switch(type){
        case I8:
            return sizeof(char);
        case I16:
            return sizeof(short);
        case I32:
            return sizeof(int);
        case I64:
            return sizeof(long long);
        case U8:
            return sizeof(unsigned char);
        case U16:
            return sizeof(unsigned short);
        case U32:
            return sizeof(unsigned int);
        case U64:
            return sizeof(unsigned long long);
        case F32:
            return sizeof(float);
        case F64:
            return sizeof(double);
        case STR:
            return max_str_size_i * sizeof(char);
        case BOOL:
            return sizeof(bool);
        default:
            return 0;
    }
}

void set_data_value(void* data, DataType type, void* value){
    if(!data || !value){
        return;
    }
    int size = get_data_type_size(type);
    memcpy(data, value, size);
}

std::string get_data_value(void* data, DataType type)
{
    if(!data){
        return "";
    }
    int size = get_data_type_size(type);
    switch(type){
        case I8:
            return std::to_string(*(char*)data);
        case I16:
            return std::to_string(*(short*)data);
        case I32:
            return std::to_string(*(int*)data);
        case I64:
            return std::to_string(*(long long*)data);
        case U8:
            return std::to_string(*(unsigned char*)data);
        case U16:
            return std::to_string(*(unsigned short*)data);
        case U32:
            return std::to_string(*(unsigned int*)data);
        case U64:
            return std::to_string(*(unsigned long long*)data);
        case F32:
            return std::to_string(*(float*)data);
        case F64:
            return std::to_string(*(double*)data);
        case STR:
            return std::string((char*)data);
        case BOOL:
            return std::to_string(*(bool*)data);
        default:
            return "";
    }
}

Operation::Operation(OperationType type, int value) : op(type), arg(value) {
    memset(ops, 0, sizeof(int) * 64);
}

Operation::Operation(OperationType type, int value, int _ops[64]) : op(type), arg(value) 
{
    memcpy(ops, _ops, sizeof(int) * 64);
}