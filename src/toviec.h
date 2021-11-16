#include <string>
#include <vector>
#include <unordered_map>
#include "operations.h"

// These are temporary
static std::vector<Operation> _TMPDATA_;
static std::unordered_map<std::string, std::string> _TMPDATA2_;

std::vector<Operation> parse(std::string& input, std::string& includePath, std::vector<Operation>& prevOps = _TMPDATA_, std::unordered_map<std::string, std::string>& defs = _TMPDATA2_, bool isInclude = false);
