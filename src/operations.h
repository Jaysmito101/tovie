#pragma once

#include <string>
enum OperationType
{
    ADD = 0,
    SUB,
    MUL,
    DIV,
    MOD,
    POW,
    AND,
    OR,
    EQ,
    NEQ,
    GE,
    LE,
    GT,
    LT,
    PUSH,
    POP,
    PRINTLN,
    PRINTLNS,
    PRINTLNNS,
    PRINTLNN,
    PRINT,
    PRINTS,
    PRINTN,
    PRINTNS,
    DUMP,
    DUMPS,
    DUMPN,
    DUMPNS,
    RECEED,
    PROCEED,
    INPUTI,
    INPUTS,
    PUTS,
    PUTSLN,
    BEGIN,
    END,
    EXIT,
    PROC,
    CALL,
    NCALL,
    MALLOC,
    MEMSET,
    MEMGET,
    RET,
    IF,
    WHILE,
    DUP,
    SWAP,
    FOR,
    LOADLIB,
    OPERATION_COUNT
};

std::string to_string(OperationType type);
std::ostream& operator<<(std::ostream& os, OperationType type);

struct Operation 
{
    Operation(OperationType type, int value = -1);
    OperationType op;
    int arg;
};