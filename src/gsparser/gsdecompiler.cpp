#include <cstddef>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <algorithm>
#include <iterator>

#include "gsvariable.h"
#include "gsdecompiler.h"
#include "gsopcodes.h"
#include "gsstring.h"
#include "gsint.h"
#include "gsbool.h"
#include "gsfloat.h"
#include "gsstubvar.h"

#define DUMP_BYTECODE

u32 GSDecompiler::countArguments(const std::string &function) {
	u32 pos  = function.find_first_of('(') + 1;
	u32 size = function.find_last_of(')') - pos;
	if(size) {
		std::string arguments = function.substr(pos, size);
		return std::count(arguments.begin(), arguments.end(), ',') + 1;
	}
	return 0;
}

void GSDecompiler::replaceMethodArguments(std::string &function) {
	u32 numArguments = countArguments(function);
	if(numArguments && !gsStack.isEmpty()) {
		GSStack tempStack = gsStack;
		std::ostringstream arg;
		for(u32 i = 0; i < numArguments; i++) {
			/*
			// To properly implement references, gsStack needs to be able to access isReference.
			if(gsFocus.focusedVariable == &(*tempStack.top()) && gsFocus.isReference) {
				arg << "&";
			}
			*/
			arg << *tempStack.top();
			tempStack.pop(false);
			if(i != numArguments - 1) {
				arg << ", ";
			}
		}
		function = function.substr(0, function.find_first_of('(') + 1) + arg.str() + ")";
	}
	function += ";";
}

GSDecompiler::GSDecompiler(u8 *_data, u32 _size, std::vector<std::string>& st) : stringTable(st) {
	data = cur = _data;
	size = _size;
}

GSDecompiler::~GSDecompiler() {
	if(data != NULL) {
		delete[] data;
	}
}

std::string GSDecompiler::readStringFromTable(int index = -1) {
	u16 idx = index == -1 ? read<u16>() : index;
	if(idx >= stringTable.size()) {
		return std::string("(null)");
	}
	return stringTable.at(idx);
}

