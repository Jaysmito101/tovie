#include "python_compiler.hpp"

#include "codemaker.hpp"
#include "info.hpp"
#include "operations.hpp"

#include <stdexcept>
#include <unordered_map>

struct ProcAddr {
	int			  procId;
	unsigned long bAddr, eAddr;

	ProcAddr();

	ProcAddr(int procId, unsigned long bAddr, unsigned long eAddr);
};

void transpile(CodeMaker& cm, Operation op) {
	switch (op.op) {
		case OperationType::PUSH: {
			cm.add_line("_stack.append(" + std::to_string(op.arg) + ")");
			break;
		}
		case OperationType::AND: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(stackB and stackA)");
			break;
		}
		case OperationType::DUP: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("_stack.append(stackA)");
			cm.add_line("_stack.append(stackA)");
			break;
		}
		case OperationType::SWAP: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");
			cm.add_line("_stack.append(stackA)");
			cm.add_line("_stack.append(stackB)");
			break;
		}
		case OperationType::OR: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(stackB or stackA)");
			break;
		}
		case OperationType::POP: {
			cm.add_line("_ = _stack.pop()");
			break;
		}
		case OperationType::ADD: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(stackB + stackA)");
			break;
		}
		case OperationType::SUB: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(stackB - stackA)");
			break;
		}
		case OperationType::MUL: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(stackB * stackA)");
			break;
		}
		case OperationType::DIV: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(stackB // stackA)");
			break;
		}
		case OperationType::MOD: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(stackB % stackA)");
			break;
		}
		case OperationType::PRINTLN: {
			cm.add_line("print(_stack[len(_stack) - 1])");
			break;
		}
		case OperationType::PRINTLNS: {
			cm.add_line("print(chr(_stack[len(_stack) - 1]))");
			break;
		}
		case OperationType::PRINTLNNS: {
			cm.add_line("print(chr(_stack[len(_stack) - _stack[len(_stack) - 1]]))");
			break;
		}
		case OperationType::PRINTLNN: {
			cm.add_line("print(_stack[len(_stack) - _stack[len(_stack) - 1]])");
			break;
		}
		case OperationType::PRINT: {
			cm.add_line("print(_stack[len(_stack) - 1]), end=\"\")");
			break;
		}
		case OperationType::PRINTS: {
			cm.add_line("print(chr(_stack[len(_stack) - 1])), end=\"\")");
			break;
		}
		case OperationType::PRINTNS: {
			cm.add_line("print(chr(_stack[len(_stack) - _stack[len(_stack) - 1]])), end=\"\")");
			break;
		}
		case OperationType::PRINTN: {
			cm.add_line("print(_stack[len(_stack) - _stack[len(_stack) - 1]]), end=\"\")");
			break;
		}
		case OperationType::DUMP: {
			cm.add_line("print(_stack[len(_stack) - 1])");
			break;
		}
		case OperationType::LOADLIB: {
			cm.add_line("libName = \"\"");
			cm.add_line("back = 0");
			cm.add_line("while True:");
			cm.begin_block();
			cm.add_line("back = _stack.pop()");
			cm.add_line("if back == -1:");
			cm.begin_block();
			cm.add_line("break");
			cm.end_block();
			cm.add_line("libName += chr(back)");
			cm.end_block();
			cm.add_line("libName = libName[::-1]");
			cm.add_line("_runtime_lib = ctypes.cdll.LoadLibrary(\"./\" + libName)");
		}
		case OperationType::DUMPS: {
			cm.add_line("print(chr(_stack[len(_stack) - 1]))");
			break;
		}
		case OperationType::DUMPN: {
			cm.add_line("print(_stack[len(_stack) - _stack[len(_stack) - 1]])");
			break;
		}
		case OperationType::DUMPNS: {
			cm.add_line("print(chr(_stack[len(_stack) - _stack[len(_stack) - 1]]))");
			break;
		}
		case OperationType::PUTS: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("iterStart = len(_stack) - stackA");
			cm.add_line("while iterStart < len(_stack):");
			cm.begin_block();
			cm.add_line("print(chr(_stack[iterStart]), end=\"\")");
			cm.add_line("iterStart += 1");
			cm.end_block();
			break;
		}
		case OperationType::PUTSLN: {
			cm.add_line("# PUTSLN");
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("iterStart = len(_stack) - stackA");
			cm.add_line("while iterStart < len(_stack):");
			cm.begin_block();
			cm.add_line("print(chr(_stack[iterStart]), end=\"\")");
			cm.add_line("iterStart += 1");
			cm.end_block();
			cm.add_line("print()");
			break;
		}
		case OperationType::POW: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(stackB ** stackA)");
			break;
		}
		case OperationType::GT: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(int(stackB > stackA))");
			break;
		}
		case OperationType::LT: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(int(stackB < stackA))");
			break;
		}
		case OperationType::GE: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(int(stackB >= stackA))");
			break;
		}
		case OperationType::LE: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(int(stackB <= stackA))");
			break;
		}
		case OperationType::INPUTI: {
			cm.add_line("_stack.append(int(input(\"\")))");
			break;
		}
		case OperationType::INPUTS: {
			cm.add_line("tStr = input(\"\")");
			cm.add_line("_stack.append(-1)");
			cm.add_line("for tChr in tStr:");
			cm.begin_block();
			cm.add_line("_stack.append(ord(tChr))");
			cm.end_block();
			cm.add_line("_stack.append(len(tStr))");
			break;
		}
		case OperationType::EQ: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(int(stackB == stackA))");
			break;
		}
		case OperationType::NEQ: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("stackB = _stack.pop()");

			cm.add_line("_stack.append(int(stackB != stackA))");
			break;
		}
		case OperationType::CALL: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("eval(\"proc_\" + str(stackA) + \"()\")");
			break;
		}
		case OperationType::NCALL: {
			cm.add_line("funcName = \"\"");
			cm.add_line("back = 0");
			cm.add_line("while True:");
			cm.begin_block();
			cm.add_line("back = _stack.pop()");
			cm.add_line("if back == -1:");
			cm.begin_block();
			cm.add_line("break");
			cm.end_block();
			cm.add_line("funcName += chr(back)");
			cm.end_block();
			cm.add_line("funcName = funcName[::-1]");
			cm.add_line("eval(\"_runtime_lib.\" + funcName + \"_stack\")");
			break;
		}
		case OperationType::RECEED: {
			cm.add_line("# RECEED : Operation Not Supported by transpiler");
			break;
		}
		case OperationType::PROCEED: {
			cm.add_line("# PROCEED : Operation Not Supported by transpiler");
			break;
		}
		case OperationType::MALLOC: {
			cm.add_line("stackA = _stack.pop()");
			cm.add_line("_memory = [0] * stackA");
			break;
		}
		case OperationType::MEMSET: {
			if (op.arg == -1) {
				cm.add_line("memAddr = _stack.pop()");
				cm.add_line("memSize = _stack.pop()");
				cm.add_line("iterA = 0");
				cm.add_line("while iterA < memSize:");
				cm.begin_block();
				cm.add_line("_memory[memAddr + iterA] = _stack.pop()");
				cm.add_line("iterA += 1");
				cm.end_block();
			} else {
				int addr = op.arg;
				cm.add_line("_memory[" + std::to_string(addr) + "] = _stack.pop()");
			}
			break;
		}
		case OperationType::MEMGET: {
			if (op.arg == -1) {
				cm.add_line("memAddr = _stack.pop()");
				cm.add_line("memSize = _stack.pop()");
				cm.add_line("iterA = memSize - 1");
				cm.add_line("while iterA >= 0:");
				cm.begin_block();
				cm.add_line("_stack.append(_memory[memAddr + iterA])");
				cm.add_line("iterA -= 1");
				cm.end_block();
			} else {
				int addr = op.arg;
				cm.add_line("_stack.append(_memory[" + std::to_string(addr) + "])");
			}
			break;
		}
		case OperationType::IF: {
			if (op.arg == 0) {
				cm.add_line("if _stack.pop():");
				cm.begin_block();
			} else if (op.arg == 1) {
				cm.end_block();
			}
			break;
		}
		case OperationType::WHILE: {
			if (op.arg == 0) {
				cm.add_line("while _stack.pop():");
				cm.begin_block();
			} else if (op.arg == 1) {
				cm.end_block();
			}
			break;
		}
		case OperationType::FOR: {
			if (op.arg != -1) {
				cm.add_line("count = " + std::to_string(op.arg));
				if (op.arg == -2) {
					cm.add_line("count = _stack.pop()");
				}
				cm.add_line("for iter in range(0, count):");
				cm.begin_block();
			} else {
				cm.end_block();
			}
			break;
		}
		case OperationType::EXIT: {
			cm.add_line("exit()");
		}
		case OperationType::RET: {
			cm.add_line("return");
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

std::string tovie2py(std::vector<Operation> ops) {
	loadProcs(ops);
	CodeMaker cm;
	cm.add_line("# This code has been generated by " + get_name());

	// import modules
	cm.add_line("import sys");
	cm.add_line("import os");
	cm.add_line("import ctypes");

	// globals
	cm.add_line("_stack = [0]");
	cm.add_line("_runtime_lib = \"\"");

	for (auto& kv : procAddresses) {
		cm.add_line("# PROC " + std::to_string(kv.first));
		cm.add_line("def proc_" + std::to_string(kv.first) + "():");
		cm.begin_block();
		cm.add_line("global _stack");
		cm.add_line("global _runtime_lib");
		cm.add_line("_memory = [0] * 1024");
		for (int i = kv.second.bAddr + 1; i < kv.second.eAddr; i++) {
			Operation op = ops[i];
			cm.add_line("# OP " + std::to_string(i) + " [ " + to_string(op.op) + " " + std::to_string(op.arg) + " ]");
			transpile(cm, op);
		}
		cm.end_block();
		cm.add_line("");
		cm.add_line("");
	}

	// calling main part
	cm.add_line("if __name__ == \"__main__\":");
	cm.begin_block();
	cm.add_line("proc_0()");
	cm.end_block();

	return cm.render();
}
