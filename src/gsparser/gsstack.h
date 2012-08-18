#ifndef GSSTACK_H
#define GSSTACK_H

#include <stack>

#include "gsvariable.h"
#include "types.h"

class GSStack {

private:

	std::stack<GSVariable*> stack;

public:

	u32 size();
	bool isEmpty();
	void pop(bool del = true);
	void add(GSVariable *var);
	void duplicateTop();
	GSVariable *top();

};

#endif // GSSTACK_H