#include <cstring>
#include <windows.h>

unsigned int n_getNetworkPacketKindCounter() {
	OutputDebugStringA("STUB: getNetworkPacketKindCounter");
	return 0;
}

unsigned int setNativeObjectLocatorPeer(void *ptr) {
	OutputDebugStringA("STUB: setNativeObjectLocatorPeer");
	return 0;
}

unsigned int startORBClient(int i) {
	OutputDebugStringA("STUB: startORBClient");
	return 0;
}

unsigned int init(int a, int b, int c, void *clusterConfiguration) {
	OutputDebugStringA("STUB: init");
	return 0;
}

extern "C" __declspec(dllexport) void* __cdecl GSNativeGetMethodImpl_Cluster_InterLinkServer(const char *methodName) {
	if(strcmp(methodName, "getNetworkPacketKindCounter()") == 0) 
		return &n_getNetworkPacketKindCounter;
	if(strcmp(methodName, "setNativeObjectLocatorPeer(pointer)") == 0) 
		return &setNativeObjectLocatorPeer;
	if(strcmp(methodName, "startORBClient(int)") == 0) 
		return &startORBClient;
	if(strcmp(methodName, "init(int,int,int,ClusterConfiguration)") == 0)
		return &init;
	return NULL;
}