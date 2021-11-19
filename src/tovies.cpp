#include "tovies.h"
#include <iostream>
#include <functional>
#include <stack>
#include <unordered_map>
#include <algorithm>

#include "tovie_runtimelib.h"

struct ProcAddr
{
    int procId;
    unsigned long bAddr, eAddr;

    ProcAddr()
    {
        procId = -1;
        bAddr = eAddr = 0;
    }

    ProcAddr(int procId, unsigned long bAddr, unsigned long eAddr)
    {
        this->procId = procId;
        this->bAddr = bAddr;
        this->eAddr = eAddr;
    }
};

// The `#if 1` is just to use code folding in the Editor.
#if 1
void print(std::vector<int> vec){
    std::cout << "[ ";
    for (int i: vec)
        std::cout << i << ' ';
    std::cout << "]" << std::endl;
}

void push(std::vector<int> &s, Operation op, bool debug = false) {
    if(debug)
        std::cout << " [DEBUG]\t push " << op.arg << std::endl;
    s.push_back(op.arg);
}

void pop(std::vector<int> &s, Operation op, bool debug = false) {
    if(debug)
        std::cout << " [DEBUG]\t pop " << std::endl;
    s.pop_back();
}


void andop(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    if(debug)
        std::cout << " [DEBUG]\t" << b << " && " << a << std::endl;
    s.push_back(b && a);
}

void orop(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    if(debug)
        std::cout << " [DEBUG]\t" << b << " || " << a << std::endl;
    s.push_back(b || a);
}


void add(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    if(debug)
        std::cout << " [DEBUG]\t" << b << " + " << a << std::endl;
    s.push_back(b + a);
}

void sub(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    if(debug)
        std::cout << " [DEBUG]\t" << b << " - " << a << std::endl;
    s.push_back(b - a);
}

void mul(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    if(debug)
        std::cout << " [DEBUG]\t" << b << " * " << a << std::endl;
    s.push_back(b * a);
}

void div(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    if(debug)
        std::cout << " [DEBUG]\t" << b << " / " << a << std::endl;
    s.push_back(b / a);
}

void mod(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    if(debug)
        std::cout << " [DEBUG]\t" << b << " % " << a << std::endl;
    s.push_back(b % a);
}

void dump(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    std::cout << a << std::endl;
}

void dumps(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    std::cout << (char)a << std::endl;
}

void dumpns(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s[(int)s.size() - 1 - s.back()];
    std::cout << (char)a << std::endl;
}

void dumpn(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s[(int)s.size() - 1 - s.back()];
    std::cout << a << std::endl;
}

void println(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    std::cout << a << std::endl;
}

void printlns(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    std::cout << (char)a << std::endl;
}

void printlnns(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s[(int)s.size() - 1 - s.back()];
    std::cout << (char)a << std::endl;
}

void printlnn(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s[(int)s.size() - 1 - s.back()];
    std::cout << a << std::endl;
}

void pow(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    int r = 1;
    for(int i=0;i<a;i++) {
        r *= b;
    }
    if(debug)
        std::cout << b << " ^ " << a << std::endl;
    s.push_back(r);
}

void print(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    std::cout << a;
}

void prints(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    std::cout << (char)a;
}

void printns(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s[(int)s.size() - 1 - s.back()];
    std::cout << (char)a;
}

void printn(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s[(int)s.size() - 1 - s.back()];
    std::cout << a;
}

void puts(std::vector<int> &s, Operation op, bool debug = false) {
    int l = s.back();
    s.pop_back();
    int i = s.size() - l;
    if(debug){
        std::cout << " [DEBUG]\t puts" << std::endl;
        std::cout << " [DEBUG]\t\t" << " length : " << l << std::endl;
        std::cout << " [DEBUG]\t\t" << " begin : " << i << std::endl;
    }
    for(int j=i;j<s.size();j++)
        std::cout << (char)s[j];
}

