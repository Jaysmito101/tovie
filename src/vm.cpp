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

struct ProcAddr {
	int			  procId;
	unsigned long long bAddr, eAddr;

	ProcAddr() {
		procId = -1;
		bAddr = eAddr = 0;
	}

	ProcAddr(int procIdi, unsigned long long bAddri, unsigned long long eAddri) {
		procId = procIdi;
		bAddr	 = bAddri;
		eAddr	 = eAddri;
	}
};

#ifdef __unix__
char* strrev(char* str) {
	char *p1, *p2;

	if (!str || !*str)
		return str;
	for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	return str;
}
#endif

struct Variable {
	DataType type;
	int		 id;
	void*	 value;
};

std::ostream& operator<<(std::ostream& os, std::unordered_map<int, Variable> vt) {
	os << "[ ";
	for (auto it = vt.begin(); it != vt.end(); ++it) {
		os << "(" << it->second.id << " " << it->second.type << " " << get_data_value(it->second.value, it->second.type) << ") ";
	}
	os << "]";
	return os;
}

std::ostream& operator<<(std::ostream& os, ProcAddr addr){
	os << addr.procId << " : " << addr.bAddr << " -> " << addr.eAddr;
	return os;
}

std::ostream& operator<<(std::ostream& os, std::unordered_map<int, ProcAddr> pt){
	os << "[ ";
	for (auto it = pt.begin(); it != pt.end(); ++it) {
		os << "(" << it->second.procId << " " << it->second.bAddr << " " << it->second.eAddr << ") ";
	}
	os << "]";
	return os;
}

static void push_variable(std::vector<int>& progStack, Variable& v, bool debug = false) {
	if (debug)
		std::cout << " [DEBUG]\t push variable : " << v.id << std::endl;
	if (v.type == DataType::INT) {
		int tmp = *(int*) v.value;
		progStack.push_back(tmp);
		return;
	} else if (v.type == DataType::UINT) {
		unsigned int tmp = *(unsigned int*) v.value;
		progStack.push_back(tmp);
		return;
	} else if (v.type == DataType::BOOL) {
		bool tmp = *(bool*) v.value;
		progStack.push_back(tmp);
		return;
	} else if (v.type == DataType::STRING) {
		char* data = (char*) v.value;
		for (int i = 0; i < strlen(data); i++) {
			progStack.push_back(data[i]);
		}
		progStack.push_back(-1);
		return;
	} else {
		int			   size = get_data_type_size(v.type);
		unsigned char* data = (unsigned char*) v.value;
		for (int i = size - 1; i >= 0; i--) {
			progStack.push_back(data[i]);
		}
		return;
	}
}

