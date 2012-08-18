#ifndef GSCLASS_H
#define GSCLASS_H

#include "gsclassallocationsite.h"
#include "gsobjectbase.h"

struct GSClass {

	void (*getMethod(char const *))(void);
	GSClassAllocationSite *findAllocationSite(char const *);
	GSObjectBase *newInstance(GSClassAllocationSite *);

};

#endif // GSCLASS_H