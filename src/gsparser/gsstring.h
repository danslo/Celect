#include <string>

#include "gsvariable.h"

class GSString : public GSCloneableVariable<GSString> {

private:

	std::string str;
	bool quoteString;

public:

	GSString(std::string str, bool quoteString = true) : str(str), quoteString(quoteString) {}

	void getVar(std::ostream &out) const {
		if(quoteString) {
			out << "\"" << str << "\"";
		}
		else {
			out << str;
		}
	}

};