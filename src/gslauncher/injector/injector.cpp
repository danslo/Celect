#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

#define PROCESS_NAME L"Celetania.exe"

BOOL injectDll(DWORD dwProcessId, const char *dllName) {
   // Attempt to open the process.
	HANDLE hProc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessId);

	// Get the address to LoadLibraryA.
	LPVOID lpLoadLibraryAddress = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");

	// Create a remote thread to load our DLL.
	LPVOID lpRemoteString = (LPVOID)VirtualAllocEx(hProc, NULL, strlen(dllName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProc, (LPVOID)lpRemoteString, dllName, strlen(dllName), NULL);
	HANDLE hRemoteThread = CreateRemoteThread(hProc, NULL, NULL, (LPTHREAD_START_ROUTINE)lpLoadLibraryAddress, (LPVOID)lpRemoteString, NULL, NULL);   
   
	// Cleanup.
	CloseHandle(hProc);

	return hRemoteThread != NULL;
}

DWORD getProcessId(const wchar_t *fileName) { 
	PROCESSENTRY32 pEntry = { sizeof(PROCESSENTRY32) };

	// Find a process id belonging to a matching process name.
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if(Process32First(hSnapshot, &pEntry)) {
		do {
			if(!wcscmp(pEntry.szExeFile, fileName)) {
				return pEntry.th32ProcessID;
			}
		} while(Process32Next(hSnapshot, &pEntry));
	}

	return NULL;
}

int main() {
	while (true) {
		// Wait for process to load.
		wprintf(L"Waiting for %s to load...\n", PROCESS_NAME);
		DWORD dwProcessId = NULL;
		do {
			Sleep(1000);
			dwProcessId = getProcessId(PROCESS_NAME);
		} while (dwProcessId == NULL);

		// Attempt to inject DLL.
		char dllFile[MAX_PATH];
		GetCurrentDirectoryA(sizeof(dllFile), dllFile);
		sprintf(dllFile, "%s\\..\\Hook\\bin\\Hook.dll", dllFile);
		printf("Found PID %d, attempting to inject DLL... ", dwProcessId);
		if (injectDll(dwProcessId, dllFile)) {
			printf("done!\n");
		}

		// Now wait for it to unload ;)
		do {
			Sleep(100);
			dwProcessId = getProcessId(PROCESS_NAME);
		} while (dwProcessId != NULL);
	}
	return 0;
}