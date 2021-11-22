#include "info.hpp"

#ifdef _WIN32

std::string get_name() {
	return "tvoie-win32-v3.0";
}

#endif

#ifdef __unix__

std::string get_name() {
	return "tvoie-linux-v3.0";
}

#endif
