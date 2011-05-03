#include <windows.h>
#include <string>
#include <stdio.h>

#include "gsgeneral.h"
#include "gsinterpreter.h"
#include "gsclassallocationsite.h"
#include "gsobjectbase.h"

extern "C" __declspec(dllexport) void celetaniaStartup() {
	LoadLibraryA("GSINTERPRETER.dll");

	/* Init... */
	gsInit();
	setupCrashHandler();
	initSockets();

	GSInterpreter *interpreter = GSInterpreter::newInterpreter("gsclasses", NULL, 0);
	//GSClass *coreClass = interpreter->getClass("Core", "Object");

	GSClass *startupClass = interpreter->getClass("ServerLogic", "ServerStartup");
	GSClassAllocationSite *allocationSite = startupClass->findAllocationSite("startup"); 
	GSObjectBase *objectBase = startupClass->newInstance(allocationSite);

	void (*startup)(void) = startupClass->getMethod("startup(string)");
	const char *config = "--server full --port 1060";
	__asm {
		push config;
		push objectBase; 
	}
	startup();

	startupClass->getMethod("loop()")();

	shutdownSockets();
	shutdownGC();
}