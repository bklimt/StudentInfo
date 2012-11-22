
#include <windows.h>
#include <wincrypt.h>

#include "Crypto.h"

string CCrypto::DecryptString( string str, string key ) {
	if ( !str.length() || !key.length() ) {
		return "";
	}

	int len = 0;
	char* armored = new char[ str.length()+1 ];
	unsigned char* unarmored = new unsigned char[ str.length()+1 ];
	if ( !armored || !unarmored ) {
		return "";
	}

	if ( sscanf( str.c_str(), "%d %s", &len, armored ) != 2 ) {
		delete[] armored;
		delete[] unarmored;
		return "";
	}

	UnAlphaArmor( armored, unarmored );
	unarmored[len] = 0;
	key = HashMD5( key );
	for ( int i=0; i<len; i++ ) {
		unarmored[i] ^= key[ i%key.length() ];
	}

	string retval = (string)(char*)unarmored;

	delete[] armored;
	delete[] unarmored;
	return retval;
}

string CCrypto::EncryptString( string str, string key ) {
	if ( !str.length() || !key.length() ) {
		return "";
	}

	unsigned char* s = new unsigned char[ str.length()+1 ];
	char* ans = new char[ str.length()+20 ];
	if ( !s || !ans ) {
		return "";
	}
	s[str.length()] = 0;
	key = HashMD5( key );
	for ( int i=0; i<str.length(); i++ ) {
		s[i] = str[i] ^ key[ i%key.length() ];
	}

	sprintf( ans, "%d %s", str.length(), AlphaArmor( s, str.length() ).c_str() );
	string retval = ans;

	delete[] s;
	delete[] ans;
	return retval;
}

string CCrypto::HashMD5( string str ) {
	//DWORD dwStatus = 0;
	BOOL bResult = FALSE;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;

	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cbHash = 0;

	//Get handle to the crypto provider
	if (!CryptAcquireContext(&hProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT)) {
		printf("CryptAcquireContext failed.\n");
		return "";
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
		printf("CryptAcquireContext failed.\n");
		CryptReleaseContext(hProv, 0);
		return "";
	}

	if (!CryptHashData(hHash, (const unsigned char*)str.c_str(), str.length(), 0)) {
		printf("CryptHashData failed.\n"); 
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		return "";
	}

	cbHash = MD5LEN;
	string ans = "";
	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
		ans = AlphaArmor( rgbHash, cbHash );
	} else {
		printf("CryptGetHashParam failed.\n"); 
	}
	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	return ans;
}

char CCrypto::GetAlphaChar( const unsigned char data ) {
	if ( data < 10 ) {
		return data + '0';
	}
	if ( data < 36 ) {
		return (data-10)+'a';
	}
	if ( data < 62 ) {
		return (data-36)+'A';
	}
	if ( data < 63 ) {
		return '?';
	}
	if ( data == 63 ) {
		return '$';
	}
	return '_';
}

unsigned char CCrypto::GetNonAlphaChar( const char c ) {
	if ( c>='0' && c<='9' ) {
		return c-'0';
	}
	if ( c>='a' && c<='z' ) {
		return 10+(c-'a');
	}
	if ( c>='A' && c<='Z' ) {
		return 36+(c-'A');
	}
	if ( c=='?' ) {
		return 62;
	}
	if ( c=='$' ) {
		return 63;
	}
	return ~0;
}

string CCrypto::AlphaArmor( const unsigned char* data, int size ) {
	int newsize;
	if ( size % 3 ) {
		newsize = size + ( 3 - ( size % 3 ) );
	} else {
		newsize = size;
	}

	char* newdata = new char[newsize];
	memset( newdata, 0, newsize );
	memcpy( newdata, data, size );

	string ans = "";
	for ( int i=0; i<newsize; i+=3 ) {
		char bytes[4];
		bytes[0] = ( newdata[i] >> 2 ) & 63;
		bytes[1] = ( ( newdata[i] & 3 ) << 4 ) + ( ( newdata[i+1] >> 4 ) & 15 );
		bytes[2] = ( ( newdata[i+1] & 15 ) << 2 ) + ( ( newdata[i+2] >> 6 ) & 3 );
		bytes[3] = newdata[i+2] & 63;

		for ( int j=0; j<4; j++ ) {
			ans += GetAlphaChar( bytes[j] );
		}
	}

	delete[] newdata;
	return ans;
}

int CCrypto::UnAlphaArmor( string data, unsigned char* s ) {
	int size;
	int i,j;
	if ( !s || !data.length() ) {
		return 0;
	}
	size = data.length()-( data.length()%4 );
	unsigned char* p = s;
	for ( i=0; i<size; i+=4 ) {
		char bytes[4];
		for ( j=0; j<4; j++ ) {
			bytes[j] = GetNonAlphaChar( data[i+j] );
			if ( bytes[j] & ( ~63 ) ) {
				return 0;
			}
		}
		*p++ = ( bytes[0]<<2 ) | ( ( bytes[1]>>4 ) & 3 );
		*p++ = ( bytes[1]<<4 ) | ( ( bytes[2]>>2 ) & 15 );
		*p++ = ( bytes[2]<<6 ) | ( ( bytes[3] ) & 63 );
	}
	*p = 0;
	return (size>>2)*3;
}

