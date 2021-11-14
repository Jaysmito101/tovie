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
        default:
            return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& os, OperationType type)
{
    os << to_string(type);
    return os;
}

Operation::Operation(OperationType type, int value) : op(type), arg(value) {}