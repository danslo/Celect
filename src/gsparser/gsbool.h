#ifndef GSBOOL_H
#define GSBOOL_H

#include "gsvariable.h"

class GSBool : public GSCloneableVariable<GSBool> {

private:

	bool b;

public:

	GSBool(bool b) : b(b) {}

	void getVar(std::ostream &out) const {
		out << b;
	}

};

#endif