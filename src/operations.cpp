#include "operations.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>

std::string to_string(OperationType type) {
	switch (type) {
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

std::ostream& operator<<(std::ostream& os, OperationType type) {
	os << to_string(type);
	return os;
}

std::string to_string(DataType type) {
	switch (type) {
		case INT:
			return "INT";
		case FLOAT:
			return "FLOAT";
		case LONG:
			return "LONG";
		case UINT:
			return "UINT";
		case ULONG:
			return "ULONG";
		case DOUBLE:
			return "DOUBLE";
		case STRING:
			return "STRING";
		case BOOL:
			return "BOOL";
		default:
			return "UNKNOWN";
	}
}

static std::string to_upper(std::string& s) {
	std::string result;
	for (char c : s) {
		result += toupper(c);
	}
	return result;
}

std::ostream& operator<<(std::ostream& os, DataType type) {
	os << to_string(type);
	return os;
}

DataType to_data_type(std::string token) {
	token = to_upper(token);
	if (token == "INT") {
		return INT;
	} else if (token == "FLOAT") {
		return FLOAT;
	} else if (token == "LONG") {
		return LONG;
	} else if (token == "UINT") {
		return UINT;
	} else if (token == "ULONG") {
		return ULONG;
	} else if (token == "DOUBLE") {
		return DOUBLE;
	} else if (token == "STRING") {
		return STRING;
	} else if (token == "BOOL") {
		return BOOL;
	} else {
		return UNKNOWN;
	}
}

DataType to_data_type(int type) {
	switch (type) {
		case INT:
			return INT;
		case FLOAT:
			return FLOAT;
		case LONG:
			return LONG;
		case UINT:
			return UINT;
		case ULONG:
			return ULONG;
		case DOUBLE:
			return DOUBLE;
		case STRING:
			return STRING;
		case BOOL:
			return BOOL;
		default:
			return UNKNOWN;
	}
}

static int max_str_size_i = 4096;

void* allocate_data_type(DataType type, int max_str_size) {
	max_str_size_i = max_str_size;
	switch (type) {
		case INT:
			return new int;
		case FLOAT:
			return new float;
		case LONG:
			return new long;
		case UINT:
			return new unsigned int;
		case ULONG:
			return new unsigned long;
		case DOUBLE:
			return new double;
		case STRING:
			return new char[max_str_size_i];
		case BOOL:
			return new bool;
		default:
			return nullptr;
	}
}

void deallocate_data_type(void* data, DataType type) {
	switch (type) {
		case INT:
			delete (int*)data;
			break;
		case FLOAT:
			delete (float*)data;
			break;
		case LONG:
			delete (long*)data;
			break;
		case UINT:
			delete (unsigned int*)data;
			break;
		case ULONG:
			delete (unsigned long*)data;
			break;
		case DOUBLE:
			delete (double*)data;
			break;
		case STRING:
			delete[] (char*)data;
			break;
		case BOOL:
			delete (bool*)data;
			break;
		default:
			break;
	}
}

int get_data_type_size(DataType type) {
	switch (type) {
		case INT:
			return sizeof(int);
		case FLOAT:
			return sizeof(float);
		case LONG:
			return sizeof(long);
		case UINT:
			return sizeof(unsigned int);
		case ULONG:
			return sizeof(unsigned long);
		case DOUBLE:
			return sizeof(double);
		case STRING:
			return max_str_size_i;
		case BOOL:
			return sizeof(bool);
		default:
			return 0;
	}
}

void set_data_value(void* data, DataType type, void* value) {
	if (!data || !value) {
		return;
	}
	int size = get_data_type_size(type);
	memcpy(data, value, size);
}

std::string get_data_value(void* data, DataType type) {
	if (!data) {
		return "";
	}
	int size = get_data_type_size(type);
	switch (type) {
		case INT:
			return std::to_string(*(int*)data);
		case FLOAT:
			return std::to_string(*(float*)data);
		case LONG:
			return std::to_string(*(long*)data);
		case UINT:
			return std::to_string(*(unsigned int*)data);
		case ULONG:
			return std::to_string(*(unsigned long*)data);
		case DOUBLE:
			return std::to_string(*(double*)data);
		case STRING:
			return std::string((char*)data);
		case BOOL:
			return std::to_string(*(bool*)data);
		default:
			return "";
	}	
}

Operation::Operation(OperationType type, int value)
	: op(type)
	, arg(value) {
	memset(ops, 0, sizeof(int) * 64);
}

Operation::Operation(OperationType type, int value, int _ops[64])
	: op(type)
	, arg(value) {
	memcpy(ops, _ops, sizeof(int) * 64);
}