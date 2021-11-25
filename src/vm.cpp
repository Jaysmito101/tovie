#include "vm.hpp"

#include "runtimelib.hpp"
#include "debug.hpp"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <array>
#include <iostream>
#include <type_traits>
#include <utility>
#include <string>
#include <unordered_map>

#include "stack.hpp"
#include "helpers.hpp"

struct Variable {
	DataType type;
	int		 id;
	void*	 value;
};

std::ostream& operator<<(std::ostream& os, std::unordered_map<int, Variable>& vt) {
	os << "[ ";
	for (auto it = vt.begin(); it != vt.end(); ++it) {
		os << "(" << it->second.id << " " << it->second.type << " " << get_data_value(it->second.value, it->second.type) << ") ";
	}
	os << "]";
	return os;
}

std::ostream& operator<<(std::ostream& os, ProcAddr& addr){
	os << addr.procId << " : " << addr.bAddr << " -> " << addr.eAddr;
	return os;
}

std::ostream& operator<<(std::ostream& os, std::vector<ProcAddr>& vec){
	os << "[ ";
	for (auto it = vec.begin(); it != vec.end(); ++it) {
		os << *it << " ";
	}
	os << "]";
	return os;
}

static void print(char* data, int size){
	std::cout << "[ ";
	for (int i = 0; i < size; i++) {
		std::cout << (int)*(data + i) << " ";
	}
	std::cout << "]";
}

static void push_variable(Stack& progStack, Variable& v, bool debug = false) {
	if (debug)
		std::cout << " [DEBUG]\t push variable : " << v.id << std::endl;
	switch(v.type)
	{
		case INT:
		{
			progStack.push(*(int*)v.value);
			break;
		}		
		case UINT:
		{
			progStack.push(*(unsigned int*)v.value);
			break;
		}
		case LONG:
		{
			progStack.push(*(long long*)v.value);
			break;
		}
		case ULONG:
		{
			progStack.push(*(unsigned long long*)v.value);
			break;
		}
		case FLOAT:
		{
			progStack.push(*(float*)v.value);
			break;
		}
		case DOUBLE:
		{
			progStack.push(*(double*)v.value);
			break;
		}
		case BOOL:
		{
			progStack.push(*(bool*)v.value);
			break;
		}
		case STRING:
		{
			progStack.push((char*)v.value);
			break;
		}
	}
}

static void pop_variable(Stack& progStack, Variable& v, bool debug = false) {
	if (debug)
		std::cout << " [DEBUG]\t pop variable : " << v.id << std::endl;
	switch(v.type)
	{
		case INT:
		{
			int tmp = progStack.pop_int();
			memcpy(v.value, &tmp, sizeof(int));
			break;
		}		
		case UINT:
		{
			unsigned int tmp = progStack.pop_uint();
			memcpy(v.value, &tmp, sizeof(unsigned int));
			break;
		}
		case LONG:
		{
			long long tmp = progStack.pop_llong();
			memcpy(v.value, &tmp, sizeof(long long));
			break;
		}
		case ULONG:
		{
			unsigned long long tmp = progStack.pop_ullong();
			memcpy(v.value, &tmp, sizeof(unsigned long long));
			break;
		}
		case FLOAT:
		{
			float tmp = progStack.pop_float();
			memcpy(v.value, &tmp, sizeof(float));
			break;
		}
		case DOUBLE:
		{
			double tmp = progStack.pop_double();
			memcpy(v.value, &tmp, sizeof(double));
			break;
		}
		case BOOL:
		{
			bool tmp = progStack.pop_bool();
			memcpy(v.value, &tmp, sizeof(bool));
			break;
		}
		case STRING:
		{
			char* data = progStack.pop_str();
			memcpy(v.value, data, strlen(data));
			delete[] data;
			break;
		}
	}
}

// The `#if 1` is just to use code folding in the Editor.
#if 1
void print(std::vector<int> vec) {
	std::cout << "[ ";
	for (int i : vec)
		std::cout << i << ' ';
	std::cout << "]" << std::endl;
}

void push(Stack& s, Operation op, bool debug = false) {
	if (debug)
		std::cout << " [DEBUG]\t push  size: " << op.arg << "(" << to_data_type_name(op.arg) << " " << get_value(op.ops, op.arg) << ")" << std::endl;
	int size = op.arg;
	for (int i = 0; i < size; i++) {
		s.push((char)op.ops[i]);
	}
}

void pop(Stack& s, Operation op, bool debug = false) {
	if (debug)
		std::cout << " [DEBUG]\t pop " << std::endl;
	s.pop_int();
}

void andop(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " && " << a << std::endl;
	s.push((int)(b && a));
}

void orop(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " || " << a << std::endl;
	s.push((int)(b || a));
}

void add(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " + " << a << std::endl;
	s.push(b + a);
}

void sub(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " - " << a << std::endl;
	s.push(b - a);
}

void mul(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " * " << a << std::endl;
	s.push(b * a);
}

void div(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " / " << a << std::endl;
	s.push(b / a);
}

void mod(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " % " << a << std::endl;
	s.push(b % a);
}

