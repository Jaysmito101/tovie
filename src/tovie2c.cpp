#include "codemaker.hpp"
#include "info.hpp"
#include "operations.hpp"
#include "tovie2py.hpp"

#include <stdexcept>
#include <unordered_map>

// the `#if 1` is to use code folding in the IDE
#if 1
std::string cSack = R"(
struct stack
{
    int size;
    float inc_factor;
    int* cptr;
    int* data;
};

void stack_validate(struct stack* s)
{
    if((*s).cptr == (*s).data + (*s).size){
        int* tmp = malloc((*s).size * (*s).inc_factor * sizeof(int));
        memcpy(tmp, (*s).data, (*s).size * sizeof(int)); 
        free((*s).data);
        (*s).data = tmp;
        (*s).cptr = (*s).data + (*s).size;
        (*s).size *= (*s).inc_factor;
    }
}

void stack_push(struct stack* s, int value)
{
    stack_validate(s);   
    *(*s).cptr = value;
    (*s).cptr += 1;
}

int stack_pop(struct stack* s)
{
    if((*s).cptr == (*s).data){
        printf("stack underflow!");
        exit(-1);
    }
    (*s).cptr -= 1;
    int value = *((*s).cptr);
    return value;
}

int stack_peek(struct stack* s)
{
    return *((*s).cptr);
}

int stack_get(struct stack* s, int index)
{
    return *((*s).cptr + index);
}

int stack_getback(struct stack* s, int depth)
{
    return *((*s).cptr - depth);
}

int stack_size(struct stack* s)
{
    return (int)((*s).cptr - (*s).data);
}

void stack_empty(struct stack* s)
{
    (*s).cptr = (*s).data;
}

void stack_print(struct stack* s)
{
    int* beg = (*s).data;
    while(beg < (*s).cptr){
        printf("%d ", *beg);
        beg += 1;
    }
    printf("\n");
}

void stack_free(struct stack* s)
{
    free((*s).data);
    free(s);
}

struct stack* stack_make(int init_size, float inc_factor)
{
    struct stack* s = malloc(sizeof(struct stack));
    (*s).size = init_size;
    (*s).inc_factor = 2;
    (*s).data = malloc(init_size * sizeof(int));
    (*s).cptr = (*s).data;
    return s;
}
)";

#	ifdef _WIN32
std::string dllLoader = R"(
LPCWSTR to_LPCWSTR(const char* str)
{
    size_t size = strlen(str) + 1;  
    wchar_t* outStr = malloc(sizeof(wchar_t) * size); 
    size_t outSize;
    mbstowcs(outStr, str, size);
    return outStr;
}

typedef void (*TovieNativeFunc)(int*, int);

TovieNativeFunc get_runtimelib_proc(HINSTANCE runtimeLib, const char* libProcName)
{
    HINSTANCE gllID = runtimeLib;    
    // resolve function address here
    TovieNativeFunc funci = (TovieNativeFunc)GetProcAddress(gllID, libProcName);
    if (!funci) {
        printf("failed to get proc address for %s", libProcName);
        exit(-1);
    }
    return funci;
}
)";
#	endif

#	ifdef __unix__
std::string dllLoader = R"(
typedef void (*TovieNativeFunc)(int*, int);
    
TovieNativeFunc get_runtimelib_proc(void* runtimeLib, const char* libProcName)
{
    TovieNativeFunc funci;
    funci = (TovieNativeFunc)dlsym(runtimeLib, libProcName);
    if (!funci) {
        printf("failed to get proc address for %s", libProcName);
        exit(-1);
    }
    return funci;
}
)";
#	endif

#endif

struct ProcAddr {
	int			  procId;
	unsigned long bAddr, eAddr;

	ProcAddr();

	ProcAddr(int procId, unsigned long bAddr, unsigned long eAddr);
};


