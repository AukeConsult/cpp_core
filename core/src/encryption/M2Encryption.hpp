#pragma once

#include <array>
#include <memory>
#include <stdio.h>
#include <string>

using namespace std;

namespace encryption {

	using byte = unsigned char;

	const byte ENCRYPT_NONE = 255; 			// No encryption
	const byte ENCRYPT_EX = 253; 				// Original m2 encryption,

	const byte ENCRYPT_DES = 251; 			// DES is not implemented
	const byte ENCRYPT_AES = 250; 			// AES encryption, 2
	const byte ENCRYPT_INVERSE = 249; 		// simply invert bytes with value

	class CipherBase {
	public:

		byte* enCrypt(byte * data, int length, int &out_length)  {

			byte headerdata[6] = { FIXED_MARKER, FORCE_INITIALIZE,ENCRYPT_EX,0,ENCRYPT_VALIDATOR1, ENCRYPT_VALIDATOR2};
			byte* outdata = (byte*)calloc(length+6, sizeof(char));

			memcpy ((void*)outdata,(void*)headerdata,6);
			memcpy ((void*)(outdata+6),(void*)data,length);
			enCryptSimple(outdata, length + 6);
			out_length = length+6;
			return outdata;

		}
		byte* deCrypt(byte * data, int length, int &out_length)  {

			byte* outdata = (byte*)calloc(length, sizeof(char));
			deCryptSimple(data,length);
			if(data[4]==ENCRYPT_VALIDATOR1 && data[5]==ENCRYPT_VALIDATOR2) {
				memcpy ((void*)(outdata),(void*)(data+6),length-6);
				out_length = length-6;
				return outdata;
			}
			out_length=0;
			return new byte(0);

		}

	private:

		const byte FIXED_MARKER = (byte) 255;
		const byte FORCE_INITIALIZE = (byte) 255;
		const byte NO_FORCE_INITIALIZE = 0;
		const byte ENCRYPT_VALIDATOR1 = (byte) 255;
		const byte ENCRYPT_VALIDATOR2 = (byte) 255;
		const byte IS_KEY_DELIVERY = (byte) 255;

		// protocol format
		// data always start with 4 bytes
		// byte 0 = 255
		// byte 1 = 255 or 0
		// byte 2 = method
		// byte 3 = length for key information record
		// Key information record

		int fixedKey[10][36] = {
				{ 195, 83, 42, 32, 47, 32, 15, 1, 223, 22, 96, 71, 28, 93, 84, 177, 43, 239, 26, 113, 21, 214, 122, 233, 42, 57, 66, 79, 83, 194, 87, 179, 63, 55, 241, 37 },
				{ 32, 23, 32, 43, 7, 42, 5, 14, 1, 34, 32, 33, 55, 164, 73, 128, 219, 110, 223, 218, 117, 116, 75, 84, 105, 93, 182, 63, 74, 64, 42, 80, 12, 14, 25, 16 },
				{ 15, 183, 42, 32, 247, 232, 15, 11, 23, 122, 196, 1, 228, 3, 284, 17, 42, 29, 226, 13, 221, 14, 12, 23, 242, 57, 66, 79, 83, 194, 287, 79, 6, 55, 21, 7 },
				{ 33, 84, 242, 29, 47, 209, 15, 73, 27, 2, 16, 171, 128, 32, 4, 17, 14, 129, 2, 13, 11, 24, 212, 23, 2, 25, 136, 47, 82, 117, 7, 127, 27, 160, 1, 200 },
				{ 0, 83, 242, 142, 46, 32, 145, 3, 7, 22, 36, 181, 170, 132, 44, 76, 15, 129, 2, 13, 26, 24, 221, 23, 3, 25, 166, 23, 222, 14, 7, 17, 62, 115, 21, 3 },
				{ 195, 55, 42, 32, 47, 32, 15, 1, 73, 22, 96, 71, 28, 93, 84, 177, 43, 239, 26, 113, 21, 214, 122, 203, 42, 57, 66, 79, 83, 194, 87, 179, 63, 55, 241, 37 },
				{ 55, 23, 32, 43, 7, 42, 5, 14, 1, 34, 32, 33, 55, 164, 73, 128, 219, 110, 63, 18, 117, 116, 75, 84, 105, 93, 182, 63, 74, 64, 42, 80, 12, 14, 25, 16 },
				{ 15, 183, 42, 32, 247, 232, 15, 11, 23, 122, 196, 1, 28, 3, 284, 37, 42, 29, 226, 13, 21, 14, 12, 23, 242, 57, 66, 79, 83, 194, 287, 79, 6, 55, 24, 7 },
				{ 4, 55, 242, 135, 48, 32, 15, 13, 27, 2, 16, 171, 128, 132, 4, 227, 13, 169, 2, 13, 218, 24, 22, 63, 2, 25, 166, 47, 82, 114, 7, 127, 62, 125, 31, 3 },
				{ 6, 88, 242, 134, 49, 35, 155, 103, 7, 2, 56, 181, 128, 132, 4, 137, 104, 129, 203, 3, 21, 129, 9, 23, 23, 25, 167, 47, 82, 14, 7, 27, 62, 115, 111, 40 }
		};


		void enCryptSimple(byte* data, int length) {
			if (data[2] != ENCRYPT_NONE && length>6) {
				short keynum = ((short)data[7]/26) + 1;
				data[3] += (byte) keynum;
				for (int i = 4, z = 10, s = 0; i < length; i++, z++) {
					short x = data[i];
					x += fixedKey[keynum][z];
					if (x > 255) {x -= 256;}
					data[i] = (byte) x;
					if (z > 31) {
						z = s;s++;
						if (s > 30) {s = 0;}
					}
				}
			}
		};

		void deCryptSimple(byte* data, int length) {
			if (data[2] != ENCRYPT_NONE&& length>6) {
				short keynum = (short) (data[3] & 0x0F);
				for (int i = 4, z = 10, s = 0; i < length; i++, z++) {
					short x = data[i];
					x -= fixedKey[keynum][z];
					if (x < 0) {x += 256;}
					data[i] = (byte) x;
					if (z > 31) {
						z = s;s++;
						if (s > 30) {s = 0;}
					}
				}
			}
		};

	};

	class EncryptFactory {
	public:

		EncryptFactory() : _method(ENCRYPT_EX) {}
		EncryptFactory(byte method) : _method(method) {}
		~EncryptFactory() {
			delete decrypt_worker;
			delete encrypt_worker;
		}
		byte getMethod() {return _method;}
		void setMethod(byte method) {_method=method;}

		byte* enCrypt(byte * data, int length, int &out_length) {
			return encrypt_worker->enCrypt(data, length, out_length);
		}

		byte* deCrypt(byte * buffer,int length, int &out_length) {
			return decrypt_worker->deCrypt(buffer, length, out_length);
		}

	private:

		byte _method;

		// LHA:
		// set encrypt and decrypt worker is only for test
		CipherBase* decrypt_worker = new CipherBase();
		CipherBase* encrypt_worker = new CipherBase();


	};
}