static void pop_variable(std::vector<int>& progStack, Variable& v, bool debug = false) {
	if (debug)
		std::cout << " [DEBUG]\t pop variable : " << v.id << std::endl;
	if (v.type == DataType::INT || v.type == DataType::UINT) {
		int tmp = progStack.back();
		progStack.pop_back();
		memcpy(v.value, &tmp, sizeof(int));
		return;
	} else if (v.type == DataType::BOOL) {
		int tmp = progStack.back();
		progStack.pop_back();
		bool val = (bool) tmp;
		memcpy(v.value, &val, sizeof(bool));
		return;
	} else if (v.type == DataType::STRING) {
		int	  size = get_data_type_size(v.type);
		char* data = (char*) malloc(size);
		memset(data, 0, size);
		for (int i = 0; i < size; i++) {
			int v = progStack.back();
			progStack.pop_back();
			if (v == -1)
				break;
			data[i] = (char) v;
		}
		strrev(data);
		memcpy(v.value, data, size);
		delete[] data;
	} else {
		int			   size = get_data_type_size(v.type);
		unsigned char* data = (unsigned char*) malloc(size);
		memset(data, 0, size);
		for (int i = 0; i < size; i++) {
			data[i] = progStack.back();
			progStack.pop_back();
		}
		memcpy(v.value, data, size);
		delete[] data;
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

void push(std::vector<int>& s, Operation op, bool debug = false) {
	if (debug)
		std::cout << " [DEBUG]\t push " << op.arg << std::endl;
	s.push_back(op.arg);
}

void pop(std::vector<int>& s, Operation op, bool debug = false) {
	if (debug)
		std::cout << " [DEBUG]\t pop " << std::endl;
	s.pop_back();
}

void andop(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " && " << a << std::endl;
	s.push_back(b && a);
}

void orop(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " || " << a << std::endl;
	s.push_back(b || a);
}

void add(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " + " << a << std::endl;
	s.push_back(b + a);
}

void sub(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " - " << a << std::endl;
	s.push_back(b - a);
}

void mul(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " * " << a << std::endl;
	s.push_back(b * a);
}

void div(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " / " << a << std::endl;
	s.push_back(b / a);
}

void mod(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " % " << a << std::endl;
	s.push_back(b % a);
}

void dump(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	std::cout << a << std::endl;
}

void dumps(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	std::cout << (char) a << std::endl;
}

void dumpns(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s[(int) s.size() - 1 - s.back()];
	std::cout << (char) a << std::endl;
}

void dumpn(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s[(int) s.size() - 1 - s.back()];
	std::cout << a << std::endl;
}

void println(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	std::cout << a << std::endl;
}

void printlns(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	std::cout << (char) a << std::endl;
}

void printlnns(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s[(int) s.size() - 1 - s.back()];
	std::cout << (char) a << std::endl;
}

void printlnn(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s[(int) s.size() - 1 - s.back()];
	std::cout << a << std::endl;
}

void pow(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	int r = 1;
	for (int i = 0; i < a; i++) {
		r *= b;
	}
	if (debug)
		std::cout << b << " ^ " << a << std::endl;
	s.push_back(r);
}

void print(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	std::cout << a;
}

void prints(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	std::cout << (char) a;
}

void printns(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s[(int) s.size() - 1 - s.back()];
	std::cout << (char) a;
}

void printn(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s[(int) s.size() - 1 - s.back()];
	std::cout << a;
}

void puts(std::vector<int>& s, Operation op, bool debug = false) {
	int l = s.back();
	s.pop_back();
	int i = s.size() - l;
	if (debug) {
		std::cout << " [DEBUG]\t puts" << std::endl;
		std::cout << " [DEBUG]\t\t"
				  << " length : " << l << std::endl;
		std::cout << " [DEBUG]\t\t"
				  << " begin : " << i << std::endl;
	}
	for (int j = i; j < s.size(); j++)
		std::cout << (char) s[j];
}

void putsln(std::vector<int>& s, Operation op, bool debug = false) {
	int l = s.back();
	s.pop_back();
	int i = s.size() - l;
	if (debug) {
		std::cout << " [DEBUG]\t putsln" << std::endl;
		std::cout << " [DEBUG]\t\t"
				  << " length : " << l << std::endl;
		std::cout << " [DEBUG]\t\t"
				  << " begin : " << i << std::endl;
	}
	for (int j = i; j < s.size(); j++)
		std::cout << (char) s[j];
	std::cout << std::endl;
}

void gt(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " > " << a << std::endl;
	s.push_back(b > a);
}

void lt(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " < " << a << std::endl;
	s.push_back(b < a);
}

void ge(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " >= " << a << std::endl;
	s.push_back(b >= a);
}

void le(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	if (debug)
		std::cout << " [DEBUG]\t" << b << " <= " << a << std::endl;
	s.push_back(b <= a);
}

void eq(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	if (debug)
		std::cout << " [DEBUG]\t" << a << " == " << b << std::endl;
	s.push_back(a == b);
}

void neq(std::vector<int>& s, Operation op, bool debug = false) {
	int a = s.back();
	s.pop_back();
	int b = s.back();
	s.pop_back();
	if (debug)
		std::cout << " [DEBUG]\t" << a << " != " << b << std::endl;
	s.push_back(a != b);
}

void inputi(std::vector<int>& s, Operation op, bool debug = false) {
	if (debug)
		std::cout << " [DEBUG]\tinput integer" << std::endl;
	int n = 0;
	std::cin >> n;
	s.push_back(n);
}

void inputs(std::vector<int>& s, Operation op, bool debug = false) {
	if (debug)
		std::cout << " [DEBUG]\tinput string" << std::endl;
	std::string ss = "";
	std::getline(std::cin, ss);
	s.push_back(-1);
	for (int i = 0; i < ss.size(); i++)
		s.push_back(ss[i]);
	s.push_back(ss.size());
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

static void addVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	Variable b = get_variable(s.back(), gV, lV);
	s.pop_back();
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
		*(unsigned long long*) c.value = *(unsigned long long*) a.value + *(unsigned long long*) b.value;
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

static void subVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	Variable b = get_variable(s.back(), gV, lV);
	s.pop_back();
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
		*(unsigned long long*) c.value = *(unsigned long long*) a.value - *(unsigned long long*) b.value;
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

static void mulVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	Variable b = get_variable(s.back(), gV, lV);
	s.pop_back();
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
		*(unsigned long long*) c.value = *(unsigned long long*) a.value * *(unsigned long long*) b.value;
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

static void divVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	Variable b = get_variable(s.back(), gV, lV);
	s.pop_back();
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
		*(unsigned long long*) c.value = *(unsigned long long*) a.value / *(unsigned long long*) b.value;
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

static void modVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	Variable b = get_variable(s.back(), gV, lV);
	s.pop_back();
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
		*(unsigned long long*) c.value = *(unsigned long long*) a.value % *(unsigned long long*) b.value;
		break;
	default:
		throw std::runtime_error("type " + to_string(c.type) + " is not supported for op mod");
		break;	
	}
	push_variable(s, c, debug);
	deallocate_data_type(c.value, c.type);
}

static void gtVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	Variable b = get_variable(s.back(), gV, lV);
	s.pop_back();
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
		flag = *(unsigned long long*) a.value > *(unsigned long long*) b.value;
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
	s.push_back(flag);
}

