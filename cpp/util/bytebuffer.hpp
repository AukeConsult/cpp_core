#pragma once

// special bytebuffer
#include <stdlib.h>
#include <string.h>
#include <array>
#include <vector>

using namespace std;

namespace Core {
namespace Util {
    
    class Bytes {

    public:

        Bytes();
        Bytes(bool val);
        Bytes(int16_t val);
        Bytes(int32_t val);
        Bytes(int64_t val);
        Bytes(const Bytes& buffer);
        Bytes(void* val, size_t size);
        Bytes(std::string val);
        ~Bytes();

        Bytes& operator=(const Bytes&);

        bool    getBool();
        int16_t getInt16();
        int32_t getInt32();
        int64_t getInt64();
        std::string getString();

        //putBool(bool val);
        //putInt16(int16_t val);
        //putInt32(int32_t val);
        //putInt64(int64_t val);
        //putstring(std::string val);
        
        std::string outString();

        void cpy(void* bytes, size_t size);
        void getval(void* dest, size_t size);
        void cpybuffer(const Bytes& buffer);

        size_t  length();
        char*   data();

    private:

        void*   _data;
        size_t  _size=0;
        size_t  _pos=0;

    };

    Bytes::Bytes() { _size = 0; }
    Bytes::Bytes(bool val) { char v = (val==true?'1':'0'); cpy((void*)&v, 1); }
    Bytes::Bytes(int16_t val) { cpy((void*)&val, 2); }
    Bytes::Bytes(int32_t val) { cpy((void*)&val, 4); }
    Bytes::Bytes(int64_t val) { cpy((void*)&val, 8); }
    Bytes::Bytes(const Bytes& buffer) { cpybuffer(buffer); }
    Bytes::Bytes(void* val , size_t size) { cpy((void*)&val, size); }
    Bytes::Bytes(string val) { cpy((void*)val.data(), val.length());}

    Bytes::~Bytes() { if (_data)free(_data); }

    Bytes& Bytes::operator=(const Bytes& buffer) { cpybuffer(buffer); return *this; }

    bool    Bytes::getBool() { char val ='0'; getval((void*)&val, 1); return (val == '0' ? false : true); }
    int16_t Bytes::getInt16()  { int16_t val = 0; getval((void*)&val, 2); return val; }
    int32_t Bytes::getInt32()  { int32_t val = 0; getval((void*)&val, 4); return val; }
    int64_t Bytes::getInt64()  { int64_t val = 0; getval((void*)&val, 8); return val; }
    string  Bytes::getString() { string val((char*)_data, _size); return val;}

    size_t Bytes::length() { return _size; }

    char* Bytes::data() {
        if (_size > 0) {
            void* _ret = malloc(_size);
            if(_ret)memcpy((char*)_ret, _data, _size);
            return (char*)_ret;
        }
        return nullptr;
    }

    // private
    void Bytes::cpy(void* bytes, size_t size) {
        _size = size;
        if (_data) free(_data);
        if (_size > 0) {
            _data = malloc(_size);
            if (_data) memcpy((char*)_data, bytes, _size);
            _pos += _size;
        }
        else { _data = 0; }
    }

    void Bytes::cpybuffer(const Bytes& buffer) {
        if (this != &buffer) {
            _size = buffer._size;
            if (_data) free(_data);
            if (_size > 0) {
                _data = malloc(_size);
                if (_data) memcpy((char*)_data, buffer._data, _size);
                _pos += _size;
            }
            else { _data = 0; }
        }
    }    
    void Bytes::getval(void* dest, size_t size) { memcpy(dest, (char*)_data, size); _pos -= _size;}

}
}

