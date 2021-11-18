#include "info.h"

#ifdef _WIN32

std::string get_name(){
    return "tvoie-win32-v2.0";
}

#endif

#ifdef __unix__

std::string get_name(){
    return "tvoie-linux-v2.0";
}

#endif