static void ltVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	Variable b = get_variable(s.back(), gV, lV);
	s.pop_back();
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
		flag = *(unsigned long long*) a.value < *(unsigned long long*) b.value;
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
	s.push_back(flag);
}

static void geVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	Variable b = get_variable(s.back(), gV, lV);
	s.pop_back();
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
		flag = *(unsigned long long*) a.value >= *(unsigned long long*) b.value;
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
	s.push_back(flag);
}

static void leVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	Variable b = get_variable(s.back(), gV, lV);
	s.pop_back();
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
		flag = *(unsigned long long*) a.value <= *(unsigned long long*) b.value;
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
	s.push_back(flag);
}

static void eqVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	Variable b = get_variable(s.back(), gV, lV);
	s.pop_back();
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
		flag = *(unsigned long long*) a.value == *(unsigned long long*) b.value;
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
	s.push_back(flag);
}

static void neqVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	eqVOP(s, gV, lV, debug);
	bool flag = !s.back();
	s.pop_back();
	s.push_back(flag);
}

static void andVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	Variable b = get_variable(s.back(), gV, lV);
	s.pop_back();
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
		flag = *(unsigned long long*) a.value && *(unsigned long long*) b.value;
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
	s.push_back(flag);
	
}

static void orVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	Variable b = get_variable(s.back(), gV, lV);
	s.pop_back();
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
		flag = *(unsigned long long*) a.value || *(unsigned long long*) b.value;
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
	s.push_back(flag);
}

