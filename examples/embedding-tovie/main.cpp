#include "../../include/tovie.h"

#include <string>
#include <iostream>

std::string code = R"(
proc_5
    "Hello, World!" dup_13 putsln
    " From tovie!" putsln
proc_end
)";

int main(int argc, char* argv[])
{
    tovie::Program program = tovie::parse(code, {"."}, false);
    tovie::Stack stack = tovie::simulate(program);
    std::cout << stack.pop_str() << " From C++!" << std::endl;
    return 0;
}