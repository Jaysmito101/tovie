#define LEXPP_IMPLEMENTATION

#include "compiler.hpp"

#include "debug.hpp"
#include "lexpp.hpp"
#include "operations.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace tovie
{

#if defined(WIN32) || defined(_WIN32)
#	define PATH_SEPARATOR "\\"
#else
#	define PATH_SEPARATOR "/"
#endif

bool is_integer(std::string token) {
	for (char c : token) {
		if (!isdigit(c)) {
			return false;
		}
	}
	return true;
}

bool is_float(std::string token) {
	for (char c : token) {
		if (!isdigit(c) && c != '.') {
			return false;
		}
	}
	return true;
}

bool starts_with(const std::string& s, const std::string& prefix) {
	return s.substr(0, prefix.size()) == prefix;
}

bool ends_with(const std::string& s, const std::string& suffix) {
	return s.substr(s.size() - suffix.size(), suffix.size()) == suffix;
}

static std::string to_upper(std::string& s) {
	std::string result;
	for (char c : s) {
		result += toupper(c);
	}
	return result;
}

static std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& v) {
	os << "[";
	for (auto& s : v) {
		os << s << ", ";
	}
	os << "]";
	return os;
}

static std::ostream& operator<<(std::ostream& os, const std::unordered_map<std::string, int> map) {
	os << "{";
	for (auto& pair : map) {
		os << pair.first << ": " << pair.second << ", ";
	}
	os << "}";
	return os;
}

static void process_defs(std::string& token, std::unordered_map<std::string, std::string>& defs, std::vector<std::string>& toks, int* index) {
	// iterate over the keys of defs
	for (auto& kv : defs) {
		// if the token starts with the key
		if (token.find(kv.first) != std::string::npos) {
			// replace the key from the string to value
			token.replace(token.find(kv.first), kv.first.size(), kv.second);
			std::vector<std::string> new_toks = lexpp::lex(token, " \t\r\n");
			for (int i = 1; i <= new_toks.size(); i++) {
				toks.insert(toks.begin() + *index + i, new_toks[i - 1]);
			}
			toks.erase(toks.begin() + *index);
			return;
		}
	}
}

static std::string read_lib(const std::string& lib_name, std::vector<std::string>& includePaths, bool* success) {
	for (std::string& includePath : includePaths) {
		std::string	  path = includePath + PATH_SEPARATOR + lib_name;
		std::ifstream file(path);
		if (file.is_open()) {
			std::stringstream buffer;
			buffer << file.rdbuf();
			*success = true;
			return buffer.str();
		}
		*success = false;
	}
	return "";
}

static std::unordered_map<std::string, int> vars;
static std::unordered_map<int, Array> arrays;

static int varId;

static void push_number(Operation& op, int size, void* d)
{
	op.arg = size;
	char* data = static_cast<char*>(d);
	for (int i = 0; i < size; i++) {
		op.ops[i] = data[i];
	}
}

Array::Array()
:isStruct(false)
{
}

Array::Array(int count, int size)
:isStruct(false), count(count), size(size)
{
}

Array::Array(int count, int size, int structId)
:isStruct(true), count(count), size(size), structId(structId)
{
}

