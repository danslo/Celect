#include <cstring>
#include <windows.h>

unsigned int generatePlayerLoginToken(int a, int b, int c, int d) {
	OutputDebugStringA("STUB: generatePlayerLoginToken");
	return 0;
}

unsigned int decodePlayerLoginToken(const char *str) {
	OutputDebugStringA("STUB: decodePlayerLoginToken");

	struct your_mom {
		unsigned int a;
		unsigned int b; /* Notice the similarity to generate login token. */
		unsigned int c;
		unsigned int d;
	};

	your_mom *mom = new your_mom;
	mom->a = 0xDEADBEEF;
	mom->b = 1;				// world, expected to be '1' in our case...
	mom->c = 0xBAADF00D;
	mom->d = 0xB00BB00B;

	return (unsigned int)mom;
}

extern "C" __declspec(dllexport) void* __cdecl GSNativeGetMethodImpl_ServerLogic_UserAccountServer(const char *methodName) {
	if(strcmp(methodName, "generatePlayerLoginToken(int,int,int,int)") == 0) 
		return &generatePlayerLoginToken;
	if(strcmp(methodName, "decodePlayerLoginToken(string)") == 0) {
		return &decodePlayerLoginToken;
	}
	return NULL;
}