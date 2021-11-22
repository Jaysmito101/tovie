#include "tovied.hpp"

std::string generate_op_table(std::vector<Operation> ops) {
	std::string opt = "Operation Table : \n\n\n";
	opt += "\tOP ID\t\tOP\t\t\tARG";
	for (int j = 0; j < 64; j++) {
		opt += std::string(2, '\t') + "ARG" + std::to_string(j + 1);
	}
	opt += "\n";
	unsigned long i = 0;
	for (auto& op : ops) {
		opt += "\t" + std::to_string(i++) + "\t\t" + to_string(op.op) + std::string(3, '\t') + std::to_string(op.arg);
		for (int j = 0; j < 64; j++) {
			opt += std::string(2, '\t') + std::to_string(op.ops[j]);
		}
		opt += "\n";
	}
	return opt;
}