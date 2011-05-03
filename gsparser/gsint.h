#ifndef GSINT_H
#define GSINT_H

#include "types.h"
#include "gsvariable.h"

class GSInt : public GSCloneableVariable<GSInt> {

private:

	u32 i;

public:

	GSInt(u32 i) : i(i) {}

	void getVar(std::ostream &out) const {
		out << i;
	}

};

#endif