#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>


using namespace std;

void split(string line);
void lexer(string word);
class Token{
	public:
		string tokenName;
    	string tokenWord;
		int tokenLineNumber;	
		Token(string tName, string tWord, int tLineNo);
}; 

extern vector<Token> tokenVector;



