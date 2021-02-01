#pragma once

#include <array>
#include <memory>
#include <type_traits>
#include <iomanip>
#include <string>

using namespace std;

namespace byteutil {

	using byte = unsigned char;

	inline vector<byte> to_bytes( const string & str ) {
		vector<byte> bytes;
		const byte* begin = reinterpret_cast< const byte* >(str.data());
		bytes.resize(str.length());
		memcpy(&bytes[0],begin,str.length());
		return bytes ;
	}

	template< typename T >vector<byte>
	inline to_bytes( const T& object ) {
		vector<byte> bytes ;
		const byte* begin = reinterpret_cast< const byte* >( std::addressof(object) ) ;
		bytes.resize(sizeof(T));
		memcpy(&bytes[0],begin,sizeof(T));
		return bytes ;
	}

	template< typename T >
	inline void from_bytes(const vector<byte>& bytes, const T& val) {
		memcpy((byte*)&val,&bytes[0],sizeof(T));
	}
	inline short getShort(const vector<byte>& bytes) {
		short val;
		int len = bytes.size()<2?bytes.size():2;
		memcpy((void*)&val,&bytes[0],len);
		return val;
	}
	inline int getInt(const vector<byte>& bytes) {
		int val;
		int len = bytes.size()<4?bytes.size():4;
		memcpy((void*)&val,&bytes[0],len);
		return val;
	}
	inline long long getLong(const vector<byte>& bytes) {
		unsigned long val=0L;
		int len = bytes.size()<8?bytes.size():8;
		memcpy((void*)&val,&bytes[0],len);
		return val;
	}

	inline string getString(const vector<byte>& bytes) {
		string val;
		int len = bytes.size();
		val.resize(len);
		memcpy(const_cast<char*>(val.data()), &bytes[0],len);
		return val;
	}

	inline vector<byte> mergeBytes( initializer_list<vector<byte>> list )  {
		vector<byte> v;
		for( vector<byte> vin : list ) {
			v.insert(v.end(),vin.begin(),vin.end());
		}
		return v;
	}

	inline vector<vector<byte>> splitBytes( vector<byte> val, int len )  {
		vector<vector<byte>> v;
		for(size_t i=0;i<val.size();i+=len) {
			vector<byte> sub;
			sub.insert(sub.end(),val.begin() + i,val.begin() + i + len);
			v.push_back(sub);
		}
		return v;
	}

	inline vector<byte> mergeDynamicBytes( initializer_list<vector<byte>> list )  {
		vector<byte> v;
		for( vector<byte> vin : list ) {
			int len = vin.size();
			v.push_back((byte)len & 0xff);
			v.push_back((byte)(len >> 8) & 0xff);
			v.push_back((byte)(len >> 16) & 0xff);
			v.push_back((byte)(len >> 24) & 0xff);
			v.insert(v.end(),vin.begin(),vin.end());
		}
		return v;
	}

	inline vector<vector<byte>> splitDynamicBytes( vector<byte> val )  {

		vector<vector<byte>> v;

		size_t pos=0;
		while(pos < val.size()) {

	        int len = val[pos++] & 0xff;
	        len = len + ((val[pos++] & 0xff) << 8);
	        len = len + ((val[pos++] & 0xff) << 16);
	        len = len + ((val[pos++] & 0xff) << 24);

	        vector<byte> sub;
			sub.insert(sub.end(),val.begin() + pos,val.begin() + pos + len);
			v.push_back(sub);
			pos += len;

		}

		return v;
	}

}

