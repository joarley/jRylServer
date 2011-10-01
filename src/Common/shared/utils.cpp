#include "utils.h"

#include <boost/thread/mutex.hpp>
#include "shared/typedef.h"
#include <ctime>

namespace jRylServer {
namespace common {
namespace shared {
void j_GetLocalTime(tm* tmResult) {
    static boost::mutex getLocalTimeMutex;
    getLocalTimeMutex.lock();
    time_t aux1;
    time(&aux1);
    tm* aux2 = localtime(&aux1);
    memcpy(tmResult, aux2, sizeof(tm));
    getLocalTimeMutex.unlock();
}

int j_hexStrToBytes(char* src, byte* dst) {
	static char* hex = "0123456789abcdef";
	static char* HEX = "0123456789ABCDEF";
	byte *dstini = dst;
	while(*src) {
		int i;
		for(i = 0; hex[i]; i++) {
			if(*src == hex[i] || *src == HEX[i]) {
				*dst = i << 4;
				src++;
				if(*src) {
					int j;
					for(j = 0; hex[j]; j++) {
						if(*src == hex[j] || *src == HEX[j]) {
							*dst += j;
							break;
						}
					}
					if(!hex[j]) {
						return -1;
					}
					src++;
				}
				break;
			}
		}
		if(!hex[i]) {
			return -1;
		}
		dst++;
	}
	return (int)(dst - dstini);
}

int j_parseHexSeq(char* src, byte* dst) {
	while (isspace(*src)) {
		src++;
	}
	if(*src == '{') {
		src++;
		int dstLen = 0;

		goto PARSE_SEQUENCE;
		while(*src != '}') {
			while (isspace(*src)) {
				src++;
			}
			if(*src == ',') {
				src++;
PARSE_SEQUENCE:
				while (isspace(*src)) {
					src++;
				}
				if(*src == '0' && (*(src + 1) == 'x' || *(src + 1) == 'X')) {
					char aux[3];

					src += 2;
					if(!isxdigit(*src) || !*(src + 1)) {
						return -1;
					}
					if(isxdigit(src[1])) {
						aux[0] = *src++;
						aux[1] = *src++;
					} else {
						aux[0] = '0';
						aux[1] = *src++;
					}

					aux[2] = '\0';

					int ret = j_hexStrToBytes(aux, dst);
					if(ret == -1)
					{
						return -1;
					}
					dstLen++;
					dst++;
				} else {
					return -1;
				}
			} else {
				return -1;
			}
			while (isspace(*src)) {
				src++;
			}
		}
		return dstLen;
	} else if(*src == '0' && (*(src + 1) == 'x' || *(src + 1) == 'X') && *(src + 2)) {
		return j_hexStrToBytes((src + 2), dst);
	} else {
		return j_hexStrToBytes(src , dst);
	}
}
} //namespace shared
} //namespace common
} //namespace jRylServer