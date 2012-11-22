
#include "XMLObject.h"

string EscapeXMLString( string s, bool hasQuotes=false ) {
	string ans = "";
	for ( int i=0; i<s.length(); i++ ) {
		if ( hasQuotes ) {
			switch ( s[i] ) {
				case '\n': ans += "\\n"; break;
				case '\r': ans += "\\r"; break;
				case '\t': ans += "\\t"; break;
				case '\"': ans += "\\\""; break;
				case '\\': ans += "\\\\"; break;
				default:
					ans += s[i];
			}
		} else {
			switch ( s[i] ) {
				case '\n': ans += "\\n"; break;
				case '\r': ans += "\\r"; break;
				case '\t': ans += "\\t"; break;
				case ' ' : ans += "\\ "; break;
				case '\"': ans += "\\\""; break;
				case '\\': ans += "\\\\"; break;
				default:
					ans += s[i];
			}
		}
	}
	return ans;
}

string FormatLineNumber( int linenum, int charnum ) {
	char temp[1000];
	sprintf( temp, "line %d, character %d", linenum, charnum );
	return temp;
}

bool CXMLObject::LoadFromFile( string filename ) {
	bool inTag=false, inString=false, inParameter=false, inValue=false, inTagName=false, inComment=false, inEscape=false;
	XMLTag tag;
	vector< XMLTag > tags;
	string parameter;
	int linenum=1;
	int charnum=1;

	FILE* f = fopen( filename.c_str(), "r" );
	if ( !f ) {
		SetError( "unable to open file" );
		return false;
	}

	int curr;
	curr = fgetc( f );
	while ( curr != EOF ) {
		charnum++;
		if ( curr == '\n' ) {
			linenum++;
			charnum=1;
		}
		char current = (char)curr;
		if ( inTag ) {
			if ( inTagName ) {
				if ( inEscape ) {
					inEscape = false;
					switch ( current ) {
						case 'n': tag.name+='\n'; break;
						case 'r': tag.name+='\r'; break;
						case 't': tag.name+='\t'; break;
						default: tag.name+=current;
					}
				} else {
					if ( current == '/' ) {
						inTagName = false;
						tag.empty = true;
					} else {
						if ( current == ' ' || current == '\n' || current == '\r' || current == '\t' ) {
							inTagName = false;
						} else {
							if ( current == '>' ) {
								inTagName = false;
								inTag = false;
								tags.push_back( tag );
							} else {
								if ( current=='\\' ) {
									inEscape = true;
								} else {
									tag.name += current;
								}
							}
						}
					}
				}
			} else {
				if ( inParameter ) {
					if ( inValue ) {
						if ( inString ) {
							if ( inEscape ) {
								inEscape = false;
								switch ( current ) {
									case 'n': tag.parameters[ tag.parameters.size()-1 ].second+='\n'; break;
									case 'r': tag.parameters[ tag.parameters.size()-1 ].second+='\r'; break;
									case 't': tag.parameters[ tag.parameters.size()-1 ].second+='\t'; break;
									default: tag.parameters[ tag.parameters.size()-1 ].second+=current;
								}
							} else {
								if ( current == '\\' ) {
									inEscape = true;
								} else {
									if ( current == '\"' ) {
										inString = false;
										inValue = false;
										inParameter = false;
									} else {
										tag.parameters[ tag.parameters.size()-1 ].second += current;
									}
								}
							}
						} else {
							if ( inEscape ) {
								inEscape = false;
								switch ( current ) {
									case 'n': tag.parameters[ tag.parameters.size()-1 ].second+='\n'; break;
									case 'r': tag.parameters[ tag.parameters.size()-1 ].second+='\r'; break;
									case 't': tag.parameters[ tag.parameters.size()-1 ].second+='\t'; break;
									default: tag.parameters[ tag.parameters.size()-1 ].second+=current;
								}
							} else {
								if ( current == '\\' ) {
									inEscape = true;
								} else {
									if ( current == '/' ) {
										inParameter = false;
										inValue = false;
										tag.empty = true;
									} else {
										if ( current == '\"' ) {
											if ( tag.parameters[ tag.parameters.size()-1 ].second == "" ) {
												inString = true;
											} else {
												SetError( (string)"stray quote at "+FormatLineNumber(linenum,charnum) );
												return false;
											}
										} else {
											if ( current == ' ' || current == '\n' || current == '\r' || current == '\t' ) {
												inParameter = false;
												inValue = false;
											} else {
												if ( current == '>' ) {
													inParameter = false;
													inValue = false;
													inTag = false;
												} else {
													tag.parameters[ tag.parameters.size()-1 ].second += current;
												}
											}
										}
									}
								}
							}
						}
					} else {
						if ( inEscape ) {
							inEscape = false;
							switch ( current ) {
								case 'n': parameter+='\n'; break;
								case 'r': parameter+='\r'; break;
								case 't': parameter+='\t'; break;
								default: parameter+=current;
							}
						} else {
							if ( current == '\\' ) {
								inEscape = true;
							} else {
								if ( current == '/' ) {
									tag.empty = true;
									inParameter = false;
									tag.options.push_back( parameter );
								} else {
									if ( current == '>' ) {
										tag.options.push_back( parameter );
										parameter = "";
										inTag = false;
										inParameter = false;
									} else {
										if ( current == '=' ) {
											tag.parameters.push_back( pair<string,string>( parameter, "" ) );
											inValue = true;
										} else {
											parameter+=current;
										}
									}
								}
							}
						}
					}
				} else {
					if ( current == '\\' ) {
						inEscape = true;
						if ( tag.name == "" ) {
							inTagName = true;
						} else {
							inParameter = true;
							parameter = "";
						}
					} else {
						if ( current == '/' ) {
							if ( tag.name == "" ) {
								tag.end = true;
							} else {
								tag.empty = true;
							}
						} else {
							if ( current == '>' ) {
								inTag = false;
								tags.push_back( tag );
							} else {
								if ( !(current == ' ' || current == '\n' || current == '\r' || current == '\t') ) {
									if ( tag.name == "" ) {
										inTagName = true;
										tag.name += current;
									} else {
										inParameter = true;
										parameter = "";
										parameter += current;
									}
								}
							}
						}
					}
				}
			}
		} else {
			if ( current == '<' ) {
				inTag = true;
				tag.name = "";
				tag.parameters = vector< pair< string, string > >();
				tag.options = vector< string >();
				tag.empty = false;
				tag.end = false;
			} else {
				if ( current != ' ' && current != '\n' && current != '\t' && current != '\r' ) {
					SetError( (string)"stray character at "+FormatLineNumber(linenum,charnum) );
					return false;
				}
			}
		}
		curr = fgetc( f );
	}
	if ( inTag ) {
		SetError( (string)"unexpected end of file at "+FormatLineNumber(linenum,charnum) );
		return false;
	}
	fclose( f );
	int index = 0;
	if ( !CreateObject( tags, index ) ) {
		return false;
	}
	return true;
}

