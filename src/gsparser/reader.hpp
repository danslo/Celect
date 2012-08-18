#ifndef READER_HPP
#define READER_HPP

#include "types.h"
#include "conversion.h"

class Reader {

protected:

	u32 size;
	u8 *data, *cur;
	
	template<typename T>
	T read(bool changeEndianness = true) {
		T var = *(T*)(cur);
		
		if(changeEndianness) {
			switch(sizeof(var)) {
				case 2:
					var = (T)ntohs(var);
					break;
				case 4:
					var = (T)ntohl(var);
					break;
			}
		}

		cur += sizeof(T);
		return var;
	}
	
	void skip(u32 n) {
		cur += n;
	}
	
	void skipToEnd() {
		cur = data + size;
	}
	
	bool reachedEnd() {
		return cur >= data + size;
	}

};

#endif