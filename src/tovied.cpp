#include "tovied.h"

std::string generate_op_table(std::vector<Operation> ops)
{
	std::string opt = "Operation Table : \n\n\n";
	opt += "\tOP ID\t\tOP\t\t\tARG\n\n";
	unsigned long i=0;
	for(auto& op : ops)
	{
		opt += "\t" + std::to_string(i++) + "\t\t" + to_string(op.op) + std::string(3, '\t') + std::to_string(op.arg) + "\n";
	}
	return opt;
}