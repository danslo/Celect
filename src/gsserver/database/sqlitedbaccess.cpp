#include <cstring>
#include <windows.h>

unsigned int configure(const char *str) {
	OutputDebugStringA("STUB: configure");
	return 0;
}

extern "C" __declspec(dllexport) void* __cdecl GSNativeGetMethodImpl_Database_SqliteDbAccess(const char *methodName) {
	if(strcmp(methodName, "configure(string)") == 0) 
		return &configure;
	return NULL;
}