
#ifndef XMLOBJECT_H
#define XMLOBJECT_H

#pragma warning( disable:4786 )
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

struct XMLTag {
	string name;
	vector< pair< string, string > > parameters;
	vector< string > options;
	bool empty;
	bool end;
};

class CXMLObject {
	public:
		bool LoadFromFile( string filename );
		bool SaveToFile( string filename );
		string GetType();
		void SetType( string newType );

		string GetParameter( string name );
		vector<string> GetParameters();
		void ClearParameters();
		void AddParameter( string param, string value );

		bool GetOption( string name );
		vector<string> GetOptions();
		void ClearOptions();
		void AddOption( string name );

		int GetChildCount();
		CXMLObject* GetChild( int i );
		void ClearChildren();
		void AddChild( CXMLObject child );

		string GetError();
	private:
		bool CreateObject( vector< XMLTag >& tags, int& index );
		void CreateXMLTags( vector< XMLTag >& tags );
		void SetError( string newError );
		string error;
		string type;
		map< string, string > parameters;
		set< string > options;
		vector< CXMLObject > children;
};

#endif
