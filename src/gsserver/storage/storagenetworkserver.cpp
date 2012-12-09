#include <cstring>
#include <windows.h>

unsigned int registerStorageCheckFunction(void *ptr, const char *str) {
	OutputDebugStringA("STUB: registerStorageCheckFunction");
	return 0;
}
 
unsigned int getListenPort() {
	return 1234;
}

unsigned int initServerAndGetPointer(const char *str) {
	OutputDebugStringA("STUB: initServerAndGetPointer");
	return 0;
}

unsigned int getNetworkPacketKindCounter() {
	OutputDebugStringA("STUB: getNetworkPacketKindCounter");
	return 0;
}

unsigned int keepALiveReal(float x, bool y) {
	OutputDebugStringA("STUB: keepALiveReal");
	return 0;
}

unsigned int n_shutdown() {
	OutputDebugStringA("STUB: shutdown");
	return 0;
}

extern "C" __declspec(dllexport) void* __cdecl GSNativeGetMethodImpl_Storage_StorageNetworkServer(const char *methodName) {
	if(strcmp(methodName, "registerStorageCheckFunction(pointer,string)") == 0) {
		return &registerStorageCheckFunction;
	}
	if(strcmp(methodName, "getListenPort()") == 0) {
		return &getListenPort;
	}
	if(strcmp(methodName, "initServerAndGetPointer(string)") == 0) {
		return &initServerAndGetPointer;
	}
	if(strcmp(methodName, "getNetworkPacketKindCounter()") == 0) {
		return &getNetworkPacketKindCounter;
	}
	if(strcmp(methodName, "keepALiveReal(float,boolean)") == 0) {
		return &keepALiveReal;
	}
	if(strcmp(methodName, "shutdown()") == 0) {
		return &n_shutdown;
	}
	return NULL;
}