void GSDecompiler::processOpcode() {
	u8 opcode = read<u8>();
	std::ostringstream current;

#ifdef DUMP_BYTECODE
	current << "@" << std::setw(4) << std::setfill('0') << ++numLines << " ";
	current << std::hex << std::setw(2) << std::uppercase << (u32)opcode << ": ";
	current << GSOpcodes_to_string(opcode) << ": ";
	current << "-> " << gsStack.size() << " " << std::endl;
#endif

	switch(opcode) {
		case PUSHCONSTSTRING: {
			std::string str = readStringFromTable();
#ifdef DUMP_BYTECODE
			current << str;
#endif
			gsStack.add(new GSString(str));
		}
		break;

		case DUPLICATETOP: {
			gsStack.duplicateTop();
		}
		break;

		case PUSHVARIABLE: {
			gsStack.add(gsFocus.focusedVariable);
			gsFocus.isReference = false;
		}
		break;

		case PUSHVARIABLEREF: {
			gsStack.add(gsFocus.focusedVariable);
			gsFocus.isReference = true;
		}
		break;

		case PUSHCONSTNULL: {
			gsStack.add(new GSInt(0));
		}
		break;

		case PUSHLASTCALLRETURN: {
			gsStack.add(new GSStubvar("LAST_CALL_RETVAL"));
		}
		break;

		case PUSHCONSTINT: {
			gsStack.add(new GSInt(read<u32>()));
		}
		break;
		
		case PUSHCONSTBOOLEAN: {
			gsStack.add(new GSBool(read<u32>() != 0));
		}
		break;
		
		case PUSHCONSTFLOAT: {
			gsStack.add(new GSFloat(read<float>()));
		}
		break;

		case NEWOBJECT: {
			std::string name = readStringFromTable();
			std::string type = readStringFromTable();
			gsStack.add(new GSStubvar("NEWLY_CREATED_OBJECT"));
		}
		break;

		case PUTCOMPARECONDITIONBOOLEAN: {
			skip(sizeof(u16)); // Unsure what this is, but it is related to allocating registers, and it isn't a string value.
			gsStack.add(new GSStubvar("BOOL_FROM_PREVIOUS_COMPARISON"));
		}
		break;

		case CLASSCAST: {
			std::string name = readStringFromTable();
			std::string type = readStringFromTable();
		}
		break;

		case POPVARIABLEREF: {
			gsStack.pop();
		}
		break;

		case POPHANDLE: {
			gsStack.pop();
		}
		break;

		case POPINT: {
			gsStack.pop();
		}
		break;

		case POPBOOLEAN: {
			gsStack.pop();
		}
		break;

		case POPPOINTER: {
			gsStack.pop();
		}
		break;

		case POPFLOAT: {
			gsStack.pop();
		}
		break;

		case POPSTRING: {
			gsStack.pop();
		}
		break;

		case POPOBJECT: {
			gsStack.pop();
		}
		break;

		case POPSTRUCT: {
			gsStack.pop();
		}
		break;

		case RETURNWITHVALUE: {
			std::string type = readStringFromTable();
			gsStack.pop();
		}
		break;

		case LOG: {
			std::string fileName = readStringFromTable();
			gsStack.pop();

			u16 unknown = read<u16>();
			std::string label = readStringFromTable();
			gsStack.pop();
		}
		break;

		case TESTBOOLEAN: {
			gsStack.pop();
		}
		break;

		case ASSIGNMENT: {
			std::string type = readStringFromTable();
			gsStack.pop();
			gsStack.pop();
		}
		break;

		case COMPAREINT: {
			gsStack.pop();
			gsStack.pop();
		}
		break;

		case COMPAREBOOLEAN: {
			gsStack.pop();
			gsStack.pop();
		}
		break;

		case COMPAREFLOAT: {
			gsStack.pop();
			gsStack.pop();
		}
		break;

		case COMPAREPOINTER: {
			gsStack.pop();
			gsStack.pop();
		}
		break;

		case ANDJUMP: {
			std::string str = readStringFromTable();
			gsStack.pop();
		}
		break;

		case ORJUMP: {
			std::string str = readStringFromTable();
			gsStack.pop();
		}
		break;

		case ADDINT: {
			gsStack.pop();
		}
		break;

		case SUBINT: {
			gsStack.pop();
		}
		break;

		case MULTINT: {
			gsStack.pop();
		}
		break;

		case DIVINT: {
			gsStack.pop();
		}
		break;

		case MODINT: {
			gsStack.pop();
		}
		break;

		case INCINT: {
			//gsStack.pop(); pop & push
		}
		break;

		case DECINT: {
			//gsStack.pop(); pop & push
		}
		break;

		case ADDFLOAT: {
			gsStack.pop();
		}
		break;

		case SUBFLOAT: {
			gsStack.pop();
		}
		break;

		case MULTFLOAT: {
			gsStack.pop();
		}
		break;

		case DIVFLOAT: {
			gsStack.pop();
		}
		break;

		case MODFLOAT: {
			gsStack.pop();
		}
		break;

		case NEGATEINT: {
			//gsStack.pop(); pop & push
		}
		break;

		case SQRTFLOAT: {
			gsStack.pop();
		}
		break;

		case NEGATEFLOAT: {
			//gsStack.pop(); pop & push
		}
		break;

		case NEGATEBOOLEAN: {
			//gsStack.pop(); pop & push
		}
		break;

		case FOCUSSTACKTOP: {
			std::string str = readStringFromTable();
#ifdef DUMP_BYTECODE
			current << "String: " << str;
#endif
			gsStack.pop();
		}
		break;

		case ASSERT: {
			std::string str = readStringFromTable();
#ifdef DUMP_BYTECODE
			current << "String: " << str;
#endif
			gsStack.pop();
		}
		break;

		case THROW: {
			gsStack.pop();
		}
		break;

		case DEBUGCHARPOS: {
			skip(8);
		}
		break;
		
		case METHODPARAMETER: {
			std::string paramName = readStringFromTable();
			std::string paramType = readStringFromTable();
			u8 unknown = read<u8>();
#ifdef DUMP_BYTECODE
			current << "Parameter " << paramName << " of type " << paramType << " (unknown: " << std::hex << (u32)unknown << ")";
#endif
		}
		break;

		case STATICMETHODDEFINITION: {
			std::string type = readStringFromTable();
		}
		break;
		
		case SIGNALDEFINITION:
		case METHODDEFINITION:
		case NATIVEMETHODREF:
		case NATIVESTATICMETHODREF:
		case CHANGETOPELEMENTTYPE: {
			std::string type = readStringFromTable();
#ifdef DUMP_BYTECODE
			current << "Type: " << type;
#endif
		}
		break;

		case CALLMETHOD:
		case CALLSUPERMETHOD: {
			std::string methodName = readStringFromTable();
			replaceMethodArguments(methodName);
#ifdef DUMP_BYTECODE
			current << "String: " << methodName << "\n";
#else
			current << methodName << "\n";
#endif	
		}
		break;

		case CALLSTATICMETHOD: {
			std::string methodName = readStringFromTable();
			std::string firstClass = readStringFromTable();
			std::string secndClass = readStringFromTable();
			replaceMethodArguments(methodName);
#ifdef DUMP_BYTECODE
			current << firstClass << "." << secndClass << "::" << methodName << "\n";
#else
			current << firstClass << "." << secndClass << "::" << methodName << "\n";
#endif
		}
		break;

		case FOCUSVARIABLE: {
			std::string str = readStringFromTable();
#ifdef DUMP_BYTECODE
			current << "String: " << str;
#endif
			gsFocus.focusedVariable = new GSString(str, false);
		}
		break;

		case FOCUSSELFVARIABLES:
		case FOCUSLOCALVARIABLES: {
			gsFocus.focusedVariable = new GSStubvar(""); // self / local stubvar
		}
		break;

		case LABEL: {
			std::string str = readStringFromTable();
			current << str << ":\n";
		}
		break;

		case BRANCH:
		case BRANCHEQ:
		case BRANCHNE:
		case BRANCHLT:
		case BRANCHLE:
		case BRANCHGT:
		case BRANCHGE: {
			std::string str = readStringFromTable();
			current << "goto " << str << ";\n";
		}
		break;

		case SWITCHEND: {
			std::string str = readStringFromTable();
#ifdef DUMP_BYTECODE
			current << "String: " << str;
#endif
		}
		break;
		
		case METHODCALLONORIGINALSTACKMARKER:
		case FOCUSMETHODPARAMETER:
		case FOCUSSTATEVARS:
		case REPLACEINTREFWITHVALUE:
		case REPLACESECONDTOPINTREFWITHVALUE:
		case RETURN:
		case ANDOR_EXPRESSION_END:
		case BITWISEOR:
		case BITWISEAND:
		case SWITCHBEGIN:
		case SWAPTOP:
		case METHODFINALMARKER:
		case CALLNATIVEINIT: {

		}
		break;

		case STACKEMPTYCHECKPOINT: {
			if(!gsStack.isEmpty()) {
				std::ostringstream err;
				err << "Reached STACKEMPTYCHECKPOINT with non-empty stack: " << gsStack.size() << "\n";
				throw(err.str());
			}
		}
		break;
		
		case LOCALVAR: {
			std::string name = readStringFromTable();
			std::string type = readStringFromTable();
#ifdef DUMP_BYTECODE
			current << "Name: " << name << " of type: " << type;
#endif
		}
		break;
		
		case SWITCHCASE: {
			u32 value = read<u32>();
			std::string name = readStringFromTable();
#ifdef DUMP_BYTECODE
			current << "Case " << name << " with value: " << value; 
#endif
		}
		break;
		
		case MAKEANONSTRUCT: {
			std::string type = readStringFromTable();
			std::string fields = readStringFromTable();
#ifdef DUMP_BYTECODE
			current << "Struct of type " << type << " with fields: " << fields;
#endif
		}
		break;
		
		default:
#ifdef DUMP_BYTECODE
			current << "...unparsed. :(\n";
#endif
			skipToEnd();
	}

#ifdef DUMP_BYTECODE
	current << " -> " << gsStack.size();
#endif
	os << current.str();
	std::cout << current.str();
}

std::string GSDecompiler::decompile() {
	numLines = 0;
	while(!reachedEnd()) {
		processOpcode();
	}
	return os.str();
}