void println(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	s.push(a);
	std::cout << a << std::endl;
}

void printlns(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	s.push(a);
	std::cout << (char)a << std::endl;
}

void pow(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	int r = 1;
	for (int i = 0; i < a; i++) {
		r *= b;
	}
	if (debug)
		std::cout << b << " ^ " << a << std::endl;
	s.push(r);
}

void print(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	s.push(a);
	std::cout << a;
}

void prints(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	s.push(a);
	std::cout << (char)a;
}

void puts(Stack& s, Operation op, bool debug = false) {
	if (debug) {
		std::cout << " [DEBUG]\t puts" << std::endl;
	}
	char* str_c = s.pop_str();
	std::cout << str_c;
	delete[] str_c;
}

void putsln(Stack& s, Operation op, bool debug = false) {
	puts(s, op, debug);
	std::cout << std::endl;
}

void gt(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " > " << a << std::endl;
	s.push((int)(b > a));
}

void lt(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " < " << a << std::endl;
	s.push((int)(b < a));
}

void ge(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " >= " << a << std::endl;
	s.push((int)(b >= a));
}

void le(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " <= " << a << std::endl;
	s.push((int)(b <= a));
}

void eq(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	if (debug)
		std::cout << " [DEBUG]\t" << a << " == " << b << std::endl;
	s.push((int)(b == a));
}

void neq(Stack& s, Operation op, bool debug = false) {
	int a = s.pop_int();
	int b = s.pop_int();
	if (debug)
		std::cout << " [DEBUG]\t" << a << " != " << b << std::endl;
	s.push((int)(b != a));
}

void inputi(Stack& s, Operation op, bool debug = false) {
	if (debug)
		std::cout << " [DEBUG]\tinput integer" << std::endl;
	int n = 0;
	std::cin >> n;
	s.push(n);
}

void inputs(Stack& s, Operation op, bool debug = false) {
	if (debug)
		std::cout << " [DEBUG]\tinput string" << std::endl;
	std::string ss = "";
	std::getline(std::cin, ss);
	s.push(ss.c_str());
	s.push(ss.size());
}
#endif

#if 1
static Variable get_variable(int id, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV) {
	if (gV.find(id) != gV.end())
		return gV[id];
	if (lV.find(id) != lV.end())
		return lV[id];
	throw std::runtime_error("variable (" + std::to_string(id) + ") is not defined");
}

static void addVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.pop_int(), gV, lV);
	Variable b = get_variable(s.pop_int(), gV, lV);

	if (a.type != b.type)
		throw std::runtime_error("type mismatch");
	if (debug)
		std::cout << " [DEBUG]\t addvop" << a.id << " + " << b.id << std::endl;
	Variable c;
	c.type = a.type;
	c.value = allocate_data_type(c.type);
	memset(c.value, 0, get_data_type_size(c.type));
	switch (c.type) {
	case INT:
		*(int*) c.value = *(int*) a.value + *(int*) b.value;
		break;
	case UINT:
		*(unsigned int*) c.value = *(unsigned int*) a.value + *(unsigned int*) b.value;
		break;
	case LONG:
		*(long*) c.value = *(long*) a.value + *(long*) b.value;
		break;
	case ULONG:
		*(unsigned long*) c.value = *(unsigned long*) a.value + *(unsigned long*) b.value;
		break;
	case DOUBLE:
		*(double*) c.value = *(double*) a.value + *(double*) b.value;
		break;
	case FLOAT:
		*(float*) c.value = *(float*) a.value + *(float*) b.value;
		break;
	case STRING:
		strcat((char*)c.value, (char*)a.value);
		strcat((char*)c.value, (char*)b.value);
		break;
	default:
		throw std::runtime_error("type " + to_string(c.type) + " is not supported for op add");
		break;	
	}
	push_variable(s, c, debug);
	deallocate_data_type(c.value, c.type);
}

static void subVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.pop_int(), gV, lV);
	Variable b = get_variable(s.pop_int(), gV, lV);

	if (a.type != b.type)
		throw std::runtime_error("type mismatch");
	if (debug)
		std::cout << " [DEBUG]\t subvop" << a.id << " + " << b.id << std::endl;
	if(a.type != b.type)
		throw std::runtime_error("type mismatch");
	
	Variable c;
	c.type = a.type;
	c.value = allocate_data_type(c.type);
	memset(c.value, 0, get_data_type_size(c.type));
	switch (c.type) {
	case INT:
		*(int*) c.value = *(int*) a.value - *(int*) b.value;
		break;
	case UINT:
		*(unsigned int*) c.value = *(unsigned int*) a.value - *(unsigned int*) b.value;
		break;
	case LONG:
		*(long*) c.value = *(long*) a.value - *(long*) b.value;
		break;
	case ULONG:
		*(unsigned long*) c.value = *(unsigned long*) a.value - *(unsigned long*) b.value;
		break;
	case DOUBLE:
		*(double*) c.value = *(double*) a.value - *(double*) b.value;
		break;
	case FLOAT:
		*(float*) c.value = *(float*) a.value - *(float*) b.value;
		break;
	default:
		throw std::runtime_error("type " + to_string(c.type) + " is not supported for op sub");
		break;	
	}
	push_variable(s, c, debug);
	deallocate_data_type(c.value, c.type);
}

