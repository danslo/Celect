#ifndef GSDECOMPILER_H
#define GSDECOMPILER_H

#include <sstream>
#include <string>
#include <vector>

#include "gsstack.h"
#include "reader.hpp"
#include "types.h"

class GSDecompiler : public Reader {

private:

	std::vector<std::string>& stringTable;
	std::ostringstream os;
	void processOpcode();
	std::string readStringFromTable(int index);
	u32 countArguments(const std::string &function);
	void replaceMethodArguments(std::string &function);
	u32 numLines;

	GSStack gsStack;

	struct GSFocus {
		bool isReference;
		GSVariable *focusedVariable;
		GSFocus() : focusedVariable(NULL) {}
	} 
	gsFocus;
	
public:

	GSDecompiler(u8 *data, u32 size, std::vector<std::string> &st);
	~GSDecompiler();
	
	std::string decompile();

};

#endif