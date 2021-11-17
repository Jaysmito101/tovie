#pragma once

#include <vector>
#include <functional>

void close_runtime_lib(void* lib);
void* open_runtime_lib(const char* lib_name);

std::function<void(std::vector<int>&)> get_runtimelib_proc(void* runtimeLib, const char* libProcName);