static void mulVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.pop_int(), gV, lV);
	Variable b = get_variable(s.pop_int(), gV, lV);

	if (a.type != b.type)
		throw std::runtime_error("type mismatch");
	if (debug)
		std::cout << " [DEBUG]\t mulvop" << a.id << " + " << b.id << std::endl;
	Variable c;
	c.type = a.type;
	c.value = allocate_data_type(c.type);
	memset(c.value, 0, get_data_type_size(c.type));
	switch (c.type) {
	case INT:
		*(int*) c.value = *(int*) a.value * *(int*) b.value;
		break;
	case UINT:
		*(unsigned int*) c.value = *(unsigned int*) a.value * *(unsigned int*) b.value;
		break;
	case LONG:
		*(long*) c.value = *(long*) a.value * *(long*) b.value;
		break;
	case ULONG:
		*(unsigned long*) c.value = *(unsigned long*) a.value * *(unsigned long*) b.value;
		break;
	case DOUBLE:
		*(double*) c.value = *(double*) a.value * *(double*) b.value;
		break;
	case FLOAT:
		*(float*) c.value = *(float*) a.value * *(float*) b.value;
		break;
	default:
		throw std::runtime_error("type " + to_string(c.type) + " is not supported for op mul");
		break;	
	}
	push_variable(s, c, debug);
	deallocate_data_type(c.value, c.type);
}

static void divVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.pop_int(), gV, lV);
	Variable b = get_variable(s.pop_int(), gV, lV);

	if (a.type != b.type)
		throw std::runtime_error("type mismatch");
	if (debug)
		std::cout << " [DEBUG]\t divop" << a.id << " + " << b.id << std::endl;
	Variable c;
	c.type = a.type;
	c.value = allocate_data_type(c.type);
	memset(c.value, 0, get_data_type_size(c.type));
	switch (c.type) {
	case INT:
		*(int*) c.value = *(int*) a.value / *(int*) b.value;
		break;
	case UINT:
		*(unsigned int*) c.value = *(unsigned int*) a.value / *(unsigned int*) b.value;
		break;
	case LONG:
		*(long*) c.value = *(long*) a.value / *(long*) b.value;
		break;
	case ULONG:
		*(unsigned long*) c.value = *(unsigned long*) a.value / *(unsigned long*) b.value;
		break;
	case DOUBLE:
		*(double*) c.value = *(double*) a.value / *(double*) b.value;
		break;
	case FLOAT:
		*(float*) c.value = *(float*) a.value / *(float*) b.value;
		break;
	default:
		throw std::runtime_error("type " + to_string(c.type) + " is not supported for op div");
		break;	
	}
	push_variable(s, c, debug);
	deallocate_data_type(c.value, c.type);
}

static void modVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.pop_int(), gV, lV);
	Variable b = get_variable(s.pop_int(), gV, lV);
	if (a.type != b.type)
		throw std::runtime_error("type mismatch");
	if (debug)
		std::cout << " [DEBUG]\t addvop" << a.id << " + " << b.id << std::endl;
	Variable c;
	c.type = a.type;
	c.value = allocate_data_type(c.type);
	memset(c.value, 0, get_data_type_size(c.type));
	switch (c.type) {
	case INT:
		*(int*) c.value = *(int*) a.value % *(int*) b.value;
		break;
	case UINT:
		*(unsigned int*) c.value = *(unsigned int*) a.value % *(unsigned int*) b.value;
		break;
	case LONG:
		*(long*) c.value = *(long*) a.value % *(long*) b.value;
		break;
	case ULONG:
		*(unsigned long*) c.value = *(unsigned long*) a.value % *(unsigned long*) b.value;
		break;
	default:
		throw std::runtime_error("type " + to_string(c.type) + " is not supported for op mod");
		break;	
	}
	push_variable(s, c, debug);
	deallocate_data_type(c.value, c.type);
}

static void gtVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.pop_int(), gV, lV);
	Variable b = get_variable(s.pop_int(), gV, lV);
	if (a.type != b.type)
		throw std::runtime_error("type mismatch");
	if (debug)
		std::cout << " [DEBUG]\t gtvop" << a.id << " + " << b.id << std::endl;

	bool flag = false;
	switch (a.type) {
	case INT:
		flag = *(int*) a.value > *(int*) b.value;
		break;
	case UINT:
		flag = *(unsigned int*) a.value > *(unsigned int*) b.value;
		break;
	case LONG:
		flag = *(long*) a.value > *(long*) b.value;
		break;
	case ULONG:
		flag = *(unsigned long*) a.value > *(unsigned long*) b.value;
		break;
	case DOUBLE:
		flag = *(double*) a.value > *(double*) b.value;
		break;
	case FLOAT:
		flag = *(float*) a.value > *(float*) b.value;
		break;
	case STRING:
		flag = strcmp((char*) a.value, (char*) b.value) > 0;
		break;
	default:
		throw std::runtime_error("type " + to_string(a.type) + " is not supported for op gt");
		break;	
	}
	s.push((int)flag);
}

