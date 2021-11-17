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

void close_runtime_lib(void* lib)
{
    FreeLibrary((HMODULE)lib);
}

LPCWSTR to_LPCWSTR(const char* str)
{
    size_t size = strlen(str) + 1;  
    wchar_t* outStr = new wchar_t[size]; 
    size_t outSize;
    mbstowcs(outStr, str, size);
    return outStr;
}

// Open DLL using Load Library
void* open_runtime_lib(const char* lib_name)
{
    HINSTANCE gllID = LoadLibraryW(to_LPCWSTR(lib_name));
    if (!gllID){
        throw std::runtime_error("Failed to load runtime library " + std::string(lib_name));
    }
    return (void*)gllID;
}

typedef void (__stdcall *TovieNativeFunc)(std::vector<int>&);

std::function<void(std::vector<int>&)> get_runtimelib_proc(void* runtimeLib, const char* libProcName)
{
    HINSTANCE gllID = (HINSTANCE)runtimeLib;
    
    // resolve function address here
    TovieNativeFunc funci = (TovieNativeFunc)GetProcAddress(gllID, libProcName);
    if (!funci) {
        throw std::runtime_error("Failed to get proc address for " + std::string(libProcName));
    }
    return funci;
}

#else

void close_runtime_lib(void* lib)
{

}


void* open_runtime_lib(const char* lib_name)
{
    return nullptr;
}

std::function<void(std::vector<int>&)> get_runtimelib_proc(void* runtimeLib, const char* libProcName)
{
    return std::function<void(std::vector<int>&)>();
}

#endif