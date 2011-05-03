#include <cstring>
#include <windows.h>

unsigned int getTransactionCheckFunctionPtr() {
	OutputDebugStringA("STUB: getTransactionCheckFunctionPtr");
	return 0;
}

unsigned int init(char *str) {
	OutputDebugStringA("STUB: init");
	return 0;
}

unsigned int setAllowTransactionScheduling(bool b) {
	OutputDebugStringA("STUB: setAllowTransactionScheduling");
	return 0;
}

unsigned int keepALive() {
	//OutputDebugStringA("STUB: keepAlive");
	return 0;
}

unsigned int requestLoad(char *a, int b, char *c) {
	OutputDebugStringA("STUB: requestLoad");
	return 0;
}

extern "C" __declspec(dllexport) void* __cdecl GSNativeGetMethodImpl_Cluster_ClusterObjectStorage(const char *methodName) {
	if(strcmp(methodName, "getTransactionCheckFunctionPtr()") == 0) 
		return &getTransactionCheckFunctionPtr;
	if(strcmp(methodName, "init(string)") == 0) 
		return &init;
	if(strcmp(methodName, "setAllowTransactionScheduling(boolean)") == 0)
		return &setAllowTransactionScheduling;
	if(strcmp(methodName, "keepALive()") == 0) 
		return &keepALive;
	if(strcmp(methodName, "requestLoad(string,int,string)") == 0) 
		return &requestLoad;
	return NULL;
}