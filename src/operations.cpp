#include "operations.h"

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

std::ostream& operator<<(std::ostream& os, DataType type)
{
    os << to_string(type);
    return os;
}

Operation::Operation(OperationType type, int value) : op(type), arg(value) {}

Operation::Operation(OperationType type, int value, int _ops[64]) : op(type), arg(value) 
{
    memcpy(ops, _ops, sizeof(int) * 64);
}