void putsln(std::vector<int> &s, Operation op, bool debug = false) {
    int l = s.back();
    s.pop_back();
    int i = s.size() - l;
    if(debug){
        std::cout << " [DEBUG]\t putsln" << std::endl;
        std::cout << " [DEBUG]\t\t" << " length : " << l << std::endl;
        std::cout << " [DEBUG]\t\t" << " begin : " << i << std::endl;
    }
    for(int j=i;j<s.size();j++)
        std::cout << (char)s[j];
    std::cout << std::endl;
}

void gt(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    if(debug)
        std::cout << " [DEBUG]\t" << b << " > " << a << std::endl;
    s.push_back(b > a);
}

void lt(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    if(debug)
        std::cout << " [DEBUG]\t" << b << " < " << a << std::endl;
    s.push_back(b < a);
}

void ge(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    if(debug)
        std::cout << " [DEBUG]\t" << b << " >= " << a << std::endl;
    s.push_back(b >= a);
}

void le(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    if(debug)
        std::cout << " [DEBUG]\t" << b << " <= " << a << std::endl;
    s.push_back(b <= a);
}

void eq(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    if(debug)
        std::cout << " [DEBUG]\t" << a << " == " << b << std::endl;
    s.push_back(a == b);
}

void neq(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    if(debug)
        std::cout << " [DEBUG]\t" << a << " != " << b << std::endl;
    s.push_back(a != b);
}

void inputi(std::vector<int> &s, Operation op, bool debug = false){
    if(debug)
        std::cout << " [DEBUG]\tinput integer" << std::endl;
    int n = 0;
    std::cin >> n;
    s.push_back(n);
}

void inputs(std::vector<int> &s, Operation op, bool debug = false){
    if(debug)
        std::cout << " [DEBUG]\tinput string" << std::endl;
    std::string ss = "";
    std::getline(std::cin, ss);
    s.push_back(-1);
    for(int i=0;i<ss.size();i++)
        s.push_back(ss[i]);
    s.push_back(ss.size());
}
#endif

// reverse string
static void reverse(std::string& str){
    std::reverse(str.begin(), str.end());
}

static std::unordered_map<int, ProcAddr> procAddresses;
static void* runtimeLib = nullptr;
static std::unordered_map<std::string, std::function<void(int*, int)>> libProcs;

static void loadLibProc(std::vector<int>& progStack, Operation op, bool debug){
    std::string libPath = "";
    int back = 0;
    while(true){
        back = progStack.back();
        progStack.pop_back();
        if(back == -1)
            break;
        libPath += (char)back;
    }
    reverse(libPath);
    if(debug)
        std::cout << " [DEBUG]\t load library : " << libPath << std::endl;
    if(runtimeLib)
        close_runtime_lib(runtimeLib);
    libProcs.clear();
    runtimeLib = open_runtime_lib(libPath.c_str());
}


static void callLibProc(std::vector<int>& progStack, Operation op, bool debug = false){
    std::string libProcName = "";
    int back = 0;
    while(true){
        back = progStack.back();
        progStack.pop_back();
        if(back == -1)
            break;
        libProcName += (char)back;
    }
    reverse(libProcName);
    if(debug)
        std::cout << " [DEBUG]\t call library procedure : " << libProcName << std::endl;
    // Find Key libProcName exists in libProcs or not
    if(libProcs.find(libProcName) == libProcs.end()){
        libProcs[libProcName] = get_runtimelib_proc(runtimeLib, libProcName.c_str());
    }
    libProcs[libProcName](progStack.data(), progStack.size());
}