static void ltVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.pop_int(), gV, lV);
	Variable b = get_variable(s.pop_int(), gV, lV);
	if (a.type != b.type)
		throw std::runtime_error("type mismatch");
	if (debug)
		std::cout << " [DEBUG]\t ltvop" << a.id << " + " << b.id << std::endl;
	
	bool flag = false;
	switch (a.type) {
	case INT:
		flag = *(int*) a.value < *(int*) b.value;
		break;
	case UINT:
		flag = *(unsigned int*) a.value < *(unsigned int*) b.value;
		break;
	case LONG:
		flag = *(long*) a.value < *(long*) b.value;
		break;
	case ULONG:
		flag = *(unsigned long*) a.value < *(unsigned long*) b.value;
		break;
	case DOUBLE:
		flag = *(double*) a.value < *(double*) b.value;
		break;
	case FLOAT:
		flag = *(float*) a.value < *(float*) b.value;
		break;
	case STRING:
		flag = strcmp((char*) a.value, (char*) b.value) < 0;
		break;
	default:
		throw std::runtime_error("type " + to_string(a.type) + " is not supported for op lt");
		break;	
	}
	s.push((int)flag);
}

static void geVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.pop_int(), gV, lV);
	Variable b = get_variable(s.pop_int(), gV, lV);
	if (a.type != b.type)
		throw std::runtime_error("type mismatch");
	if (debug)
		std::cout << " [DEBUG]\t gevop" << a.id << " + " << b.id << std::endl;
	
	bool flag = false;
	switch (a.type) {
	case INT:
		flag = *(int*) a.value >= *(int*) b.value;
		break;
	case UINT:
		flag = *(unsigned int*) a.value >= *(unsigned int*) b.value;
		break;
	case LONG:
		flag = *(long*) a.value >= *(long*) b.value;
		break;
	case ULONG:
		flag = *(unsigned long*) a.value >= *(unsigned long*) b.value;
		break;
	case DOUBLE:
		flag = *(double*) a.value >= *(double*) b.value;
		break;
	case FLOAT:
		flag = *(float*) a.value >= *(float*) b.value;
		break;
	case STRING:
		flag = strcmp((char*) a.value, (char*) b.value) >= 0;
		break;
	default:
		throw std::runtime_error("type " + to_string(a.type) + " is not supported for op ge");
		break;	
	}
	s.push((int)flag);
}

static void leVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.pop_int(), gV, lV);
	Variable b = get_variable(s.pop_int(), gV, lV);
	if (a.type != b.type)
		throw std::runtime_error("type mismatch");
	if (debug)
		std::cout << " [DEBUG]\t levop" << a.id << " + " << b.id << std::endl;
	
	bool flag = false;
	switch (a.type) {
	case INT:
		flag = *(int*) a.value <= *(int*) b.value;
		break;
	case UINT:
		flag = *(unsigned int*) a.value <= *(unsigned int*) b.value;
		break;
	case LONG:
		flag = *(long*) a.value <= *(long*) b.value;
		break;
	case ULONG:
		flag = *(unsigned long*) a.value <= *(unsigned long*) b.value;
		break;
	case DOUBLE:
		flag = *(double*) a.value <= *(double*) b.value;
		break;
	case FLOAT:
		flag = *(float*) a.value <= *(float*) b.value;
		break;
	case STRING:
		flag = strcmp((char*) a.value, (char*) b.value) <= 0;
		break;
	default:
		throw std::runtime_error("type " + to_string(a.type) + " is not supported for op le");
		break;	
	}
	s.push((int)flag);
}

static void eqVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.pop_int(), gV, lV);
	Variable b = get_variable(s.pop_int(), gV, lV);
	if (a.type != b.type)
		throw std::runtime_error("type mismatch");
	if (debug)
		std::cout << " [DEBUG]\t eqvop" << a.id << " + " << b.id << std::endl;

	bool flag = false;
	switch (a.type) {
	case INT:
		flag = *(int*) a.value == *(int*) b.value;
		break;
	case UINT:
		flag = *(unsigned int*) a.value == *(unsigned int*) b.value;
		break;
	case LONG:
		flag = *(long*) a.value == *(long*) b.value;
		break;
	case ULONG:
		flag = *(unsigned long*) a.value == *(unsigned long*) b.value;
		break;
	case DOUBLE:
		flag = *(double*) a.value == *(double*) b.value;
		break;
	case FLOAT:
		flag = *(float*) a.value == *(float*) b.value;
		break;
	case STRING:
		flag = strcmp((char*) a.value, (char*) b.value) == 0;
		break;
	default:
		throw std::runtime_error("type " + to_string(a.type) + " is not supported for op eq");
		break;	
	}
	s.push((int)flag);
}

static void neqVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	eqVOP(s, gV, lV, debug);
	bool flag = !s.pop_bool();
	s.push((int)flag);
}

