#include "gsstack.h"

bool GSStack::isEmpty() {
	return this->stack.size() == 0;
}

void GSStack::pop(bool del) {
	if(this->isEmpty()) {
		throw(std::string("Tried to pop an empty stack."));
	}
	if(del) {
		delete this->stack.top();
	}
	stack.pop();
}

void GSStack::add(GSVariable *var) {
	this->stack.push(var);
}

u32 GSStack::size() {
	return this->stack.size();
}

void GSStack::duplicateTop() {
	this->stack.push(this->stack.top()->clone());
}

GSVariable *GSStack::top() {
	return this->stack.top();
}