bool CXMLObject::SaveToFile( string filename ) {
	FILE* f = fopen( filename.c_str(), "w" );
	if ( !f ) {
		SetError( "unable to open file" );
		return false;
	}

	vector< XMLTag > tags;
	CreateXMLTags( tags );

	int depth=0;
	for ( int i=0; i<tags.size(); i++ ) {
		if ( !tags[i].empty && tags[i].end ) {
			depth--;
		}

		int j;
		for ( j=0; j<depth; j++ ) {
			fprintf( f, "  " );
		}
		fprintf( f, "<%s%s", tags[i].end ? "/" : "", tags[i].name.c_str() );
		for ( j=0; j<tags[i].options.size(); j++ ) {
			fprintf( f, " %s", tags[i].options[j].c_str() );
		}
		for ( j=0; j<tags[i].parameters.size(); j++ ) {
			fprintf( f, " %s=\"%s\"",
				tags[i].parameters[j].first.c_str(),
				tags[i].parameters[j].second.c_str()
			);
		}
		fprintf( f, "%s>\n", tags[i].empty ? " /" : "" );
		if ( !tags[i].empty && !tags[i].end ) {
			depth++;
		}
	}

	fclose(f);
	return true;
}

string CXMLObject::GetType() {
	return type;
}

void CXMLObject::SetType( string newType ) {
	type = newType;
}