static void andVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.pop_int(), gV, lV);
	Variable b = get_variable(s.pop_int(), gV, lV);
	if (a.type && b.type)
		throw std::runtime_error("type mismatch");
	if (debug)
		std::cout << " [DEBUG]\t andvop" << a.id << " + " << b.id << std::endl;

	bool flag = false;
	switch (a.type) {
	case INT:
		flag = *(int*) a.value && *(int*) b.value;
		break;
	case UINT:
		flag = *(unsigned int*) a.value && *(unsigned int*) b.value;
		break;
	case LONG:
		flag = *(long*) a.value && *(long*) b.value;
		break;
	case ULONG:
		flag = *(unsigned long*) a.value && *(unsigned long*) b.value;
		break;
	case DOUBLE:
		flag = *(double*) a.value && *(double*) b.value;
		break;
	case FLOAT:
		flag = *(float*) a.value && *(float*) b.value;
		break;
	default:
		throw std::runtime_error("type " + to_string(a.type) + " is not supported for op and");
		break;	
	}
	s.push((int)flag);
	
}

static void orVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.pop_int(), gV, lV);
	Variable b = get_variable(s.pop_int(), gV, lV);
	if (a.type || b.type)
		throw std::runtime_error("type mismatch");
	if (debug)
		std::cout << " [DEBUG]\t orvop" << a.id << " + " << b.id << std::endl;
	
	bool flag = false;
	switch (a.type) {
	case INT:
		flag = *(int*) a.value || *(int*) b.value;
		break;
	case UINT:
		flag = *(unsigned int*) a.value || *(unsigned int*) b.value;
		break;
	case LONG:
		flag = *(long*) a.value || *(long*) b.value;
		break;
	case ULONG:
		flag = *(unsigned long*) a.value || *(unsigned long*) b.value;
		break;
	case DOUBLE:
		flag = *(double*) a.value || *(double*) b.value;
		break;
	case FLOAT:
		flag = *(float*) a.value || *(float*) b.value;
		break;
	default:
		throw std::runtime_error("type " + to_string(a.type) + " is not supported for op or");
		break;	
	}
	s.push((int)flag);
}

static void printVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	if(debug)
		std::cout << " [DEBUG]\t printvop" << std::endl;
	Variable a = get_variable(s.pop_int(), gV, lV);
	std::cout << get_data_value(a.value, a.type);
}
static void printlnVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	if(debug)
		std::cout << " [DEBUG]\t printlnvop" << std::endl;
	Variable a = get_variable(s.pop_int(), gV, lV);
	std::cout << get_data_value(a.value, a.type) << std::endl;
}
static void inputVOP(Stack& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	if(debug)
		std::cout << " [DEBUG]\t inputvop" << std::endl;
	Variable a = get_variable(s.pop_int(), gV, lV);
	switch (a.type) {
	case INT:
		std::cin >> *(int*) a.value;
		break;
	case UINT:
		std::cin >> *(unsigned int*) a.value;
		break;
	case LONG:
		std::cin >> *(long*) a.value;
		break;
	case ULONG:
		std::cin >> *(unsigned long*) a.value;
		break;
	case DOUBLE:
		std::cin >> *(double*) a.value;
		break;
	case FLOAT:
		std::cin >> *(float*) a.value;
		break;
	case STRING:
		std::cin >> *(char*) a.value;
		break;
	default:
		throw std::runtime_error("type " + to_string(a.type) + " is not supported for op input");
		break;	
	}
}
#endif

// reverse string
static void reverse(std::string& str) {
	std::reverse(str.begin(), str.end());
}

static std::vector<ProcAddr>							   procAddresses;
static std::unordered_map<int, Variable>							   gVars;
static void*														   runtimeLib = nullptr;
static std::unordered_map<std::string, std::function<void(void*, int)>> libProcs;

static ProcAddr get_proc_addr(int procId) {
	for (auto& proc : procAddresses) {
		if (proc.procId == procId)
			return proc;
	}
	throw std::runtime_error("proc " + std::to_string(procId) + " not found");
}

static void clearVars(std::unordered_map<int, Variable>& v) {
	for (auto& it : v) {
		if (it.second.value)
			deallocate_data_type(it.second.value, it.second.type);
	}
	v.clear();
}

static void loadLibProc(Stack& progStack, Operation op, bool debug) {
	std::string libPath(progStack.pop_str());
	libPath = libPath.substr(0, libPath.size() - 1);
	int			back	= 0;
	if (debug)
		std::cout << " [DEBUG]\t load library : " << libPath << std::endl;
	if (runtimeLib)
		close_runtime_lib(runtimeLib);
	libProcs.clear();
	runtimeLib = open_runtime_lib(libPath.c_str());
}

static void loadGlobals(std::vector<Operation>& ops) {
	clearVars(gVars);
	bool inProc = false;
	for (int i = 0; i < ops.size(); i++) {
		if (!inProc) {
			if (ops[i].op == OperationType::DECL) {
				Variable v;
				v.id	= ops[i].arg;
				v.type	= to_data_type(ops[i].ops[0]);
				v.value = allocate_data_type(v.type);
				memset(v.value, 0, get_data_type_size(v.type));
				gVars[v.id] = v;
			}
		}
		if (ops[i].op == OperationType::PROC && ops[i].arg != -1) {
			inProc = true;
		}
		if (ops[i].op == OperationType::PROC && ops[i].arg == -1) {
			inProc = false;
		}
	}
}

