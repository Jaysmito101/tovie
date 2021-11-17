#include "tovie_runtimelib.h"

#include <stdexcept>
#include <string>

#if defined(WIN32) || defined(_WIN32) 

#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>
#include <assert.h>
#include <windows.h>
#include <stdlib.h>

void close_runtime_lib(void* lib){
    FreeLibrary((HMODULE)lib);
}

LPCWSTR to_LPCWSTR(const char* str){
    size_t size = strlen(str) + 1;  
    wchar_t* outStr = new wchar_t[size]; 
    size_t outSize;
    mbstowcs(outStr, str, size);
    return outStr;
}

// Open DLL using Load Library
void* open_runtime_lib(const char* lib_name){
    HINSTANCE gllID = LoadLibraryW(to_LPCWSTR(lib_name));
    if (!gllID){
        throw std::runtime_error("Failed to load runtime library " + std::string(lib_name));
    }
    return (void*)gllID;
}

#else

void close_runtime_lib(void* lib){

}


void* open_runtime_lib(const char* lib_name){
    return nullptr;
}

#endif