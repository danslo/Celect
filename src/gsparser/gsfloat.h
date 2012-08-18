#ifndef GSFLOAT_H
#define GSFLOAT_H

#include "gsvariable.h"

class GSFloat : public GSCloneableVariable<GSFloat> {

private:

	float f;

public:

	GSFloat(float f) : f(f) {}

	void getVar(std::ostream &out) const {
		out << f;
	}

};

#endif