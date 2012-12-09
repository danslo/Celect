#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <vector>
#include <string>

const wchar_t *filenames[] = {
	L"Celetania.exe",
	L"Celetania_client.exe"
};

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
	VirtualFreeEx(hProc, (LPVOID)lpRemoteString, strlen(dllName), MEM_RELEASE);
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
				CloseHandle(hSnapshot);
				return pEntry.th32ProcessID;
			}
		} while(Process32Next(hSnapshot, &pEntry));
	}
	CloseHandle(hSnapshot);

	return NULL;
}

void injectThread(const wchar_t *processName) {
	while (true) {
		// Wait for process to load.
		wprintf(L"Waiting for %s to load...\n", processName);
		DWORD dwProcessId = NULL;
		do {
			Sleep(10);
			dwProcessId = getProcessId(processName);
		} while (dwProcessId == NULL);

		// Attempt to inject DLL.
		char dllFile[MAX_PATH];
		GetCurrentDirectoryA(sizeof(dllFile), dllFile);
		sprintf(dllFile, "%s\\..\\Hook\\bin\\Hook.dll", dllFile);
		printf("Found PID %d, attempting to inject DLL... ", dwProcessId);
		if (injectDll(dwProcessId, dllFile)) {
			printf("done!\n");
		}

		// Now wait for it to unload.
		do {
			Sleep(10);
			dwProcessId = getProcessId(processName);
		} while (dwProcessId != NULL);
	}
}

int main() {
	for (int i = 0; i < sizeof(filenames) / sizeof(wchar_t *); i++) {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)injectThread, (LPVOID)filenames[i], NULL, NULL);
	}
	getchar();
	return 0;
}