#pragma once
// helper funtions for manibulate chars
//
//#include <stdlib.h>
#include <string>
#include <array>

namespace Core {
namespace Util {


    // get byte array
    inline char* getBytes(int64_t val, const size_t length) {
        char* result = new char[length];
        for (size_t i = 0; i < length; i++) {
            result[i] = (char)(val >> (length - i - 1) * 8 & 0xFF);
        }
        return result;
    }
    inline char* getShortBytes(int16_t val) { return getBytes(val, 2); }
    inline char* getIntBytes(int32_t val) { return getBytes(val, 4); }
    inline char* getLongBytes(int64_t val) { return getBytes(val, 8); }

    // getvalues from bytearray
    inline long getVal(char* bytes, const size_t length) {
        if (bytes != nullptr && length <= 8) {
            int64_t result = 0;
            for (int i = 0; i < length; i++) { result |= (bytes[i] & 0xFF) << (length - i - 1) * 8; }
            return result;
        }
        else {
            return 0;
        }
    }
    inline int64_t getLong(char* bytes) { return getVal(bytes, 8); }
    inline int32_t getInt(char* bytes) { return (int)getVal(bytes, 4); }
    inline int16_t getShort(char* bytes) { return (short)getVal(bytes, 2); }        

}
}

