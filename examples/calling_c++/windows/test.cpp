#include <iostream>
#include <vector>

extern "C" void __declspec(dllexport) __stdcall test_function(std::vector<int>& programStack) {
   std::cout << "Hello World From C++!" << std::endl;
}