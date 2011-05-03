#ifndef GSPARSER_H
#define GSPARSER_H

#include <map>
#include <string>
#include <vector>
#include <stdio.h>

#include "filereader.hpp"

class GSParser : public FileReader {

private:
	
	typedef void(GSParser::* funcPtr)();
	typedef std::map<std::string, funcPtr> parserMap;
	parserMap parsers;
	std::vector<std::string> stringTable;

	u32 parseSection();
	void mapParsers();
	void gscfParser();
	void tazfParser();
	void strtParser();
	void clasParser();
	void struParser();
	void enumParser();
	void methParser();
	void mtdeParser();
	void refcParser();
	void statParser();
	void sstaParser();
	void varsParser();
	std::string readStringFromTable();

public:

	GSParser(const char *filename);
	GSParser(FILE *_fp);
	~GSParser();

	void parse();

};

#endif