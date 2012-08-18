#include <stdio.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "gsparser.h"
#include "gsdecompiler.h"
#include "types.h"

GSParser::GSParser(const char *filename) {
	fp = fopen(filename, "rb");
	if(fp == NULL) {
		throw(std::string("Could not open file."));
	}
}

GSParser::GSParser(FILE *_fp) {
	fp = _fp;
}

GSParser::~GSParser() {
	fclose(fp);
}

std::string GSParser::readStringFromTable() {
	return stringTable.at(read<u16>());
}

u32 GSParser::parseSection() {
	u32 offset = getOffset();
	std::string sectionHeader = readString(4);
	if(sectionHeader.length() != 4) {
		throw(std::string("Could not find next section, please check previous section's parser."));
	}
	std::cout << "Found section: " << sectionHeader << "\n";
	
	parserMap::iterator pIt = parsers.find(sectionHeader);
	if(pIt == parsers.end()) {
		std::ostringstream os;
		os << "Could not find parser for section: " << sectionHeader;
		throw(os.str());
	}
	(this->*(pIt->second))();
	return getOffset() - offset;
}

void GSParser::parse() {
	mapParsers();
	fileSize = read<u32>(false);
	
	while(!reachedEndOfFile()) {
		parseSection();
	}
	std::cout << "Done!\n";
}

void GSParser::gscfParser() {
	std::cout << "GSCF method stub.\n";
}

void GSParser::tazfParser() {
	u32 unknown = read<u32>(false);
}

void GSParser::strtParser() {
	u32 size = read<u32>();
	u32 numStrings = read<u32>(), readStrings = 0;
	std::cout << "Found " << numStrings << " strings (total size: " << size << ")\n";
	
	stringTable.reserve(numStrings);
	while(readStrings++ != numStrings) {
		stringTable.push_back(readString());
	}
}

void GSParser::clasParser() {
	std::string path1 = readStringFromTable(),
				path2 = readStringFromTable(),
				path3 = readStringFromTable(),
				path4 = readStringFromTable();
	std::string originalFile = readStringFromTable();
	
	std::cout << "Original filename: " << originalFile << "\n";
	std::cout << "( " << path1 <<  " ) -> ( " << path2 <<  " ) -> ( " << path3 << " ) -> ( " << path4 << " )\n";
}

void GSParser::struParser() {
	u32 structSize = read<u32>(), size = 0;
	while(size < structSize) {
		std::string structName = readStringFromTable();
		u32 numberOfVariables  = read<u32>(), readVariables = 0;
		std::cout << "Struct: " << structName << " with " << numberOfVariables << " variables.\n";
		size += sizeof(u16) + sizeof(numberOfVariables);
		if(numberOfVariables) {
			while(readVariables++ < numberOfVariables && size < structSize) {
				std::string varName = readStringFromTable(),
							varType = readStringFromTable();
				std::cout << "Struct variable: " << varType << " " << varName << "\n";
				size += (sizeof(u16) * 2);
			}
		}
	}
}

void GSParser::enumParser() {
	u32 enumSize = read<u32>(), size = 0;
	while(size < enumSize) {
		std::string enumName = readStringFromTable();
		u32 numberOfValues  = read<u32>(), readValues = 0;
		std::cout << "Enum: " << enumName << " with " << numberOfValues << " values.\n";
		size += sizeof(u16) + sizeof(numberOfValues);
		if(numberOfValues) {
			while(readValues++ < numberOfValues && size < enumSize) {
				std::string valName = readStringFromTable();
				u32 val = read<u32>();
				std::cout << "Enum value: " << val << ": " << valName << "\n";
				size += sizeof(u16) + sizeof(val);
			}
		}
	}
}

void GSParser::methParser() {
	u32 size = read<u32>();
	std::cout << "Total size of method section: " << size << "\n";
}

void GSParser::mtdeParser() {
	u32 size = read<u32>() - 2;
	std::string methodName = readStringFromTable();
	std::cout << "Method " << methodName << " of size " << size << "\n";
	
	u8 *data = readByteArray(size);
	GSDecompiler decompiler(data, size, stringTable);
	//std::cout << decompiler.decompile() << "\n";
	decompiler.decompile();
}

void GSParser::refcParser() {
	u32 refSize = read<u32>(), read = 0;
	while(read != refSize) {
		std::string ref = readStringFromTable();
		std::string classname = readStringFromTable();
		std::cout << "Class: " << classname << " can be found in " << ref << "\n";
		read += (sizeof(u16) * 2);
	}
}

void GSParser::statParser() {
	u32 size = read<u32>();
	std::cout << "Found a state section of size: " << size << "\n";
}

void GSParser::tczfParser() {
	u32 size = read<u32>(false); // I have no idea what this is :)
}

void GSParser::sstaParser() {
	u32 size = read<u32>() - 2, readBytes = 0;
	std::string state = readStringFromTable();
	std::cout << "Found state section: " << state << "\n";
	while(readBytes != size) {
		readBytes += parseSection();
	}
	std::cout << "End of state section!\n";
}

void GSParser::varsParser() {
	u32 size = read<u32>();
	for(u32 i = 0; i < size; i += (sizeof(u16) * 3)) {
		std::string variable = readStringFromTable();
		u16 unknown = read<u16>();
		std::string type = readStringFromTable();
		std::cout << type << " " << variable << " (unknown: " << unknown << ")\n";
	}
}

void GSParser::mapParsers() {
	parsers.insert(parserMap::value_type("GSCF", &GSParser::gscfParser));
	parsers.insert(parserMap::value_type("TAZF", &GSParser::tazfParser));
	parsers.insert(parserMap::value_type("STRT", &GSParser::strtParser));
	parsers.insert(parserMap::value_type("CLAS", &GSParser::clasParser));
	parsers.insert(parserMap::value_type("STRU", &GSParser::struParser));
	parsers.insert(parserMap::value_type("ENUM", &GSParser::enumParser));
	parsers.insert(parserMap::value_type("METH", &GSParser::methParser));
	parsers.insert(parserMap::value_type("MTDE", &GSParser::mtdeParser));
	parsers.insert(parserMap::value_type("REFC", &GSParser::refcParser));
	parsers.insert(parserMap::value_type("STAT", &GSParser::statParser));
	parsers.insert(parserMap::value_type("SSTA", &GSParser::sstaParser));
	parsers.insert(parserMap::value_type("VARS", &GSParser::varsParser));
	parsers.insert(parserMap::value_type("TCZF", &GSParser::tczfParser));
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		std::cout << "Usage: " << argv[0] << " <filename>\n";
		std::getchar();
		return 0;
	}
	try {
		GSParser parser(argv[1]);
		parser.parse();
	}
	catch(const std::string e) {
		std::cout << "Parsing file '" << argv[1] << "' failed...\nReason: " << e << "\n";
	}
	return 0;
}