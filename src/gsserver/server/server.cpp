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

	/*
		--server:
			nodecontroller			-> needs config\clusterconfig.xml
			nodecontroller_process	-> missing native implementation
			full_storage			-> missing native implementation for getListenPort()
			full_filedb				-> temporarily works, until it fails with "Cant get challenge". It is trying to connect to ObjectLocator service.
			full					-> same as full_filedb
			test					-> missing native implementation for makeTestConnect() in serverlogic
		--localip	123.123.123.123	-> unknown
		--port		1060			-> at least for full_* it will use this port to listen on
		--server	someword		-> unknown
		--config	someword		-> unknown
	*/
	
	/*
	__asm {
		push objectBase;
	}
	startupClass->getMethod("startObjectLocatorServer()")();
	*/


	const char *config = "--server full --port 1060";
	__asm {
		push config;
		push objectBase; 
	}
	startupClass->getMethod("startup(string)")();
	startupClass->getMethod("loop()")();

	/*
	__asm {
		push objectBase;
	}
	startupClass->getMethod("initStorageAccessFileDbFull()")();
	
	startupClass->getMethod("loop()")();
*/
	shutdownSockets();
	shutdownGC();
}