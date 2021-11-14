#include "tovied.h"

std::string generate_op_table(std::vector<Operation> ops)
{
	std::string opt = "Operation Table : \n\n\n";
	opt += "\tOP\t\t\tARG\n\n";
	for(auto& op : ops)
	{
		opt += "\t" + to_string(op.op) + std::string(3, '\t') + std::to_string(op.arg) + "\n";
	}
	return opt;
}