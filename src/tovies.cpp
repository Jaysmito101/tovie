#include "tovies.h"
#include <iostream>
#include <stack>

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
        std::cout << " pop " << std::endl;
    s.pop_back();
}

void add(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    s.push_back(a);
    s.push_back(b);
    if(debug)
        std::cout << " [DEBUG]\t" << b << " + " << a << std::endl;
    s.push_back(b + a);
}

void sub(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    s.push_back(a);
    s.push_back(b);
    if(debug)
        std::cout << " [DEBUG]\t" << b << " - " << a << std::endl;
    s.push_back(b - a);
}

void mul(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    s.push_back(a);
    s.push_back(b);
    if(debug)
        std::cout << " [DEBUG]\t" << b << " * " << a << std::endl;
    s.push_back(b * a);
}

void div(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    s.push_back(a);
    s.push_back(b);
    if(debug)
        std::cout << " [DEBUG]\t" << b << " / " << a << std::endl;
    s.push_back(b / a);
}

void mod(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    s.push_back(a);
    s.push_back(b);
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
    std::cout << a << std::endl;
}

void dumpns(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s[(int)s.size() - 1 - s.back()];
    std::cout << a << std::endl;
}

void dumpn(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s[(int)s.size() - 1 - s.back()];
    std::cout << a << std::endl;
}

void println(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    std::cout << (char)a << std::endl;
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
    std::cout << (char)a << std::endl;
}

void pow(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    s.push_back(a);
    s.push_back(b);
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
    std::cout << (char)a;
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
    std::cout << (char)a;
}

void puts(std::vector<int> &s, Operation op, bool debug = false) {
    int l = s.back();
    int i = s.size() - l - 2;
    if(debug){
        std::cout << " [DEBUG]\t puts" << std::endl;
        std::cout << " [DEBUG]\t\t" << " length : " << l << std::endl;
        std::cout << " [DEBUG]\t\t" << " begin : " << i << std::endl;
    }
    for(int j=i+1;j<s.size() -1;j++)
        std::cout << (char)s[j];
}

void putsln(std::vector<int> &s, Operation op, bool debug = false) {
    int l = s.back();
    int i = s.size() - l - 2;
    if(debug){
        std::cout << " [DEBUG]\t puts" << std::endl;
        std::cout << " [DEBUG]\t\t" << " length : " << l << std::endl;
        std::cout << " [DEBUG]\t\t" << " begin : " << i << std::endl;
    }
    for(int j=i+1;j<s.size() -1;j++)
        std::cout << (char)s[j];
    std::cout << std::endl;
}

void gt(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    s.push_back(a);
    s.push_back(b);
    if(debug)
        std::cout << " [DEBUG]\t" << b << " > " << a << std::endl;
    s.push_back(b > a);
}

void lt(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    s.push_back(a);
    s.push_back(b);
    if(debug)
        std::cout << " [DEBUG]\t" << b << " < " << a << std::endl;
    s.push_back(b < a);
}

void ge(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    s.push_back(a);
    s.push_back(b);
    if(debug)
        std::cout << " [DEBUG]\t" << b << " >= " << a << std::endl;
    s.push_back(b >= a);
}

void le(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    s.push_back(a);
    s.push_back(b);
    if(debug)
        std::cout << " [DEBUG]\t" << b << " <= " << a << std::endl;
    s.push_back(b <= a);
}

void eq(std::vector<int> &s, Operation op, bool debug = false) {
    int a = s.back();
    s.pop_back();
    int b = s.back();
    s.pop_back();
    s.push_back(a);
    s.push_back(b);
    if(debug)
        std::cout << " [DEBUG]\t" << a << " == " << b << std::endl;
    s.push_back(a == b);
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
    std::cin >> ss;
    for(int i=0;i<ss.size();i++)
        s.push_back(ss[i]);
    s.push_back(ss.size());
}

void simulate(std::vector<Operation> ops, bool debug){
    std::vector<int> progStack;
    for(int i=0;i<ops.size();i++){
        Operation op = ops[i];
        if(debug && false) // TMP
            std::cout << " [DEBUG]\t" << op.op << "\t\t" << op.arg << std::endl;
        if(debug){
            std::cout  << " [DEBUG] STACK :";
            print(progStack);
        }
        switch(op.op){
            case OperationType::PUSH:
                push(progStack, op, debug);
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
            case OperationType::RECEED:
            {
                int count = progStack.back();
                progStack.pop_back();
                if(progStack.back())
                    i -= count + 2;
                if(debug && progStack.back())
                    std::cout << " [DEBUG]\t receed " << count << std::endl;
                progStack.push_back(count);
                break;
            }
            case OperationType::PROCEED:
            {
                int count = progStack.back();
                progStack.pop_back();
                if(progStack.back())
                    i += count-1;
                if(debug && progStack.back())
                    std::cout << " [DEBUG]\t porceed " << count << std::endl;
                progStack.push_back(count);
                break;
            }
            case OperationType::EXIT:
                return;
        }
    }
}
