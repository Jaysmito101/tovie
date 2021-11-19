#include <iostream>
#include <vector>

extern "C" void test_function(int* programStack, int stackSize) {
   std::cout << "Hello World From C++!" << std::endl;
}