string CXMLObject::GetParameter( string name ) {
	if ( parameters.find( name ) == parameters.end() ) {
		return "";
	} else {
		return parameters[ name ];
	}
}

vector<string> CXMLObject::GetParameters() {
	vector<string> ans;
	map<string,string>::iterator i;
	for ( i=parameters.begin(); i!=parameters.end(); i++ ) {
		ans.push_back( (*i).first );
	}
	return ans;
}

void CXMLObject::ClearParameters() {
	parameters.clear();
}

void CXMLObject::AddParameter( string param, string value ) {
	parameters[param] = value;
}

bool CXMLObject::GetOption( string name ) {
	return ( options.find( name ) != options.end() );
}

vector<string> CXMLObject::GetOptions() {
	vector<string> ans;
	set<string>::iterator i;
	for ( i=options.begin(); i!=options.end(); i++ ) {
		ans.push_back( *i );
	}
	return ans;
}

void CXMLObject::ClearOptions() {
	options.clear();
}

void CXMLObject::AddOption( string name ) {
	options.insert( name );
}

int CXMLObject::GetChildCount() {
	return children.size();
}

CXMLObject* CXMLObject::GetChild( int i ) {
	if ( i>=0 && i<children.size() ) {
		return &(children[i]);
	} else {
		return NULL;
	}
}

void CXMLObject::ClearChildren() {
	children.clear();
}

void CXMLObject::AddChild( CXMLObject child ) {
	children.push_back( child );
}

string CXMLObject::GetError() {
	return error;
}

bool CXMLObject::CreateObject( vector< XMLTag >& tags, int& index ) {
	type = tags[ index ].name;
	for ( int i=0; i<tags[ index ].parameters.size(); i++ ) {
		parameters[ tags[ index ].parameters[ i ].first ] = tags[ index ].parameters[ i ].second;
	}
	for ( int j=0; j<tags[ index ].options.size(); j++ ) {
		options.insert( tags[ index ].options[ j ] );
	}
	if ( tags[ index ].empty ) {
		index++;
		return true;
	}
	index++;
	while ( index<tags.size() ) {
		if ( tags[ index ].name == type && tags[ index ].end ) {
			index++;
			return true;
		}
		CXMLObject child;
		children.push_back( child );
		if ( !children[ children.size()-1 ].CreateObject( tags, index ) ) {
			SetError( children[ children.size()-1 ].GetError() );
			return false;
		}
	}
	SetError( ( (string)"unexpected end of XML file at " + type + " tag" ).c_str() );
	return false;
}

void CXMLObject::CreateXMLTags( vector< XMLTag >& tags ) {
	XMLTag current;
	current.name = EscapeXMLString( GetType() );

	set<string>::iterator optit;
	map<string,string>::iterator parit;

	for ( optit=options.begin(); optit!=options.end(); optit++ ) {
		current.options.push_back( EscapeXMLString( *optit ) );
	}

	for ( parit=parameters.begin(); parit!=parameters.end(); parit++ ) {
		pair<string,string> param = *parit;
		param.first = EscapeXMLString( param.first );
		param.second = EscapeXMLString( param.second, true );
		current.parameters.push_back( param );
	}

	current.end = false;
	if ( GetChildCount() == 0 ) {
		current.empty = true;
		tags.push_back( current );
	} else {
		current.empty = false;
		tags.push_back( current );

		for ( int i=0; i<GetChildCount(); i++ ) {
			GetChild(i)->CreateXMLTags( tags );
		}

		XMLTag finish;
		finish.name = current.name;
		finish.empty = false;
		finish.end = true;
		tags.push_back( finish );
	}
}

void CXMLObject::SetError( string newError ) {
	error = newError;
}
