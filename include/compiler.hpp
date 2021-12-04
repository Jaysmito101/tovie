#include "operations.hpp"

#include <string>
#include <unordered_map>
#include <vector>


namespace tovie
{
// These are temporary
static std::vector<Operation>						_TMPDATA_;
static std::unordered_map<std::string, std::string> _TMPDATA2_;

std::vector<Operation> parse(std::string input, std::vector<std::string> includePaths, bool debug, std::vector<Operation>& prevOps = _TMPDATA_, std::unordered_map<std::string, std::string>& defs = _TMPDATA2_, bool isInclude = false);

}