#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Lexer.h"
#include <stdlib.h>

using namespace std;

vector<Token>tokenVector;

Token::Token(string name, string word, int lineNo)
{
	tokenName = name;
	tokenWord = word;
	tokenLineNumber = lineNo;
}

int arr[30][30]; 
int lineNumber;

bool isKeyword(string str)
{

	if(str=="if"||str=="int"||str=="while"||str=="case"||str=="switch"||str=="default"||str=="print"||str=="IF"||str=="WHILE"||str=="PRINT"||str=="SWITCH"||str=="CASE"||str=="DEFAULT")
	{
		return true;
			//break;
	}
    return false;
}

bool isOperator(char ch)
{
	if(ch=='+'||ch=='-'||ch=='*'||ch=='/'|| ch=='<'||ch=='>'||ch=='='||ch=='!')
	{
		return true;
	}
	return false;
}

/*bool isRelationalOper(string str)
{
	if(str=="<"||str==">"||str=="!=")
	{
		return true;
	}
	return false;
}*/

bool isDelimiter(char c)
{
	if(c==';'||c=='{'||c=='('||c=='['||c==':'||c=='}'||c==')'||c==']'||c==',')
	{
		return true;
	}
	else
	{
		return false;
	}
}

int calculateNextState(int state, int letter)
{
	int input;
	arr[1][1] = 1;
	arr[1][2] = 1;
	arr[2][1] = 2;
	arr[2][2] = 2;
	
	if(state==0 && (isalpha(letter)||letter=='$'||letter=='_'))
	{
		state = 1;
		input = 1;	
		return 	arr[state][input];
	}
	if((state==1)&& (isdigit(letter)||isalpha(letter)||letter=='$'||letter=='_'))
	{
		state = 1;
		input = 2;
		return arr[state][input];
	}
	if(state==0 && isdigit(letter))
	{
		state = 2;
		input = 1;
		return arr[state][input];
	}
	if(state==2 && isdigit(letter))
	{
		state = 2;
		input = 2;
		return arr[state][input];
	}
}

void split(string line)
{
	string word ="";
	int len = (int)line.length();
	lineNumber++;
    for(int i=0;i<=len-1;i++)
    {
    	if(line[i]=='\t'||line[i]==' '||isDelimiter(line[i])||isOperator(line[i])){
        if(word!="")
		{
				lexer(word);
		} 
    	if(isDelimiter(line[i]))
		{
				string opword;
				opword = line[i];
				tokenVector.push_back(Token("DELIMITER", opword, lineNumber));
		}
		else if(isOperator(line[i]))
		{
				string opword;
				opword = line[i];
				tokenVector.push_back(Token("OPERATOR", opword, lineNumber));
		}
		if(line[i]==' ')
		{
              //do nothing
		}
		word="";
    }
	else
	{
		word =  word + line[i];
       if(i==line.length()-1)
		{
		    lexer(word);
	    }
	}
    }
}

void lexer(string word)
{
	int state = 0;
	int wlen = (int)word.length()-1;
	/*if(isRelationalOper(word))
	{
		tokenVector.push_back(Token("RELATIONAL OPERATOR", word, lineNumber));
	}*/
	if(isKeyword(word))
	{
		tokenVector.push_back(Token("KEYWORD", word, lineNumber));
	}
	else{
	
	for(int i=0;i<=wlen;i++)
	{
		state = calculateNextState(state,word[i]);
	}
	if(state == 1)
	{
		tokenVector.push_back(Token("IDENTIFIER", word, lineNumber));
	}
	else if(state == 2)
	{
		tokenVector.push_back(Token("INT", word, lineNumber));
	}
	else
	{
		tokenVector.push_back(Token("UNDEFINED", word, lineNumber));
	}
	state = 0;
   }
}