static void loadProcs(std::vector<Operation> ops){
    procAddresses.clear();
    ProcAddr pAddr;
    bool inProc = false;
    for(int i=0;i<ops.size();i++){
        if(inProc && ops[i].op == OperationType::PROC){
            if(ops[i].arg == -1){
                pAddr.eAddr = i;
                inProc = false;
                procAddresses[pAddr.procId] = pAddr;
            }
            else{
                throw std::runtime_error("proc" + std::to_string(pAddr.procId) + " begin inside another proc error!");
            }
        }
        else if(ops[i].op == OperationType::PROC){
            if(ops[i].arg > -1){
                pAddr.procId = ops[i].arg;
                pAddr.bAddr = i;
                inProc = true;
            }
            else{
                throw std::runtime_error("proc end without begin error!");
            }
        }
    }
}


static int find_next_end(std::vector<Operation> ops, int i, OperationType type, int endArg = 1){
    for(int j=i;j<ops.size();j++){
        if(ops[j].op == type && ops[j].arg == endArg){
            return j;
        }
    }
    return ops.size();
}

static int find_prev_begin(std::vector<Operation> ops, int i, OperationType type, int begArg = 0){
    for(int j=i;j>=0;j--){
        if(ops[j].op == type && ops[j].arg == begArg){
            return j;
        }
    }
    return ops.size();
}

static void simulate_proc(std::vector<int>& progStack, std::vector<Operation> ops, ProcAddr pAddr, bool debug);

