#include <cstring>
#include <windows.h>

unsigned int registerStorageCheckFunction(void *ptr, const char *str) {
	OutputDebugStringA("STUB: registerStorageCheckFunction");
	return 0;
}
 
extern "C" __declspec(dllexport) void* __cdecl GSNativeGetMethodImpl_Storage_StorageNetworkServer(const char *methodName) {
	if(strcmp(methodName, "registerStorageCheckFunction(pointer,string)") == 0) 
		return &registerStorageCheckFunction;
	return NULL;
}