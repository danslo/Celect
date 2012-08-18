#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <sstream>

#include "types.h"
#include "conversion.h"

class FileReader {

protected:

	FILE *fp;
	u32  fileSize;
	
	template<typename T>
	T read(bool changeEndianness = true) {
		T var;
		fread(&var, sizeof(var), 1, fp);
		
		if(changeEndianness) {
			switch(sizeof(var)) {
				case 2:
					var = ntohs(var);
					break;
				case 4:
					var = ntohl(var);
					break;
			}
		}

		return var;
	}
	
	std::string readString(int size) {
		char *str = new char[size + 1];
		fread(str, size, 1, fp);
		str[size] = '\0';
		std::string ret(str);
		delete[] str;
		return ret;
	}

	std::string readString() {
		std::ostringstream os;
		while(true) {
			wchar_t character;
			fread(&character, sizeof(character), 1, fp);
			if(character == '\0') {
				break;
			}
			os.put((char)character);
		}
		return os.str();
	}
	
	u8 *readByteArray(int size) {
		u8 *data = new u8[size];
		fread(data, size, 1, fp);
		return data;
	}

	bool reachedEndOfFile() {
		return ftell(fp) >= (long)fileSize;
	}
	
	u32 getOffset() {
		return ftell(fp);
	}


};

#endif