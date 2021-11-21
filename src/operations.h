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
    DECL,
    VAR,
    OPERATION_COUNT
};

OperationType to_operation_type(int id);
std::string to_string(OperationType type);
std::ostream& operator<<(std::ostream& os, OperationType type);

enum DataType
{
    UNKNOWN = 0,
    I8,
    I16,
    I32,
    I64,
    U8,
    U16,
    U32,
    U64,
    F32,
    F64,
    STR,
    BOOL,
    DATA_TYPE_COUNT
};

DataType to_data_type(std::string type);
DataType to_data_type(int type);
std::string to_string(DataType type);
std::ostream& operator<<(std::ostream& os, DataType type);
void* allocate_data_type(DataType type, int max_str_size = 4096);
void deallocate_data_type(void* data, DataType type);
int get_data_type_size(DataType type);
void set_data_value(void* data, DataType type, void* value);
std::string get_data_value(void* data, DataType type);

struct Operation 
{
    Operation(OperationType type, int value = -1);
    Operation(OperationType type, int value, int ops[64]);
    OperationType op;
    int arg;
    int ops[64];
};