std::vector<Operation> parse(std::string input, std::vector<std::string> includePaths, bool debug, std::vector<Operation>& ioperations, std::unordered_map<std::string, std::string>& idefs, bool isInclude) {
	std::vector<Operation>&	operations = ioperations;
	std::unordered_map<std::string, std::string>& defs = idefs;
	std::vector<std::string> tokens	= lexpp::lex(input, " \t\r\n");
	if (isInclude) {
		defs	   = idefs;
		operations = ioperations;
	} else {
		varId = 0;
		arrays.clear();
		vars.clear();
		defs.clear();
		operations.clear();
		defs["main"] = "0";
		operations.push_back(Operation(OperationType::BEGIN));
	}
	for (int l = 0; l < tokens.size(); l++) {
		std::string token = tokens[l];

		if (token.size() == 0) {
			continue;
		}

		// Preprocessor tokens
		if (starts_with(token, "include<\"")) {
			while (!ends_with(token, "\">")) {
				l++;
				token += tokens[l] + " ";
			}
			bool		isIncludeOK = false;
			std::string libSrc		= read_lib(token.substr(9, token.size() - 2), includePaths, &isIncludeOK);
			if (isIncludeOK) {
				parse(libSrc, includePaths, debug, operations, defs, true);
			} else {
				throw std::runtime_error("include " + token.substr(9, token.size() - 2) + " error");
			}
			continue;
		} else if (starts_with(token, "def<")) {
			std::string name = token.substr(4);
			token			 = "";
			while (token[token.size() - 2] != '>') {
				l++;
				token += tokens[l] + " ";
			}
			defs[name] = token.substr(0, token.size() - 2);
			continue;
		}

		process_defs(token, defs, tokens, &l);

		if(debug) {
			std::cout << "token: " << token << std::endl;
			std::cout << "vars: " << vars << std::endl;
		}

		// Operation tokens
		if (is_integer(token)) {
			Operation opers(OperationType::PUSH);
			try {
				int n = std::stoi(token);
				push_number(opers, sizeof(int), &n);
			} catch (...) {
				long long val = std::stoll(token);
				push_number(opers, sizeof(long long), &val);
			}
			operations.push_back(opers);
		} else if (is_float(token)) {
			Operation opers(OperationType::PUSH);
			double		   val	= std::stod(token);
			push_number(opers, sizeof(double), &val);
			operations.push_back(opers);
		} else if (token[0] == '-' && token.size() >=2  && is_integer(token.substr(1))) {
			Operation opers(OperationType::PUSH);
			try {
				int n = std::stoi(token);
				push_number(opers, sizeof(int), &n);
			} catch (...) {
				long long val = std::stoll(token);
				push_number(opers, sizeof(long long), &val);
			}
			operations.push_back(opers);
		} else if (token[0] == '-' && token.size() >=2 && is_float(token.substr(1))) {
			Operation opers(OperationType::PUSH);
			double		   val	= std::stod(token);
			push_number(opers, sizeof(double), &val);
			operations.push_back(opers);
		} else if (token[0] == 'u' && is_integer(token.substr(1))) {
			Operation opers(OperationType::PUSH);
			try {
				int n = std::stoi(token.substr(1));
				push_number(opers, sizeof(int), &n);
			} catch (...) {
				unsigned long long val = std::stoull(token.substr(1));
				push_number(opers, sizeof(unsigned long long), &val);
			}
			operations.push_back(opers);
		} else if (token[0] == 'f' && is_float(token.substr(1))) {
			Operation opers(OperationType::PUSH);
			float val = std::stof(token);
			push_number(opers, sizeof(float), &val);
			operations.push_back(opers);
		} else if (token == "dec") {
			token			 = tokens[++l];
			std::string name = token.substr(0, token.find(":"));
			DataType	type = to_data_type(token.substr(token.find(":") + 1));
			if (type == DataType::UNKNOWN) {
				if(token[token.size() - 1] == ']' && token.find("[") != std::string::npos) {
					std::string typeName = token.substr(token.find(":") + 1, token.find("[") - token.find(":") - 1);
					type = to_data_type(typeName);
					if (type == DataType::UNKNOWN) {
						throw std::runtime_error("unknown data type: " + typeName);
					}
					std::string size = token.substr(token.find("[") + 1, token.find("]") - token.find("[") - 1);
					Array array(stoi(size), get_data_type_size(type));
					Operation op(OperationType::DECL);
					op.arg	  = varId++;
					op.ops[0] = DataType::PTR;
					op.ops[1] = array.count * array.size;
					operations.push_back(op);					
					vars[name] = op.arg;
					arrays[op.arg] = array;
				}
				else{
					throw std::runtime_error("unknown data type : " + token.substr(token.find(":") + 1));
				}
			}
			else{
				Operation op(OperationType::DECL);
				op.arg	  = varId++;
				op.ops[0] = type;
				operations.push_back(op);
				vars[name] = op.arg;
			}
		} else if (vars.find(token) != vars.end()) {
			Operation op(OperationType::PUSH);
			int val = vars[token];
			push_number(op, sizeof(int), &val);
			operations.push_back(op);
		} else if (token == "true") {
			Operation op(OperationType::PUSH);
			int val = true;
			push_number(op, sizeof(int), &val);
			operations.push_back(op);
		} else if (token == "false") {
			Operation op(OperationType::PUSH);
			int val = false;
			push_number(op, sizeof(int), &val);
			operations.push_back(op);
		} else if (token == "+") {
			operations.push_back(Operation(OperationType::ADD));
		} else if (token == "-") {
			operations.push_back(Operation(OperationType::SUB));
		} else if (token == "*") {
			operations.push_back(Operation(OperationType::MUL));
		} else if (token == "/") {
			operations.push_back(Operation(OperationType::DIV));
		} else if (token == "&&") {
			operations.push_back(Operation(OperationType::AND));
		} else if (token == "||") {
			operations.push_back(Operation(OperationType::OR));
		} else if (token == ">") {
			operations.push_back(Operation(OperationType::GT));
		} else if (token == "<") {
			operations.push_back(Operation(OperationType::LT));
		} else if (token == "==") {
			operations.push_back(Operation(OperationType::EQ));
		} else if (token == "<=") {
			operations.push_back(Operation(OperationType::LE));
		} else if (token == ">=") {
			operations.push_back(Operation(OperationType::GE));
		} else if (token == "!=") {
			operations.push_back(Operation(OperationType::NEQ));
		} else if (token[0] == '&') {
			token = token.substr(1);
			if (vars.find(token) != vars.end()) {
				Operation op(OperationType::VAR, vars[token]);
				op.ops[0] = -3;
				operations.push_back(op);
			} else {
				throw std::runtime_error("unknown variable : " + token);
			}
		} else if (token[0] == '$') {
			token = token.substr(1);
			Operation op(OperationType::VAR);
			if (token == "+") {
				op.arg = OperationType::ADD;
			} else if (token == "-") {
				op.arg = OperationType::SUB;
			} else if (token == "*") {
				op.arg = OperationType::MUL;
			} else if (token == "/") {
				op.arg = OperationType::DIV;
			} else if (token == "&&") {
				op.arg = OperationType::AND;
			} else if (token == "||") {
				op.arg = OperationType::OR;
			} else if (token == ">") {
				op.arg = OperationType::GT;
			} else if (token == "<") {
				op.arg = OperationType::LT;
			} else if (token == "==") {
				op.arg = OperationType::EQ;
			} else if (token == "<=") {
				op.arg = OperationType::LE;
			} else if (token == ">=") {
				op.arg = OperationType::GE;
			} else if (token == "!=") {
				op.arg = OperationType::NEQ;
			} else if (token == "print") {
				op.arg = OperationType::PRINT;
			} else if (token == "println") {
				op.arg = OperationType::PRINTLN;
			} else if (token == "input") {
				op.arg = OperationType::INPUTI;
			}
			operations.push_back(op);
		} else if (token[0] == '<') {
			token = token.substr(1);
			if (vars.find(token) == vars.end()) {
				auto tp1 = token.find("[");
				auto tp2 = token.find("]");
				if(tp1 != std::string::npos && tp2 != std::string::npos){
					std::string name = token.substr(0, tp1);
					std::string index = (token.substr(tp1 + 1, tp2 - tp1 - 1));
					if(vars.find(name) == vars.end()){
						throw std::runtime_error("unknown variable : " + name);
					}
					int vid = vars[name];
					Array arr = arrays[vid];
					if(!arr.isStruct){
						if(vars.find(index) != vars.end()){
							Operation op(OperationType::VAR, vars[index]);
							op.ops[0] = -1;
							operations.push_back(op);
							op = Operation(OperationType::PUSH);
							push_number(op, sizeof(int), &arr.size);
							operations.push_back(op);
							operations.push_back(Operation(OperationType::MUL));
						}
						else{
							int indexV;
							try{
								indexV = stoi(index) * arr.size;
							}
							catch(std::exception& e){
								throw std::runtime_error("unparsable value : " + index);
							}
							if(indexV > arr.size * arr.count){
								throw std::runtime_error("array index out of range");
							}
							Operation op(OperationType::PUSH);
							push_number(op, sizeof(int), &indexV);
							operations.push_back(op);
						}
						Operation op(OperationType::PUSH);
						push_number(op, sizeof(int), &arr.size);
						operations.push_back(op);
						op = Operation(OperationType::PUSH);
						push_number(op, sizeof(int), &vid);
						operations.push_back(op);
						operations.push_back(Operation(OperationType::VMEMGET));
					}else{
						throw std::runtime_error("structs not implemented");
					}
				}
				else{
					throw std::runtime_error("undefined variable : " + token);
				}
			}
			else{
				Operation op(OperationType::VAR, vars[token]);
				op.ops[0] = -1;
				operations.push_back(op);
			}
		} else if (token[0] == '>') {
			token = token.substr(1);
			if (vars.find(token) == vars.end()) {
				auto tp1 = token.find("[");
				auto tp2 = token.find("]");
				if(tp1 != std::string::npos && tp2 != std::string::npos){
					std::string name = token.substr(0, tp1);
					std::string index = (token.substr(tp1 + 1, tp2 - tp1 - 1));
					if(vars.find(name) == vars.end()){
						throw std::runtime_error("unknown variable : " + name);
					}
					int vid = vars[name];
					Array arr = arrays[vid];
					if(!arr.isStruct){
						if(vars.find(index) != vars.end()){
							Operation op(OperationType::VAR, vars[index]);
							op.ops[0] = -1;
							operations.push_back(op);
							op = Operation(OperationType::PUSH);
							push_number(op, sizeof(int), &arr.size);
							operations.push_back(op);
							operations.push_back(Operation(OperationType::MUL));
						}
						else{
							int indexV;
							try{
								indexV = stoi(index) * arr.size;
							}
							catch(std::exception& e){
								throw std::runtime_error("unparsable value : " + index);
							}
							if(indexV > arr.size * arr.count){
								throw std::runtime_error("array index out of range");
							}
							Operation op(OperationType::PUSH);
							push_number(op, sizeof(int), &indexV);
							operations.push_back(op);
						}
						Operation op(OperationType::PUSH);
						push_number(op, sizeof(int), &arr.size);
						operations.push_back(op);
						op = Operation(OperationType::PUSH);
						push_number(op, sizeof(int), &vid);
						operations.push_back(op);
						operations.push_back(Operation(OperationType::VMEMSET));
					}else{
						throw std::runtime_error("structs not implemented");
					}
				}
				else{
					throw std::runtime_error("undefined variable : " + token);
				}
			}
			else{
				Operation op(OperationType::VAR, vars[token]);
				op.ops[0] = -2;
				operations.push_back(op);
			}
		} else if (token == "println") {
			operations.push_back(Operation(OperationType::PRINTLN));
		} else if (token == "dup") {
			operations.push_back(Operation(OperationType::DUP, 4));
		} else if (token == "swap") {
			operations.push_back(Operation(OperationType::SWAP, 4));
		} else if (token == "printlns") {
			operations.push_back(Operation(OperationType::PRINTLNS));
		} else if (token == "printlnns") {
			operations.push_back(Operation(OperationType::PRINTLNNS));
		} else if (token == "printlnn") {
			operations.push_back(Operation(OperationType::PRINTLNN));
		} else if (token == "print") {
			operations.push_back(Operation(OperationType::PRINT));
		} else if (token == "prints") {
			operations.push_back(Operation(OperationType::PRINTS));
		} else if (token == "printns") {
			operations.push_back(Operation(OperationType::PRINTNS));
		} else if (token == "printn") {
			operations.push_back(Operation(OperationType::PRINTN));
		} else if (token == "dump") {
			operations.push_back(Operation(OperationType::DUMP));
		} else if (token == "dumps") {
			operations.push_back(Operation(OperationType::DUMPS));
		} else if (token == "dumpns") {
			operations.push_back(Operation(OperationType::DUMPNS));
		} else if (token == "dumpn") {
			operations.push_back(Operation(OperationType::DUMPN));
		} else if (token == "receed") {
			operations.push_back(Operation(OperationType::RECEED));
		} else if (token == "proceed") {
			operations.push_back(Operation(OperationType::PROCEED));
		} else if (token == "puts") {
			operations.push_back(Operation(OperationType::PUTS));
		} else if (token == "putsln") {
			operations.push_back(Operation(OperationType::PUTSLN));
		} else if (token == "inputi") {
			operations.push_back(Operation(OperationType::INPUTI));
		} else if (token == "inputs") {
			operations.push_back(Operation(OperationType::INPUTS));
		} else if (token == "pop") {
			operations.push_back(Operation(OperationType::POP, 4));
		} else if (token == "loadlib") {
			operations.push_back(Operation(OperationType::LOADLIB));
		} else if (token == "^") {
			operations.push_back(Operation(OperationType::POW));
		} else if (token == "%") {
			operations.push_back(Operation(OperationType::MOD));
		} else if (token == "exit") {
			operations.push_back(Operation(OperationType::EXIT));
		} else if (token == "proc_end" || token == "end_proc") {
			operations.push_back(Operation(OperationType::PROC, -1));
		} else if (starts_with(token, "proc_")) {
			operations.push_back(Operation(OperationType::PROC, std::stoi(token.substr(5))));
		} else if (token == "call") {
			operations.push_back(Operation(OperationType::CALL));
		} else if (token == "ncall") {
			operations.push_back(Operation(OperationType::NCALL));
		} else if (token == "malloc") {
			operations.push_back(Operation(OperationType::MALLOC));
		} else if (token == "memset") {
			operations.push_back(Operation(OperationType::MEMSET));
		} else if (token == "memget") {
			operations.push_back(Operation(OperationType::MEMGET));
		} else if (starts_with(token, "memset_")) {
			operations.push_back(Operation(OperationType::MEMSET, std::stoi(token.substr(7))));
		} else if (starts_with(token, "memget_")) {
			operations.push_back(Operation(OperationType::MEMGET, std::stoi(token.substr(7))));
		} else if (token == "if") {
			operations.push_back(Operation(OperationType::IF, 0));
		} else if (token == "elif") {
			operations.push_back(Operation(OperationType::IF, 2));
		} else if (token == "else") {
			operations.push_back(Operation(OperationType::IF, 3));
		} else if (token == "end_if" || token == "if_end") {
			operations.push_back(Operation(OperationType::IF, 1));
		} else if (token == "end_for" || token == "for_end") {
			operations.push_back(Operation(OperationType::FOR, -1));
		} else if (starts_with(token, "for_")) {
			operations.push_back(Operation(OperationType::FOR, std::stoi(token.substr(4))));
		} else if (starts_with(token, "for")) {
			operations.push_back(Operation(OperationType::FOR, -2));
		} else if (starts_with(token, "pop_")) {
			operations.push_back(Operation(OperationType::POP, std::stoi(token.substr(4))));
		} else if (starts_with(token, "dup_")) {
			operations.push_back(Operation(OperationType::DUP, std::stoi(token.substr(4))));
		} else if (starts_with(token, "swap_")) {
			operations.push_back(Operation(OperationType::SWAP, std::stoi(token.substr(5))));
		} else if (starts_with(token, "cast:")) {
			token = token.substr(5);
			auto pos = token.find("->");
			if(pos == std::string::npos)
				throw std::runtime_error("type cast incomplete error");
			std::string from = token.substr(0, pos);
			std::string to = token.substr(pos + 2);
			DataType fromT = to_data_type(from);
			DataType toT = to_data_type(to);
			Operation op(OperationType::CAST);
			op.ops[0] = fromT;
			op.ops[1] = toT;
			operations.push_back(op);
		}  else if (token == "do") {
			int o=operations.size() - 1;
			while(o >=0){
				if(operations[o].op == OperationType::IF && (operations[o].ops[0] == 0 || operations[o].ops[0] == 2)){
					operations[o].ops[1] = (operations.size() - o - 1);
					break;
				}
				o--;
			}
			operations.push_back(Operation(OperationType::IF, 4));
		} else if (token == "while") {
			operations.push_back(Operation(OperationType::WHILE, 0));
		} else if (token == "end_while" || token == "while_end" || token == "do_end" || token == "end_do") {
			operations.push_back(Operation(OperationType::WHILE, 1));
		} else if (token == "ret") {
			operations.push_back(Operation(OperationType::RET));
		} else if (token[0] == '\"') {
			if (token[token.size() - 1] != '\"') {
				while (token[token.size() - 1] != '\"') {
					token += " " + tokens[++l];
				}
			}
			operations.push_back(Operation(OperationType::PUSHS, 0));
			bool isBkslash = false;
			for (int i = 1; i < token.size() - 1; i++) {
				if (token[i] == '\\') {
					isBkslash = true;
				} else {
					if (isBkslash) {
						isBkslash = false;
						switch (token[i]) {
							case 'n':
								token[i] = '\n';
								break;
							case 't':
								token[i] = '\t';
								break;
							case 'r':
								token[i] = '\r';
								break;
							case '\\':
								token[i] = '\\';
								break;
							case '\"':
								token[i] = '\"';
								break;
							case '\'':
								token[i] = '\'';
								break;
							case 'f':
								token[i] = '\f';
								break;
							case 'b':
								token[i] = '\b';
								break;
							default:
								throw std::runtime_error("invalid escape sequence error!");
						}
					}
					operations.push_back(Operation(OperationType::PUSHS, (int) token[i]));
				}
			}
		}
	}
	if (!isInclude)
		operations.push_back(Operation(OperationType::END));
	return operations;
}

}