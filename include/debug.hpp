#include <iostream>

#define DEBUG(x)        \
	std::cout << "EN-"; \
	x                   \
	std::cout << "EX" << std::endl;

#define DEBUG_PRINT(x) std::cout << x << std::endl;