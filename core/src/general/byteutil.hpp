#pragma once

#include <array>
#include <memory>
#include <type_traits>
#include <iomanip>
#include <string>

using namespace std;

namespace ByteUtil {

using byte = unsigned char ;

template< typename T > vector<unsigned char> to_bytes( const T& object ) noexcept {
	vector<unsigned char> v(sizeof(T));
	T val = object;
	memcpy(&v[0],(char*)&val,sizeof(val));
	return v;
}

vector<unsigned char> to_bytes( const char* object ) noexcept {

	cout << "1 " << object << endl;
	cout << "1x " << strlen(object) << endl;

	string val;
	val.resize((int)strlen(object));
	cout << "2a " << val.length() << endl;

	val.insert(1, object, strlen(object));

	cout << "2x " << val << endl;

	vector<unsigned char> v(sizeof(val.length()));
	memcpy(&v[0],val.c_str(),sizeof(val.length()));
	return v;
}

template< typename T >
T from_bytes(const vector<unsigned char>& bytes, const T& L) {
    //static_assert( std::is_trivially_copyable<T>::bytes, "not a TriviallyCopyable type" ) ;
    T val = L;
	memcpy((char*)&val,&bytes[0],sizeof(T));
    return val;
}


template< typename T > vector<unsigned char> toBytesLen( const T& object ) noexcept {
	int len = sizeof(object);
	vector<unsigned char> v(sizeof(object) + 2);
	T val = object;
	memcpy(&v[0],(char*)&len,4);
	memcpy(&v[2],(char*)&val,sizeof(val));
	return v;
}

template< class T > vector<unsigned char> mergeBytes( initializer_list<vector<unsigned char>> list ) noexcept {

	vector<unsigned char> v;
	for( vector<unsigned char> elem : list ) {

	}
	return v;

}




}

