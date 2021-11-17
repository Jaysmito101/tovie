#include <iostream>
#include <vector>

extern "C" void test_function(std::vector<int>& programStack) {
   std::cout << "Hello World From C++!" << std::endl;
}