static void printVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	std::cout << get_data_value(a.value, a.type);
}
static void printlnVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
	std::cout << get_data_value(a.value, a.type) << std::endl;
}
static void inputVOP(std::vector<int>& s, std::unordered_map<int, Variable>& gV, std::unordered_map<int, Variable>& lV, bool debug = false) {
	Variable a = get_variable(s.back(), gV, lV);
	s.pop_back();
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
		std::cin >> *(unsigned long long*) a.value;
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

static std::unordered_map<int, ProcAddr>							   procAddresses;
static std::unordered_map<int, Variable>							   gVars;
static void*														   runtimeLib = nullptr;
static std::unordered_map<std::string, std::function<void(int*, int)>> libProcs;

static void clearVars(std::unordered_map<int, Variable>& v) {
	for (auto& it : v) {
		if (it.second.value)
			deallocate_data_type(it.second.value, it.second.type);
	}
	v.clear();
}

static void loadLibProc(std::vector<int>& progStack, Operation op, bool debug) {
	std::string libPath = "";
	int			back	= 0;
	while (true) {
		back = progStack.back();
		progStack.pop_back();
		if (back == -1)
			break;
		libPath += (char) back;
	}
	reverse(libPath);
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

static void callLibProc(std::vector<int>& progStack, Operation op, bool debug = false) {
	std::string libProcName = "";
	int			back		= 0;
	while (true) {
		back = progStack.back();
		progStack.pop_back();
		if (back == -1)
			break;
		libProcName += (char) back;
	}
	reverse(libProcName);
	if (debug)
		std::cout << " [DEBUG]\t call library procedure : " << libProcName << std::endl;
	// Find Key libProcName exists in libProcs or not
	if (libProcs.find(libProcName) == libProcs.end()) {
		libProcs[libProcName] = get_runtimelib_proc(runtimeLib, libProcName.c_str());
	}
	libProcs[libProcName](progStack.data(), progStack.size());
}

static void loadProcs(std::vector<Operation> ops) {
	procAddresses.clear();
	ProcAddr pAddr;
	for (int i = 0; i < ops.size(); i++) {
		if (ops[i].op == OperationType::PROC && ops[i].arg != -1) {
			pAddr.procId = ops[i].arg;
			pAddr.bAddr = i;
		}
		else if(ops[i].op == OperationType::PROC && ops[i].arg == -1) {
			pAddr.eAddr = i;
			procAddresses[pAddr.procId] = pAddr;

			// TODO: FIXME:
			std::cout << procAddresses[pAddr.procId] << std::endl;
			std::cout << procAddresses[0] << std::endl;

		}
	}
}


static void simulate_proc(std::vector<int>& progStack, std::vector<Operation> ops, ProcAddr pAddr, bool debug);

static void simulate_op(std::vector<int>& progStack, Operation op, unsigned long long* i, int* memory, std::vector<Operation>& ops, bool debug, std::unordered_map<int, Variable>& lVars) {

	if (debug && false)	   // TMP
		std::cout << " [DEBUG]\t" << op.op << "\t\t" << op.arg << std::endl;
	if (debug) {
		std::cout << " [DEBUG] STACK : ";
		print(progStack);
		std::cout << " [DEBUG] GVARS : " << gVars << std::endl;
		std::cout << " [DEBUG] LVARS : " << lVars << std::endl;
		std::cout << " [DEBUG] OP ID : " << *i << " [ " << op.op << " " << op.arg << " ]" << std::endl;
	}
	int a, b;
	switch (op.op) {
		case OperationType::PUSH:
			push(progStack, op, debug);
			break;
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
			println(progStack, op, debug);
			break;
		case OperationType::PRINTLNS:
			printlns(progStack, op, debug);
			break;
		case OperationType::DUP:
			progStack.push_back(progStack.back());
			break;
		case OperationType::SWAP:
			a = progStack.back();
			progStack.pop_back();
			b = progStack.back();
			progStack.pop_back();
			progStack.push_back(a);
			progStack.push_back(b);
			break;
		case OperationType::PRINTLNNS:
			printlnns(progStack, op, debug);
			break;
		case OperationType::PRINTLNN:
			printlnn(progStack, op, debug);
			break;
		case OperationType::PRINT:
			print(progStack, op, debug);
			break;
		case OperationType::PRINTS:
			prints(progStack, op, debug);
			break;
		case OperationType::PRINTNS:
			printns(progStack, op, debug);
			break;
		case OperationType::PRINTN:
			printn(progStack, op, debug);
			break;
		case OperationType::DUMP:
			dump(progStack, op, debug);
			break;
		case OperationType::LOADLIB:
			loadLibProc(progStack, op, debug);
			break;
		case OperationType::DUMPS:
			dumps(progStack, op, debug);
			break;
		case OperationType::DUMPN:
			dumpn(progStack, op, debug);
			break;
		case OperationType::DUMPNS:
			dumpns(progStack, op, debug);
			break;
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
			if (!tp)
				throw std::runtime_error("variable not found error");
			if (op.ops[0] == -1) {
				if (debug)
					std::cout << " [DEBUG]\t"
							  << " VAR " << op.arg << " SETV" << std::endl;
				push_variable(progStack, v);
			} else if (op.ops[0] == -2) {
				if (debug)
					std::cout << " [DEBUG]\t"
							  << " VAR " << op.arg << " GETV" << std::endl;
				pop_variable(progStack, v);
			} else {
				if (debug)
					std::cout << " [DEBUG]\t"
							  << " VAR " << op.arg << " VOP" << std::endl;
				OperationType vop = (OperationType) op.ops[0];
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
					}
					case OperationType::LT: {
						ltVOP(progStack, lVars, gVars, debug);
					}
					case OperationType::EQ: {
						eqVOP(progStack, lVars, gVars, debug);
					}
					case OperationType::NEQ: {
						neqVOP(progStack, lVars, gVars, debug);
					}
					case OperationType::GE: {
						geVOP(progStack, lVars, gVars, debug);
					}
					case OperationType::LE: {
						leVOP(progStack, lVars, gVars, debug);
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
			int procId = progStack.back();
			progStack.pop_back();
			if (debug)
				std::cout << " [DEBUG]\tcall proc_" << procId << std::endl;
			if (procAddresses.find(procId) == procAddresses.end()) {
				throw std::runtime_error("proc_" + std::to_string(procId) + " not found!");
			}
			simulate_proc(progStack, ops, procAddresses[procId], debug);
			break;
		}
		case OperationType::NCALL: {
			callLibProc(progStack, op, debug);
			break;
		}
		case OperationType::RECEED: {
			int count = progStack.back();
			progStack.pop_back();
			if (progStack.back())
				*i -= count + 2;
			if (debug && progStack.back())
				std::cout << " [DEBUG]\t receed " << count << std::endl;
			break;
		}
		case OperationType::PROCEED: {
			int count = progStack.back();
			progStack.pop_back();
			if (progStack.back())
				*i += count - 1;
			if (debug && progStack.back())
				std::cout << " [DEBUG]\t porceed " << count << std::endl;
			break;
		}
		case OperationType::MALLOC: {
			int count = progStack.back();
			progStack.pop_back();
			if (debug)
				std::cout << " [DEBUG]\t malloc " << count << std::endl;
			if (memory != nullptr)
				delete[] memory;
			memory = new int[count];
			break;
		}
		case OperationType::MEMGET: {
			if (op.arg == -1) {
				int addr = progStack.back();
				progStack.pop_back();
				int size = progStack.back();
				progStack.pop_back();
				if (debug)
					std::cout << " [DEBUG]\t memget " << addr << size << std::endl;
				for (int k = addr + size - 1; k >= addr; k--)
					progStack.push_back(memory[k]);
			} else {
				int addr = op.arg;
				if (debug)
					std::cout << " [DEBUG]\t memget " << addr << std::endl;
				progStack.push_back(memory[addr]);
			}
			break;
		}
		case OperationType::MEMSET: {
			if (op.arg == -1) {
				int addr = progStack.back();
				progStack.pop_back();
				int size = progStack.back();
				progStack.pop_back();
				if (debug)
					std::cout << " [DEBUG]\t memset " << addr << size << std::endl;
				for (int k = addr; k < addr + size; k++) {
					memory[k] = progStack.back();
					progStack.pop_back();
				}
			} else {
				int addr = op.arg;
				if (debug)
					std::cout << " [DEBUG]\t memset " << addr << std::endl;
				memory[addr] = progStack.back();
				progStack.pop_back();
			}
			break;
		}
		case OperationType::IF: {
			if (op.arg == 0) {
				int condition = progStack.back();
				progStack.pop_back();
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
				int condition = progStack.back();
				progStack.pop_back();
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
					count = progStack.back();
					progStack.pop_back();
				}
				if (debug) {
					std::cout << " [DEBUG]\t for " << count << " times from " << *i << " to " << end << std::endl;
				}
				for (int k = 0; k < count; k++) {
					if (debug)
						std::cout << " [DEBUG]\t for " << k + 1 << "/" << count << std::endl;
					for (unsigned long long p = *i + 1; p < end && p >= *i; p++)
						simulate_op(progStack, ops[p], &p, memory, ops, debug, lVars);
				}
				*i = end;
			}

			break;
		}
		case OperationType::EXIT: {
			if (debug)
				std::cout << " [DEBUG]\texit " << progStack.back() << std::endl;
			if (memory != nullptr)
				delete[] memory;
			exit(progStack.back());
		}
		case OperationType::RET: {
			if (debug)
				std::cout << " [DEBUG]\treturn" << std::endl;
			return;
		}
	}
}


static void simulate_proc(std::vector<int>& progStack, std::vector<Operation> ops, ProcAddr pAddr, bool debug) {
	int*							  memory	  = new int[1024];
	int								  endIdToSkip = 0;
	std::unordered_map<int, Variable> lVars;
	for (unsigned long long i = pAddr.bAddr + 1; i < pAddr.eAddr; i++) {
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
	std::vector<int> progStack;
	progStack.push_back(0);
	if (procAddresses.find(0) == procAddresses.end()) {
		throw std::runtime_error("no main proc!");
	}
	std::cout << procAddresses << std::endl;
	simulate_proc(progStack, ops, procAddresses[0], debug);
}