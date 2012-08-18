#include <cstring>
#include <stdio.h>
#include <windows.h>

#include "gsinterpreter.h"

/* Methods */
unsigned int getDatabase(int i) {
	OutputDebugStringA("STUB: getDatabase");

	/* Let's see what this does... */
	GSInterpreter *interpreter = GSInterpreter::getInterpreter();
	GSClass *startupClass = interpreter->getClass("Database", "DbAccess");
	GSClassAllocationSite *allocationSite = startupClass->findAllocationSite("database"); 
	GSObjectBase *objectBase = startupClass->newInstance(allocationSite);

	return (unsigned int)objectBase;
}

unsigned int registerDatabase(void *dbaccess, int i) {
	OutputDebugStringA("STUB: registerDatabase");
	return 0;
}

unsigned int freeUnusedPreparedStatements() {
	//OutputDebugStringA("STUB: freeUnusedPreparedStatements");
	return 0;
}

unsigned int getAffectedRows() {
	OutputDebugStringA("STUB: getAffectedRows");
	return 0;
}

unsigned int hasRow() {
	OutputDebugStringA("STUB: hasRow");
	return 0;
}

unsigned int _free() {
	OutputDebugStringA("STUB: free");
	return 0;
}

/**
 * Dirty workaround to actually get things working for now.
 */
unsigned int prepareStatement(void *, char *statement) {
	OutputDebugStringA("STUB: prepareStatement");
	return (unsigned int)&statement;
}

unsigned int execute() {
	OutputDebugStringA("STUB: execute");
	return 0;
}

/* Classes. */
extern "C" __declspec(dllexport) void* __cdecl GSNativeGetMethodImpl_Database_DbResultSet(const char *methodName) {
	if(strcmp(methodName, "free()") == 0)
		return &_free;
	if(strcmp(methodName, "hasRow()") == 0)
		return &hasRow;
	return NULL;
}

extern "C" __declspec(dllexport) void* __cdecl GSNativeGetMethodImpl_Database_DbAccess(const char *methodName) {
	if(strcmp(methodName, "getDatabase(int)") == 0) 
		return &getDatabase;
	if(strcmp(methodName, "registerDatabase(DbAccess,int)") == 0)
		return &registerDatabase;
	if(strcmp(methodName, "freeUnusedPreparedStatements()") == 0)
		return &freeUnusedPreparedStatements;
	if(strcmp(methodName, "getAffectedRows()") == 0)
		return &getAffectedRows;
	if(strcmp(methodName, "prepareStatement(string)") == 0)
		return &prepareStatement;
	return NULL;
}

extern "C" __declspec(dllexport) void* __cdecl GSNativeGetMethodImpl_Database_DbPreparedStatement(const char *methodName) {
 	if(strcmp(methodName, "execute()") == 0)
		return &execute;
	return NULL;
}