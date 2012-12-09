#ifndef GSINTERPRETER_H
#define GSINTERPRETER_H

#include "gsclass.h"

class GSInterpreter {

public:

	static GSInterpreter *getInterpreter(void);
	static GSInterpreter *newInterpreter(char const *, char const **, int);
	GSClass *getClass(char const *, char const *);

};

bool __cdecl writeSocketBytesBlocking(int, char const *, int);

#endif // GSINTERPRETER_H