static void transpile(CodeMaker& cm, Operation op) {
	switch (op.op) {
		case OperationType::PUSH: {
			cm.add_line("stack_push(_stack, " + std::to_string(op.arg) + ");");
			break;
		}
		case OperationType::AND: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, stackB && stackA);");
			break;
		}
		case OperationType::OR: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, stackB || stackA);");
			break;
		}
		case OperationType::DUP: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stack_push(_stack, stackA);");
			break;
		}
		case OperationType::SWAP: {
			cm.add_line("stackA = stak_pop(_stack);");
			cm.add_line("stackB = stak_pop(_stack);");
			cm.add_line("stack_push(_stack, stackA);");
			cm.add_line("stack_push(_stack, stackB);");
			break;
		}
		case OperationType::POP: {
			cm.add_line("stack_pop(_stack);");
			break;
		}
		case OperationType::ADD: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, stackB + stackA);");
			break;
		}
		case OperationType::SUB: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, stackB - stackA);");
			break;
		}
		case OperationType::MUL: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, stackB * stackA);");
			break;
		}
		case OperationType::DIV: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, stackB / stackA);");
			break;
		}
		case OperationType::MOD: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, stackB % stackA);");
			break;
		}
		case OperationType::PRINTLN: {
			cm.add_line("printf(\"%d\\n\", stack_getback(_stack, 1));");
			break;
		}
		case OperationType::PRINTLNS: {
			cm.add_line("printf(\"%c\\n\", stack_getback(_stack, 1));");
			break;
		}
		case OperationType::PRINTLNNS: {
			cm.add_line("printf(\"%c\\n\", stack_getback(_stack, stack_getback(_stack, 1)));");
			break;
		}
		case OperationType::PRINTLNN: {
			cm.add_line("printf(\"%d\\n\", stack_getback(_stack, stack_getback(_stack, 1)));");
			break;
		}
		case OperationType::PRINT: {
			cm.add_line("printf(\"%d\", stack_getback(_stack, 1));");
			break;
		}
		case OperationType::PRINTS: {
			cm.add_line("printf(\"%c\", stack_getback(_stack, 1));");
			break;
		}
		case OperationType::PRINTNS: {
			cm.add_line("printf(\"%c\", stack_getback(_stack, stack_getback(_stack, 1)));");
			break;
		}
		case OperationType::PRINTN: {
			cm.add_line("printf(\"%d\", stack_getback(_stack, stack_getback(_stack, 1)));");
			break;
		}
		case OperationType::DUMP: {
			cm.add_line("printf(\"%d\\n\", stack_getback(_stack, 1));");
			break;
		}
		case OperationType::LOADLIB: {
			cm.add_line("if(_runtime_lib)");
			cm.add_line("{");
			cm.begin_block();
#ifdef _WIN32
			cm.add_line("FreeLibrary(_runtime_lib);");
#else
			cm.add_line("dlclose(_runtime_lib);");
#endif
			cm.end_block();
			cm.add_line("}");
			cm.add_line("memset(t_str, 0, MAX_ISTR);");
			cm.add_line("iterA = 0;");
			cm.add_line("while(1)");
			cm.add_line("{");
			cm.begin_block();
			cm.add_line("back = stack_pop(_stack);");
			cm.add_line("if(back == -1)");
			cm.add_line("{");
			cm.begin_block();
			cm.add_line("break;");
			cm.end_block();
			cm.add_line("}");
			cm.add_line("t_str[iterA] = (char)back;");
			cm.add_line("iterA++;");
			cm.end_block();
			cm.add_line("}");
			cm.add_line("strrev(t_str);");
#ifdef _WIN32
			cm.add_line("_runtime_lib = LoadLibraryW(to_LPCWSTR(t_str));");
			cm.add_line("if(!_runtime_lib)");
			cm.add_line("{");
			cm.begin_block();
			cm.add_line("printf(\"failed to load runtime library %s\\n\", t_str);");
			cm.add_line("exit(1);");
			cm.end_block();
			cm.add_line("}");
#else
#endif
			break;
		}
		case OperationType::DUMPS: {
			cm.add_line("printf(\"%c\", stack_getback(_stack, 1));");
			break;
		}
		case OperationType::DUMPN: {
			cm.add_line("printf(\"%d\", stack_getback(_stack, stack_getback(_stack, 1)));");
			break;
		}
		case OperationType::DUMPNS: {
			cm.add_line("printf(\"%c\", stack_getback(_stack, stack_getback(_stack, 1)));");
			break;
		}
		case OperationType::PUTS: {
			cm.add_line("length = stack_pop(_stack);");
			cm.add_line("while(length > 0)");
			cm.add_line("{");
			cm.begin_block();
			cm.add_line("printf(\"%c\", stack_getback(_stack, length));");
			cm.add_line("length -= 1;");
			cm.end_block();
			cm.add_line("}");
			break;
		}
		case OperationType::PUTSLN: {
			cm.add_line("length = stack_pop(_stack);");
			cm.add_line("while(length > 0)");
			cm.add_line("{");
			cm.begin_block();
			cm.add_line("printf(\"%c\", stack_getback(_stack, length));");
			cm.add_line("length -= 1;");
			cm.end_block();
			cm.add_line("}");
			cm.add_line("printf(\"\\n\");");
			break;
		}
		case OperationType::POW: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, (int)pow(stackB, stackA));");
			break;
		}
		case OperationType::GT: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, stackB > stackA);");
			break;
		}
		case OperationType::LT: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, stackB < stackA);");
			break;
		}
		case OperationType::GE: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, stackB >= stackA);");
			break;
		}
		case OperationType::LE: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, stackB <= stackA);");
			break;
		}
		case OperationType::INPUTI: {
			cm.add_line("scanf(\"%d\", &stackA);");
			cm.add_line("stack_push(_stack, stackA);");
			break;
		}
		case OperationType::INPUTS: {
			cm.add_line("memset(t_str, 0, MAX_ISTR);");
			cm.add_line("fgets(t_str, MAX_ISTR, stdin);");
			cm.add_line("if(t_str)");
			cm.add_line("{");
			cm.begin_block();
			cm.add_line("stack_push(_stack, -1);");
			cm.add_line("length = strlen(t_str);");
			cm.add_line("for(int i = 0 ; i < length ; i++)");
			cm.add_line("{");
			cm.begin_block();
			cm.add_line("stack_push(_stack, t_str[i]);");
			cm.end_block();
			cm.add_line("}");
			cm.add_line("stack_push(_stack, length);");
			cm.end_block();
			cm.add_line("}");
			break;
		}
		case OperationType::EQ: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, stackB == stackA);");
			break;
		}
		case OperationType::NEQ: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("stackB = stack_pop(_stack);");

			cm.add_line("stack_push(_stack, stackB != stackA);");
			break;
		}
		case OperationType::CALL: {
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("call_proc(stackA);");
			break;
		}
		case OperationType::NCALL: {
			cm.add_line("memset(t_str, 0, MAX_ISTR);");
			cm.add_line("iterA = 0;");
			cm.add_line("while(1)");
			cm.add_line("{");
			cm.begin_block();
			cm.add_line("back = stack_pop(_stack);");
			cm.add_line("if(back == -1)");
			cm.add_line("{");
			cm.begin_block();
			cm.add_line("break;");
			cm.end_block();
			cm.add_line("}");
			cm.add_line("t_str[iterA] = (char)back;");
			cm.add_line("iterA++;");
			cm.end_block();
			cm.add_line("}");
			cm.add_line("strrev(t_str);");
			cm.add_line("func = get_runtimelib_proc(_runtime_lib, t_str);");
			cm.add_line("if(func)");
			cm.add_line("{");
			cm.begin_block();
			cm.add_line("func((*_stack).data, (int)((*_stack).cptr - (*_stack).data));");
			cm.end_block();
			cm.add_line("}");
			cm.add_line("else");
			cm.add_line("{");
			cm.begin_block();
			cm.add_line("printf(\"failed to load native functon %s\", t_str);");
			cm.add_line("exit(-1);");
			cm.end_block();
			cm.add_line("}");
			break;
		}
		case OperationType::RECEED: {
			cm.add_line("// RECEED : Operation Not Supported by transpiler");
			break;
		}
		case OperationType::PROCEED: {
			cm.add_line("// PROCEED : Operation Not Supported by transpiler");
			break;
		}
		case OperationType::MALLOC: {
			cm.add_line("if(_memory)");
			cm.add_line("{");
			cm.begin_block();
			cm.add_line("free(_memory);");
			cm.end_block();
			cm.add_line("}");
			cm.add_line("stackA = stack_pop(_stack);");
			cm.add_line("_memory = malloc(stackA * sizeof(int));");
			break;
		}
		case OperationType::MEMSET: {
			if (op.arg == -1) {
				cm.add_line("memAddr = stack_pop(_stack);");
				cm.add_line("memSize = stack_pop(_stack);");
				cm.add_line("iterA = 0;");
				cm.add_line("while(iterA < memSize)");
				cm.add_line("{");
				cm.begin_block();
				cm.add_line("*(_memory + memAddr + iterA) = stack_pop(_stack);");
				cm.add_line("iterA += 1;");
				cm.end_block();
				cm.add_line("}");
			} else {
				int addr = op.arg;
				cm.add_line("*(_memory + " + std::to_string(addr) + ") = stack_pop(_stack);");
			}
			break;
		}
		case OperationType::MEMGET: {
			if (op.arg == -1) {
				cm.add_line("memAddr = stack_pop(_stack);");
				cm.add_line("memSize = stack_pop(_stack);");
				cm.add_line("iterA = memSize - 1;");
				cm.add_line("while(iterA >= 0)");
				cm.add_line("{");
				cm.begin_block();
				cm.add_line("stack_push(_stack, *(_memory + memAddr + iterA));");
				cm.add_line("iterA -= 1;");
				cm.end_block();
				cm.add_line("}");
			} else {
				int addr = op.arg;
				cm.add_line("stack_push(_stack, *(_memory + " + std::to_string(addr) + "));");
			}
			break;
		}
		case OperationType::IF: {
			if (op.arg == 0) {
				cm.add_line("if(stack_pop(_stack))");
				cm.add_line("{");
				cm.begin_block();
			} else if (op.arg == 1) {
				cm.end_block();
				cm.add_line("}");
			}
			break;
		}
		case OperationType::WHILE: {
			if (op.arg == 0) {
				cm.add_line("while(stack_pop(_stack))");
				cm.add_line("{");
				cm.begin_block();
			} else if (op.arg == 1) {
				cm.end_block();
				cm.add_line("}");
			}
			break;
		}
		case OperationType::FOR: {
			if (op.arg != -1) {
				if (op.arg == -2) {
					cm.add_line("count = stack_pop(_stack);");
				} else {
					cm.add_line("count = " + std::to_string(op.arg) + ";");
				}
				cm.add_line("for(int i = 0 ; i < count ; i++)");
				cm.add_line("{");
				cm.begin_block();
			} else {
				cm.end_block();
				cm.add_line("}");
			}
			break;
		}
		case OperationType::EXIT: {
			cm.add_line("exit(0);");
		}
		case OperationType::RET: {
			cm.add_line("return;");
		}
	}
}