static void callLibProc(Stack& progStack, Operation op, bool debug = false) {
	std::string libProcName(progStack.pop_str());
	libProcName = libProcName.substr(0, libProcName.size() - 1);
	if (debug)
		std::cout << " [DEBUG]\t call library procedure : " << libProcName << std::endl;
	// Find Key libProcName exists in libProcs or not
	if (libProcs.find(libProcName) == libProcs.end()) {
		libProcs[libProcName] = get_runtimelib_proc(runtimeLib, libProcName.c_str());
	}
	libProcs[libProcName](progStack.get_data(), progStack.length());
}

static void loadProcs(std::vector<Operation>& ops) {
	procAddresses.clear();
	ProcAddr pAddr;
	bool	 inProc = false;
	for (int i = 0; i < ops.size(); i++) {
		if (inProc && ops[i].op == OperationType::PROC) {
			if (ops[i].arg == -1) {
				pAddr.eAddr					= i;
				inProc						= false;
				procAddresses.push_back(pAddr);
			} else {
				throw std::runtime_error("proc" + std::to_string(pAddr.procId) + " begin inside another proc error!");
			}
		} else if (ops[i].op == OperationType::PROC) {
			if (ops[i].arg > -1) {
				pAddr.procId = ops[i].arg;
				pAddr.bAddr	 = i;
				inProc		 = true;
			} else {
				throw std::runtime_error("proc end without begin error!");
			}
		}
	}
}


static void simulate_proc(Stack& progStack, std::vector<Operation>& ops, ProcAddr& pAddr, bool debug);

