#include "runtimelib.hpp"

#include <stdexcept>
#include <string>

#ifdef _WIN32

#	include <assert.h>
#	include <locale.h>
#	include <stdio.h>
#	include <stdlib.h>
#	include <wchar.h>
#	include <windows.h>

void close_runtime_lib(void* lib) {
	FreeLibrary((HMODULE) lib);
}

LPCWSTR to_LPCWSTR(const char* str) {
	size_t	 size	= strlen(str) + 1;
	wchar_t* outStr = new wchar_t[size];
	size_t	 outSize;
	mbstowcs(outStr, str, size);
	return outStr;
}

// Open DLL using Load Library
void* open_runtime_lib(const char* lib_name) {
	HINSTANCE gllID = LoadLibraryW(to_LPCWSTR(lib_name));
	if (!gllID) {
		throw std::runtime_error("failed to load runtime library " + std::string(lib_name));
	}
	return (void*) gllID;
}

typedef void (*TovieNativeFunc)(void*, int);

std::function<void(void*, int)> get_runtimelib_proc(void* runtimeLib, const char* libProcName) {
	HINSTANCE gllID = (HINSTANCE) runtimeLib;

	// resolve function address here
	TovieNativeFunc funci = (TovieNativeFunc) GetProcAddress(gllID, libProcName);
	if (!funci) {
		throw std::runtime_error("failed to get proc address for " + std::string(libProcName));
	}
	return funci;
}

#endif

#ifdef __unix__

#	include <dlfcn.h>
#	include <limits.h>
#	include <stdio.h>
#	include <unistd.h>

void close_runtime_lib(void* lib) {
	dlclose(lib);
}


void* open_runtime_lib(const char* lib_name) {
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
	} else {
		perror("getcwd() error");
		throw std::runtime_error("failed to get current working directory");
	}
	std::string lib_path = std::string(cwd) + "/" + lib_name;
	void*		gllID	 = dlopen(lib_path.c_str(), RTLD_LAZY);
	if (!gllID) {
		throw std::runtime_error("failed to load runtime library " + std::string(lib_name));
	}
	return gllID;
}


typedef void (*TovieNativeFunc)(void*, int);

std::function<void(void*, int)> get_runtimelib_proc(void* runtimeLib, const char* libProcName) {
	TovieNativeFunc funci;
	;
	funci = (TovieNativeFunc) dlsym(runtimeLib, libProcName);
	if (!funci) {
		throw std::runtime_error("failed to get proc address for " + std::string(libProcName));
	}
	return funci;
}

#endif