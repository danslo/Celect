#include <cstring>
#include <windows.h>

unsigned int getNetworkPacketKindCounter() {
	OutputDebugStringA("STUB: getNetworkPacketKindCounter");
	return 0;
}

unsigned int progressAuth() {
	OutputDebugStringA("STUB: progressAuth");
	return 0;
}

unsigned int authConnection(void *ptr) {
	OutputDebugStringA("STUB: authConnection");
	return 0;
}


extern "C" __declspec(dllexport) void* __cdecl GSNativeGetMethodImpl_ServerLogic_ObjectNetworkServer(const char *methodName) {
	if(strcmp(methodName, "getNetworkPacketKindCounter()") == 0) 
		return &getNetworkPacketKindCounter;
	if(strcmp(methodName, "progressAuth()") == 0) 
		return &progressAuth;
	if(strcmp(methodName, "authConnection(pointer)") == 0)
		return &authConnection;
	return NULL;
}