static std::unordered_map<int, ProcAddr> procAddresses;

static void loadProcs(std::vector<Operation> ops) {
	procAddresses.clear();
	ProcAddr pAddr;
	bool	 inProc = false;
	for (int i = 0; i < ops.size(); i++) {
		if (inProc && ops[i].op == OperationType::PROC) {
			if (ops[i].arg == -1) {
				pAddr.eAddr					= i;
				inProc						= false;
				procAddresses[pAddr.procId] = pAddr;
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

std::string tovie2c(std::vector<Operation> ops) {
	loadProcs(ops);
	CodeMaker cm;
	cm.add_line("// This code has been generated by " + get_name());

	// include files
	cm.add_line("#include <stdio.h>");
	cm.add_line("#include <stdlib.h>");
	cm.add_line("#include <string.h>");
	cm.add_line("#include <stdint.h>");

#ifdef _WIN32
	cm.add_line("#include <windows.h>");
#endif
#ifdef __unix__
	cm.add_line("#include <unistd.h>");
	cm.add_line("#include <dlfcn.h>");
#endif

	cm.add_line("");
	cm.add_line("");
	cm.add_line("");


	cm.add_line("#define MAX_ISTR 4096");

	cm.add_line("");
	cm.add_line("");
	cm.add_line("");

	// the c stack
	cm.add_line(cSack);

	cm.add_line("");
	cm.add_line("");

	// dll loader
	cm.add_line(dllLoader);

	// globals
	cm.add_line("struct stack* _stack;");

#ifdef _WIN32
	cm.add_line("HINSTANCE _runtime_lib;");
#else
	cm.add_line("void* _runtime_lib;");
#endif

	cm.add_line("");
	cm.add_line("");

	// proc_defs
	for (auto& kv : procAddresses) {
		cm.add_line("void proc_" + std::to_string(kv.first) + "();");
	}

	cm.add_line("");
	cm.add_line("");


	// proc_caller
	cm.add_line("void call_proc(int n)");
	cm.add_line("{");
	cm.begin_block();
	cm.add_line("switch(n)");
	cm.add_line("{");
	cm.begin_block();
	for (auto& kv : procAddresses) {
		cm.add_line("case " + std::to_string(kv.first) + ": proc_" + std::to_string(kv.first) + "(); break;");
	}
	cm.add_line("default : printf(\"proc %d not found error!\", n); exit(-1);");
	cm.end_block();
	cm.add_line("}");
	cm.end_block();
	cm.add_line("}");


	for (auto& kv : procAddresses) {
		cm.add_line("// PROC " + std::to_string(kv.first));
		cm.add_line("void proc_" + std::to_string(kv.first) + "()");
		cm.add_line("{");
		cm.begin_block();
		cm.add_line("int* _memory = malloc(1024 * sizeof(int));");
		cm.add_line("// declare variables");
		cm.add_line("int stackA, stackB, memAddr, memSize, length, iterStart, iterA, iterB, count, back;");
		cm.add_line("char t_str[MAX_ISTR];");
		cm.add_line("TovieNativeFunc func;");
		for (int i = kv.second.bAddr + 1; i < kv.second.eAddr; i++) {
			Operation op = ops[i];
			cm.add_line("// OP " + std::to_string(i) + " [ " + to_string(op.op) + " " + std::to_string(op.arg) + " ]");
			transpile(cm, op);
		}
		cm.add_line("if(_memory)");
		cm.add_line("{");
		cm.begin_block();
		cm.add_line("free(_memory);");
		cm.end_block();
		cm.add_line("}");
		cm.end_block();
		cm.add_line("}");
		cm.add_line("");
		cm.add_line("");
	}

	// calling main part
	cm.add_line("int main(int argc, char** argv)");
	cm.add_line("{");
	cm.begin_block();
	cm.add_line("_stack = stack_make(1024, 1.5f);");
	cm.add_line("proc_0();");
	cm.add_line("stack_free(_stack);");
	cm.add_line("if(_runtime_lib)");
	cm.add_line("{");
	cm.begin_block();
#ifdef _WIN32
	cm.add_line("FreeLibrary(_runtime_lib);");
#else
	cm.add_line("dlclose(_runtime_lib);");
#endif
	cm.end_block();
	cm.add_line("}");
	cm.end_block();
	cm.add_line("}");
	return cm.render();
}