static void simulate_op(std::vector<int> &progStack, Operation op, unsigned long* i, int* memory, std::vector<Operation>& ops,bool debug = false){
    if(debug && false) // TMP
        std::cout << " [DEBUG]\t" << op.op << "\t\t" << op.arg << std::endl;
    if(debug){
        std::cout  << " [DEBUG] STACK :";
        print(progStack);
        std::cout  << " [DEBUG] OP ID :" << *i << std::endl;
    }
    int a, b;
     switch(op.op){
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
            case OperationType::CALL:
            {
                int procId = progStack.back();
                progStack.pop_back();
                if(debug)
                    std::cout << " [DEBUG]\tcall proc_" << procId << std::endl;
                if(procAddresses.find(procId) == procAddresses.end()){
                    throw std::runtime_error("proc_" + std::to_string(procId) + " not found!");
                }
                simulate_proc(progStack, ops, procAddresses[procId], debug);
                break;
            }
            case OperationType::NCALL:
            {
                callLibProc(progStack, op, debug);
                break;
            }
            case OperationType::RECEED:
            {
                int count = progStack.back();
                progStack.pop_back();
                if(progStack.back())
                    *i -= count + 2;
                if(debug && progStack.back())
                    std::cout << " [DEBUG]\t receed " << count << std::endl;
                break;
            }
            case OperationType::PROCEED:
            {
                int count = progStack.back();
                progStack.pop_back();
                if(progStack.back())
                    *i += count-1;
                if(debug && progStack.back())
                    std::cout << " [DEBUG]\t porceed " << count << std::endl;
                break;
            }
            case OperationType::MALLOC:
            {
                int count = progStack.back();
                progStack.pop_back();
                if(debug)
                    std::cout << " [DEBUG]\t malloc " << count << std::endl;
                if(memory != nullptr)
                    delete[] memory;
                memory = new int[count];
                break;
            }
            case OperationType::MEMGET:
            {
                if(op.arg == -1){
                    int addr = progStack.back();
                    progStack.pop_back();
                    int size = progStack.back();
                    progStack.pop_back();
                    if(debug)
                        std::cout << " [DEBUG]\t memget " << addr << size << std::endl;
                    for(int k = addr + size - 1 ; k >= addr ; k--)
                        progStack.push_back(memory[k]);
                }
                else{
                    int addr = op.arg;
                    if(debug)
                        std::cout << " [DEBUG]\t memget " << addr << std::endl;
                    progStack.push_back(memory[addr]);
                }
                break;
            }
            case OperationType::MEMSET:
            {
                if(op.arg == -1){
                    int addr = progStack.back();
                    progStack.pop_back();
                    int size = progStack.back();
                    progStack.pop_back();
                    if(debug)
                        std::cout << " [DEBUG]\t memset " << addr << size << std::endl;
                    for(int k = addr ; k < addr + size ; k++){
                        memory[k] = progStack.back();
                        progStack.pop_back();
                    }
                }
                else{
                    int addr = op.arg;
                    if(debug)
                        std::cout << " [DEBUG]\t memset " << addr << std::endl;
                    memory[addr] = progStack.back();
                    progStack.pop_back();
                }
                break;
            }
            case OperationType::IF:
            {
                if(op.arg == 0){
                    int condition = progStack.back();
                    progStack.pop_back();
                    if(debug)
                        std::cout << " [DEBUG]\t if " << condition << std::endl;
                    if(condition)
                    {
                        // For future use
                    }
                    else{
                        *i = find_next_end(ops, *i, OperationType::IF);
                        if(debug)
                            std::cout << " [DEBUG]\t skipping to op " << (i+1) << std::endl;
                    }
                }
                else if(op.arg == 1){
                    // For future use
                }
                break;
            }
            case OperationType::WHILE:
            {
                if(op.arg == 0){
                    int condition = progStack.back();
                    progStack.pop_back();
                    if(condition){
                       // For future use 
                       if(debug)
                            std::cout << " [DEBUG]\t while " << condition << std::endl;
                    }
                    else{
                        if(debug)
                            std::cout << " [DEBUG]\t skipping to op " << (i+1) << std::endl;
                        *i = find_next_end(ops, *i, OperationType::WHILE);
                    }
                }
                else if(op.arg == 1){
                    *i = find_prev_begin(ops, *i, OperationType::WHILE) - 1;
                    if(debug){
                        std::cout << " [DEBUG]\t moving to op " << (i+1) << std::endl;
                    }
                }
                break;
            }
            case OperationType::FOR:
            {
                if(op.arg != -1){
                    int end = find_next_end(ops, *i, OperationType::FOR, -1);
                    int count = op.arg;
                    if(op.arg == -2){
                        count = progStack.back();
                        progStack.pop_back();
                    }
                    if(debug){
                        std::cout << " [DEBUG]\t for " << count << " times from " << *i << " to " << end << std::endl;
                    }
                    for(int k = 0 ; k < count ; k++){
                        if(debug)
                            std::cout << " [DEBUG]\t for " << k+1 << "/" << count << std::endl;
                        for(unsigned long p=*i+1; p<end && p >= *i; p++)
                            simulate_op(progStack, ops[p], &p, memory, ops, debug);
                    }
                    *i = end;
                }

                break;
            }
            case OperationType::EXIT:
            {
                if(debug)
                    std::cout << " [DEBUG]\texit " << progStack.back() << std::endl;
                if(memory != nullptr)
                    delete[] memory;
                exit(progStack.back());
            }
            case OperationType::RET:
            {
                if(debug)
                    std::cout << " [DEBUG]\treturn" << std::endl;
                return;
            }
        }
}

static void simulate_proc(std::vector<int>& progStack, std::vector<Operation> ops, ProcAddr pAddr, bool debug){
    int* memory = new int[1024];
    int endIdToSkip = 0;
    for(unsigned long i = pAddr.bAddr + 1 ; i < pAddr.eAddr ; i++){
        Operation op = ops[i];
	if(op.op == OperationType::RET){
		if(debug)
                	std::cout << " [DEBUG]\treturn" << std::endl;
                return;
	}
        simulate_op(progStack, op, &i, memory, ops, debug);
    }
    if(memory)
        delete[] memory;
}

void simulate(std::vector<Operation> ops, bool debug){
    loadProcs(ops);
    runtimeLib = nullptr;
    std::vector<int> progStack;
    progStack.push_back(0);
    if(procAddresses.find(0) == procAddresses.end()){
        throw std::runtime_error("no main proc!");
    }
    simulate_proc(progStack, ops, procAddresses[0], debug);
}