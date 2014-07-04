#include <windows.h>
#include "detours.h"

void (WINAPI * Real_GetSystemTimeAsFileTime)(LPFILETIME lpSystemTimeAsFileTime) = GetSystemTimeAsFileTime;

void WINAPI Mine_GetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime) {
	// Get real system time.
	FILETIME realFileTime;
	Real_GetSystemTimeAsFileTime(&realFileTime);

	// Just set us back a couple of years :)
	lpSystemTimeAsFileTime->dwHighDateTime = realFileTime.dwHighDateTime - 500000;
	lpSystemTimeAsFileTime->dwLowDateTime  = realFileTime.dwLowDateTime;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            DetourTransactionBegin(); 
            DetourUpdateThread(GetCurrentThread());
            DetourAttach(&(PVOID&)Real_GetSystemTimeAsFileTime, Mine_GetSystemTimeAsFileTime);
            DetourTransactionCommit();
            break;
        case DLL_PROCESS_DETACH:
            DetourTransactionBegin(); 
            DetourUpdateThread(GetCurrentThread());
            DetourDetach(&(PVOID&)Real_GetSystemTimeAsFileTime, Mine_GetSystemTimeAsFileTime);
            DetourTransactionCommit();
            break;
    }
    return TRUE;
}