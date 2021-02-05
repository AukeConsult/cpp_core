#pragma once

#include <array>
#include <memory>
#include <type_traits>
#include <iomanip>
#include <string>

using namespace std;

namespace byteutil {

	inline vector<uint8_t> to_bytes( const string & str ) {
		vector<uint8_t> bytes;
		const uint8_t* begin = reinterpret_cast< const uint8_t* >(str.data());
		bytes.resize(str.length());
		memcpy(&bytes[0],begin,str.length());
		return bytes ;
	}

	template< typename T >vector<uint8_t>
	inline to_bytes( const T& object ) {
		vector<uint8_t> bytes ;
		const uint8_t* begin = reinterpret_cast< const uint8_t* >( std::addressof(object) ) ;
		bytes.resize(sizeof(T));
		memcpy(&bytes[0],begin,sizeof(T));
		return bytes ;
	}

	template< typename T >
	inline void from_bytes(const vector<uint8_t>& bytes, const T& val) {
		memcpy((uint8_t*)&val,&bytes[0],sizeof(T));
	}
	inline short getShort(const vector<uint8_t>& bytes) {
		short val;
		int len = bytes.size()<2?bytes.size():2;
		memcpy((void*)&val,&bytes[0],len);
		return val;
	}
	inline int getInt(const vector<uint8_t>& bytes) {
		int val;
		int len = bytes.size()<4?bytes.size():4;
		memcpy((void*)&val,&bytes[0],len);
		return val;
	}
	inline long long getLong(const vector<uint8_t>& bytes) {
		unsigned long val=0L;
		int len = bytes.size()<8?bytes.size():8;
		memcpy((void*)&val,&bytes[0],len);
		return val;
	}

	inline string getString(const vector<uint8_t>& bytes) {
		string val;
		int len = bytes.size();
		val.resize(len);
		memcpy(const_cast<char*>(val.data()), &bytes[0],len);
		return val;
	}

	inline vector<uint8_t> mergeBytes( initializer_list<vector<uint8_t>> list )  {
		vector<uint8_t> v;
		for( vector<uint8_t> vin : list ) {
			v.insert(v.end(),vin.begin(),vin.end());
		}
		return v;
	}

	inline vector<vector<uint8_t>> splitBytes( vector<uint8_t> val, int len )  {
		vector<vector<uint8_t>> v;
		for(size_t i=0;i<val.size();i+=len) {
			vector<uint8_t> sub;
			sub.insert(sub.end(),val.begin() + i,val.begin() + i + len);
			v.push_back(sub);
		}
		return v;
	}

	inline vector<uint8_t> mergeDynamicBytes( initializer_list<vector<uint8_t>> list )  {
		vector<uint8_t> v;
		for( vector<uint8_t> vin : list ) {
			int len = vin.size();
			v.push_back((uint8_t)len & 0xff);
			v.push_back((uint8_t)(len >> 8) & 0xff);
			v.push_back((uint8_t)(len >> 16) & 0xff);
			v.push_back((uint8_t)(len >> 24) & 0xff);
			v.insert(v.end(),vin.begin(),vin.end());
		}
		return v;
	}

	inline vector<vector<uint8_t>> splitDynamicBytes( vector<uint8_t> val )  {

		vector<vector<uint8_t>> v;

		size_t pos=0;
		while(pos < val.size()) {

	        int len = val[pos++] & 0xff;
	        len = len + ((val[pos++] & 0xff) << 8);
	        len = len + ((val[pos++] & 0xff) << 16);
	        len = len + ((val[pos++] & 0xff) << 24);

	        vector<uint8_t> sub;
			sub.insert(sub.end(),val.begin() + pos,val.begin() + pos + len);
			v.push_back(sub);
			pos += len;

		}

		return v;
	}

}

