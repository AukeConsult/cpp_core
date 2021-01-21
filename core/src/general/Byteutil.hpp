#pragma once

#include <array>
#include <memory>
#include <type_traits>
#include <iomanip>
#include <string>

using namespace std;

namespace ByteUtil {

	using byte = unsigned char ;

	template< typename T >vector<byte>  to_bytes( const T& object ) {
		vector<byte> bytes ;
		const byte* begin = reinterpret_cast< const byte* >( std::addressof(object) ) ;
		bytes.resize(sizeof(T));
		memcpy(&bytes[0],begin,sizeof(T));
		return bytes ;
	}

	template< typename T >
	void from_bytes(const vector<unsigned char>& bytes, const T& val) {
		//static_assert( std::is_trivially_copyable<T>::bytes, "not a TriviallyCopyable type" ) ;
		memcpy((char*)&val,&bytes[0],sizeof(T));
	}

	vector<unsigned char> mergeBytes( initializer_list<vector<unsigned char>> list )  {
		vector<unsigned char> v;
		for( vector<unsigned char> vin : list ) {
			v.insert(v.end(),vin.begin(),vin.end());
		}
		return v;
	}

	vector<vector<unsigned char>> splitBytes( vector<unsigned char> val, int len )  {
		vector<vector<unsigned char>> v;
		for(size_t i=0;i<val.size();i+=len) {
			vector<unsigned char> sub;
			sub.insert(sub.end(),val.begin() + i,val.begin() + i + len);
			v.push_back(sub);
		}
		return v;
	}


	vector<unsigned char> mergeDynamicBytes( initializer_list<vector<unsigned char>> list )  {
		vector<unsigned char> v;
		for( vector<unsigned char> vin : list ) {
			int len = vin.size();
			v.push_back((unsigned char)len & 0xff);
			v.push_back((unsigned char)(len >> 8) & 0xff);
			v.push_back((unsigned char)(len >> 16) & 0xff);
			v.push_back((unsigned char)(len >> 24) & 0xff);
			v.insert(v.end(),vin.begin(),vin.end());
		}
		return v;
	}


	vector<vector<unsigned char>> splitDynamicBytes( vector<unsigned char> val )  {
		vector<vector<unsigned char>> v;

		size_t pos=0;
		while(pos < val.size()) {

	        int len = val[pos++] & 0xff;
	        len = len + ((val[pos++] & 0xff) << 8);
	        len = len + ((val[pos++] & 0xff) << 16);
	        len = len + ((val[pos++] & 0xff) << 24);

	        vector<unsigned char> sub;
			sub.insert(sub.end(),val.begin() + pos,val.begin() + pos + len);
			v.push_back(sub);
			pos += len;

		}

		return v;
	}

}

