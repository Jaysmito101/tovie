#include "operations.hpp"
#include "stack.hpp"

#include <string>
#include <vector>

namespace tovie
{
    /*
    * Simulates the program in a virtual machine.
    *
    * @param program The program to simulate.
    * @param debug Whether to print debug information.
    */
    Stack simulate(std::vector<Operation> ops, bool debug = false);  

    /*
    * Simulates the program in a virtual machine.
    *
    * @param program The program to simulate.
    * @param
    * @param debug Whether to print debug information.
    */
    Stack simulate(std::vector<Operation> ops, int entrypoint, bool debug = false);

}