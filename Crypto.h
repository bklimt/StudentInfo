
#include <string>
using namespace std;

#define MD5LEN 16

class CCrypto {
	public:
		static string DecryptString( string str, string key );
		static string EncryptString( string str, string key );
		static string HashMD5( string str );
	private:
		static char GetAlphaChar( const unsigned char data );
		static unsigned char GetNonAlphaChar( const char c );
		static string AlphaArmor( const unsigned char* data, int size );
		static int UnAlphaArmor( string data, unsigned char* s );
};

