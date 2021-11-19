#include <iostream>

extern "C" void __declspec(dllexport) test_function(int* programStack, int stackSize) {
   std::cout << "Hello World From C++!" << std::endl;
}