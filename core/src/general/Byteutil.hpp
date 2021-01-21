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
			vector<unsigned char> x;
			x.insert(x.end(),val.begin() + i,val.begin() + i + len - 1);
			v.push_back(x);
		}
		return v;
	}

}

