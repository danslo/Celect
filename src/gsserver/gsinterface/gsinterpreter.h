#ifndef GSINTERPRETER_H
#define GSINTERPRETER_H

#include "gsclass.h"

class GSInterpreter {

public:

	static GSInterpreter *getInterpreter(void);
	static GSInterpreter *newInterpreter(char const *, char const **, int);
	GSClass *getClass(char const *, char const *);

};

#endif // GSINTERPRETER_H