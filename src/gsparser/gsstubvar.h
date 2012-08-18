#ifndef GSSTUBVAR_H
#define GSSTUBVAR_H

#include "gsvariable.h"

class GSStubvar : public GSCloneableVariable<GSStubvar> {

private:

	char *msg;

public:

	GSStubvar(char *msg) : msg(msg) {}
	GSStubvar() {
		msg = "STUBVAR";
	}

	void getVar(std::ostream &out) const {
		out << msg;
	}

};

#endif