static void simulate_op(Stack& progStack, Operation op, unsigned long* i, int* memory, std::vector<Operation>& ops, bool debug, std::unordered_map<int, Variable>& lVars) {

	if (debug && false)	   // TMP
		std::cout << " [DEBUG]\t" << op.op << "\t\t" << op.arg << std::endl;
	if (debug) {
		std::cout << " [DEBUG] STACK (" << progStack.length() <<  ") : ";
		progStack.print();
		//std::cout << " [DEBUG] GVARS : " << gVars << std::endl;
		//std::cout << " [DEBUG] LVARS : " << lVars << std::endl;
		//std::cout << " [DEBUG] OP ID : " << *i << " [ " << op.op << " " << op.arg << " ]" << std::endl;
	}
	int a, b;
	switch (op.op) {
		case OperationType::PUSH:
			push(progStack, op, debug);
			break;
		case PUSHS:
		{
			char data[4096];
			int co = 0;
			*i += 1;
			while(co <= 4096)
			{
				unsigned long c = *i;
				c += 1;
				if(c >= ops.size() || ops[c - 1].op != OperationType::PUSHS){
					if(debug)
						std::cout << " [DEBUG] PUSHS done data : " << data << std::endl;
					break;
				}
				if(debug)
					std::cout << " [DEBUG] " << ops[*i].op << "\t\t" << ops[*i].arg << std::endl;
				data[co++] = (char)ops[*i].arg;
				*i += 1;
			}
			*i -= 1;
			progStack.push('\0');
			progStack.push(data, co);
			break;
		}
		case OperationType::AND:
			andop(progStack, op, debug);
			break;
		case OperationType::OR:
			orop(progStack, op, debug);
			break;
		case OperationType::POP:
			pop(progStack, op, debug);
			break;
		case OperationType::ADD:
			add(progStack, op, debug);
			break;
		case OperationType::SUB:
			sub(progStack, op, debug);
			break;
		case OperationType::MUL:
			mul(progStack, op, debug);
			break;
		case OperationType::DIV:
			div(progStack, op, debug);
			break;
		case OperationType::MOD:
			mod(progStack, op, debug);
			break;
		case OperationType::PRINTLN:
			println(progStack, op, debug);;
			break;
		case OperationType::PRINTLNS:
			printlns(progStack, op, debug);;
			break;
		case OperationType::DUP:
			a = progStack.pop_int();
			progStack.push(a);
			progStack.push(a);
			break;
		case OperationType::SWAP:
			a = progStack.pop_int();
			b = progStack.pop_int();			
			progStack.push(a);
			progStack.push(b);
			break;
		case OperationType::PRINTLNNS:
			throw std::runtime_error("printlnns deperecated!");
		case OperationType::PRINTLNN:
			throw std::runtime_error("printlnn deperecated!");
		case OperationType::PRINT:
			print(progStack, op, debug);
			break;
		case OperationType::PRINTS:
			prints(progStack, op, debug);
			break;
		case OperationType::PRINTNS:
			throw std::runtime_error("printns deperecated!");
		case OperationType::PRINTN:
			throw std::runtime_error("printn deperecated!");
		case OperationType::DUMP:
			println(progStack, op, debug);
			break;
		case OperationType::LOADLIB:
			loadLibProc(progStack, op, debug);
			break;
		case OperationType::DUMPS:
			printlns(progStack, op, debug);
			break;
		case OperationType::DUMPN:
			throw std::runtime_error("dumpn deperecated!");
		case OperationType::DUMPNS:
			throw std::runtime_error("dumpns deperecated!");
		case OperationType::PUTS:
			puts(progStack, op, debug);
			break;
		case OperationType::PUTSLN:
			putsln(progStack, op, debug);
			break;
		case OperationType::POW:
			pow(progStack, op, debug);
			break;
		case OperationType::GT:
			gt(progStack, op, debug);
			break;
		case OperationType::LT:
			lt(progStack, op, debug);
			break;
		case OperationType::GE:
			ge(progStack, op, debug);
			break;
		case OperationType::LE:
			le(progStack, op, debug);
			break;
		case OperationType::INPUTI:
			inputi(progStack, op, debug);
			break;
		case OperationType::INPUTS:
			inputs(progStack, op, debug);
			break;
		case OperationType::EQ:
			eq(progStack, op, debug);
			break;
		case OperationType::NEQ:
			neq(progStack, op, debug);
			break;
		case OperationType::DECL: {
			Variable v;
			v.id	= ops[*i].arg;
			v.type	= to_data_type(ops[*i].ops[0]);
			v.value = allocate_data_type(v.type);
			memset(v.value, 0, get_data_type_size(v.type));
			lVars[v.id] = v;
			break;
		}
		case OperationType::VAR: {
			Variable v;
			bool	 tp = false;
			if (gVars.find(op.arg) != gVars.end()) {
				v  = gVars[op.arg];
				tp = true;
			} else if (lVars.find(op.arg) != lVars.end()) {
				v  = lVars[op.arg];
				tp = true;
			}
			if (!tp && (op.ops[0] == -1 || op.ops[0] == -2 || op.ops[0] == -3) )
				throw std::runtime_error("variable not found error");
			if (op.ops[0] == -1) {
				if (debug)
					std::cout << " [DEBUG]\t"
							  << " VAR " << op.arg << " SETV" << std::endl;
				push_variable(progStack, v);
			}
			else if (op.ops[0] == -2) {
				if (debug)
					std::cout << " [DEBUG]\t"
							  << " VAR " << op.arg << " GETV" << std::endl;
				pop_variable(progStack, v);
			}
			else if(op.ops[0] == -3){
				if (debug)
					std::cout << " [DEBUG]\t"
							  << " VAR " << op.arg << " PUTV" << std::endl;
				progStack.push(v.id);
			}
			else {
				OperationType vop = (OperationType) op.arg;
				if (debug)
					std::cout << " [DEBUG]\t"
							  << " VAR " << op.arg << " VOP " << vop << std::endl;

				switch (vop) {
					case OperationType::ADD: {
						addVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::SUB: {
						subVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::MUL: {
						mulVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::DIV: {
						divVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::MOD: {
						modVOP(progStack, lVars, gVars, debug);
					}
					case OperationType::AND: {
						andVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::OR: {
						orVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::GT: {
						gtVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::LT: {
						ltVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::EQ: {
						eqVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::NEQ: {
						neqVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::GE: {
						geVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::LE: {
						leVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::PRINT: {
						printVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::PRINTLN: {
						printlnVOP(progStack, lVars, gVars, debug);
						break;
					}
					case OperationType::INPUTI: {
						inputVOP(progStack, lVars, gVars, debug);
						break;
					}
					default:
						throw std::runtime_error("invalid variable operation type");
				}
			}
			break;
		}
		case OperationType::CALL: {
			int procId = progStack.pop_int();
			if (debug)
				std::cout << " [DEBUG]\tcall proc_" << procId << std::endl;
			ProcAddr pAddr = get_proc_addr(procId);
			simulate_proc(progStack, ops, pAddr, debug);
			break;
		}
		case OperationType::NCALL: {
			callLibProc(progStack, op, debug);
			break;
		}
		case OperationType::RECEED: {
			throw std::runtime_error("RECEED operation deprecated!");
		}
		case OperationType::PROCEED: {
			throw std::runtime_error("PROCEED operation deprecated!");
		}
		case OperationType::MALLOC: {
			int count = progStack.pop_int();
			if (debug)
				std::cout << " [DEBUG]\t malloc " << count << std::endl;
			if (memory != nullptr)
				delete[] memory;
			memory = new int[count];
			break;
		}
		case OperationType::MEMGET: {
			if (op.arg == -1) {
				int addr = progStack.pop_int();
				int size = progStack.pop_int();
				if (debug)
					std::cout << " [DEBUG]\t memget " << addr << size << std::endl;
				for (int k = addr + size - 1; k >= addr; k--)
					progStack.push(memory[k]);
			} else {
				int addr = op.arg;
				if (debug)
					std::cout << " [DEBUG]\t memget " << addr << std::endl;
				progStack.push(memory[addr]);
			}
			break;
		}
		case OperationType::MEMSET: {
			if (op.arg == -1) {
				int addr = progStack.pop_int();
				int size = progStack.pop_int();
				if (debug)
					std::cout << " [DEBUG]\t memset " << addr << size << std::endl;
				for (int k = addr; k < addr + size; k++) {
					memory[k] = progStack.pop_int();
				}
			} else {
				int addr = op.arg;
				if (debug)
					std::cout << " [DEBUG]\t memset " << addr << std::endl;
				memory[addr] = progStack.pop_int();
			}
			break;
		}
		case OperationType::IF: {
			if (op.arg == 0) {
				int condition = progStack.pop_int();
				if (debug)
					std::cout << " [DEBUG]\t if " << condition << std::endl;
				if (condition) {
					// For future use
				} else {
					int tmp = 0;
					for (int j = *i; j < ops.size(); j++) {
						if (ops[j].op == OperationType::IF && ops[j].arg == 0)
							tmp++;
						else if (ops[j].op == OperationType::IF && ops[j].arg == 1) {
							tmp--;
							if (tmp == 0) {
								*i = j;
								break;
							}
						}
					}
					if (debug)
						std::cout << " [DEBUG]\t skipping to op " << (*i + 1) << std::endl;
				}
			} else if (op.arg == 1) {
				// For future use
			}
			break;
		}
		case OperationType::WHILE: {
			if (op.arg == 0) {
				int condition = progStack.pop_int();
				if (condition) {
					// For future use
					if (debug)
						std::cout << " [DEBUG]\t while " << condition << std::endl;
				} else {
					if (debug)
						std::cout << " [DEBUG]\t skipping to op " << (i + 1) << std::endl;
					int tmp = 0;
					for (int j = *i; j < ops.size(); j++) {
						if (ops[j].op == OperationType::WHILE && ops[j].arg == 0)
							tmp++;
						else if (ops[j].op == OperationType::WHILE && ops[j].arg == 1) {
							tmp--;
							if (tmp == 0) {
								*i = j;
								break;
							}
						}
					}
				}
			} else if (op.arg == 1) {
				int tmp = 0;
				for (int j = *i; j >= 0; j--) {
					if (ops[j].op == OperationType::WHILE && ops[j].arg == 1)
						tmp++;
					else if (ops[j].op == OperationType::WHILE && ops[j].arg == 0) {
						tmp--;
						if (tmp == 0) {
							*i = j - 1;
							break;
						}
					}
				}
				if (debug) {
					std::cout << " [DEBUG]\t moving to op " << (*i + 1) << std::endl;
				}
			}
			break;
		}
		case OperationType::FOR: {
			if (op.arg != -1) {
				int end = 0;
				int tmp = 0;
				for (int j = *i; j < ops.size(); j++) {
					if (ops[j].op == OperationType::FOR && ops[j].arg != -1) {
						tmp++;
					} else if (ops[j].op == OperationType::FOR && ops[j].arg == -1) {
						tmp--;
						if (tmp == 0) {
							end = j;
							break;
						}
					}
				}
				int count = op.arg;
				if (op.arg == -2) {
					count = progStack.pop_int();
				}
				if (debug) {
					std::cout << " [DEBUG]\t for " << count << " times from " << *i << " to " << end << std::endl;
				}
				for (int k = 0; k < count; k++) {
					if (debug)
						std::cout << " [DEBUG]\t for " << k + 1 << "/" << count << std::endl;
					for (unsigned long p = *i + 1; p < end && p >= *i; p++)
						simulate_op(progStack, ops[p], &p, memory, ops, debug, lVars);
				}
				*i = end;
			}

			break;
		}
		case OperationType::EXIT: {
			int code = progStack.pop_int();
			if (debug)
				std::cout << " [DEBUG]\texit " << code << std::endl;
			if (memory != nullptr)
				delete[] memory;
			exit(code);
		}
		case OperationType::RET: {
			if (debug)
				std::cout << " [DEBUG]\treturn" << std::endl;
			return;
		}
	}
}


static void simulate_proc(Stack& progStack, std::vector<Operation>& ops, ProcAddr& pAddr, bool debug) {
	int*							  memory	  = new int[1024];
	int								  endIdToSkip = 0;
	std::unordered_map<int, Variable> lVars;
	for (unsigned long i = pAddr.bAddr + 1; i < pAddr.eAddr; i++) {
		Operation op = ops[i];
		if (op.op == OperationType::RET) {
			if (debug)
				std::cout << " [DEBUG]\treturn" << std::endl;
			if (memory)
				delete[] memory;
			clearVars(lVars);
			return;
		}
		simulate_op(progStack, op, &i, memory, ops, debug, lVars);
	}
	if (memory)
		delete[] memory;
	clearVars(lVars);
}

void simulate(std::vector<Operation> ops, bool debug) {
	loadProcs(ops);
	loadGlobals(ops);
	runtimeLib = nullptr;
	Stack progStack;
	progStack.push(0);
	ProcAddr pAddr = get_proc_addr(0);
	simulate_proc(progStack, ops, pAddr, debug);
}