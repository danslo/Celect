#ifndef GSVARIABLE_H
#define GSVARIABLE_H

#include <ostream>

class GSVariable {

protected:

	virtual void getVar(std::ostream&) const = 0;

public:

	friend std::ostream &operator<<(std::ostream &out, const GSVariable &gsvar) {
		gsvar.getVar(out);
		return out;
	};

	virtual GSVariable *clone() = 0;

};

template <class T>
class GSCloneableVariable : public GSVariable
{

public:

   GSVariable *clone() { 
	   return new T(*static_cast<T*>(this)); 
   }

};


#endif