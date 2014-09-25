#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Lexer.h"
#include <map>
#include<stack>


using namespace std;

void program();
void var_section();
void body();
void id_list();
void stmt_list();
void stmt();
void assign_stmt();
void print_stmt();
void while_stmt();
void if_stmt();
void switch_stmt();
void primary();
void op();
void relop();
void expr();
void condition();
void case_stmt();
void default_case();
void case_list();

void insert(string name, string type, string tvalue);
bool search(string name);
stack<string> stack1;
stack<string> stackw1;
stack<string> stackw2;
stack<string> stackc1;
stack<string> stackc2;
ofstream outputfile;
string message = "";
int flg=0;
string var_case1 = "#c1";
string var_case2 = "#c2";
int countCase=0;
string var_operator = "";
string var_relop = "";
string var_if = "#i";
string var_while1 = "#w1";
string var_while2 = "#w2";
string caseVariable = "";
int count_if=0;
int count_while=0;

string convertInt(int number)
{
	stringstream s;
	s<<number;
	return s.str();
}

struct Value 
{ 
    string type;
	int tvalue; 

    bool operator==(const Value &o) 
	{
        return type == o.type && tvalue == o.tvalue;
    }

    bool operator<(const Value &o) 
	{
        return type < o.type || (type == o.type && tvalue < o.tvalue);
    }
};


std::multimap<string,Value> Symbols;
multimap<string,Value>::iterator mi;
    
int currentToken=0;
int programCounter = 0;

void skipLine()
{
	int line;
	line = tokenVector[currentToken].tokenLineNumber;
	while(line==tokenVector[currentToken].tokenLineNumber)
	{
		currentToken++;
	}	
}
     // symbol table
     
void insert(string name, string type, int tvalue)
{
  Symbols.insert(std::make_pair<string,Value>(name,Value{type,tvalue}));  	
}
 
bool search(string name)
{
  	for(mi = Symbols.find(name);mi != Symbols.end();mi++)
	{
 		return true;
 	}
 	return false;
}

		 	  //Parsing starts
		     
Token nextToken()
{
	Token next = tokenVector[currentToken];
	currentToken++;
	return next;
}


void error_func()
{
	cout<<"ERROR: Exiting the main program"<<tokenVector[currentToken].tokenLineNumber;
	exit(0);
}
void program()
{
	string ids ="";
	string labels = "";
	var_section();
	
		//file<<symbol table
	if(currentToken!=tokenVector.size())
	{
	body();
   }
  
	for( mi = Symbols.begin() ; mi != Symbols.end(); mi++ )
    {  
     	//ids = mi->first;
     //	ids= mi->first;
		// ids= mi->first;
		if(mi->first.substr(0,1)=="#")
		{
			labels= labels + mi->first + "=" + convertInt(mi->second.tvalue) + ",";
		}
		else
		{
			 ids = ids + mi->first + ",";
		}
    //	outputfile<<mi->first<<endl;
 		//outputfile<<mi->first<<" "<<mi->second.tvalue<<endl;
 	   //cout<<mi->first<<" "<<mi->second.type<<" "<<mi->second.tvalue<<endl;
 	//message = string(mi->first) + string(mi->second.type) + string(mi->second.tvalue) + string("\n");
 	}
 	cout<<ids;
 	outputfile<<ids.substr(0,ids.length()-1)<<endl;
 	if(flg==0)
 	{
 		outputfile<<"#e1=1000"<<endl;
 	}
 	else
 	{
 		outputfile<<labels.substr(0,labels.length()-1)<<endl;

 	}
	programCounter++;
	message = message + "OPR 0,0";
	outputfile<<message;	
	cout<<"Finished program";
    
}

void var_section()
{
	id_list();
	if(tokenVector[currentToken].tokenWord == ";")
	{
		currentToken++;
		return;
	}
	else
	{
		error_func();
	}
}

void id_list()
{
	if(tokenVector[currentToken].tokenName == "IDENTIFIER")
	{
		if(search(tokenVector[currentToken].tokenWord))
		{
			cout<<"Duplicated variable";
		}
		else
		{
			insert(tokenVector[currentToken].tokenWord, "int", 0);
		}
		currentToken++;
	}
	else
	{
		error_func();
	}
	if(tokenVector[currentToken].tokenWord == ",")
	{
		currentToken++;
		id_list();
		//return;
	}
	else
	{
		return;
	}
}

void body()
{
	if(tokenVector[currentToken].tokenWord!="{")
	{
		error_func();
	}
	else
	{
		currentToken++;
	}
	cout<<tokenVector[currentToken].tokenWord<<endl;
	stmt_list();
	
	if(tokenVector[currentToken].tokenWord!="}")
	{
		error_func();
	}
	else
	{
		currentToken++;
		return;
	}
}

void stmt_list()
{
	stmt();
	if(tokenVector[currentToken].tokenWord=="}")
	{
		//currentToken++;
		return;
	}
	else
	{
		stmt_list();
	}
}

void stmt()
{
	if(tokenVector[currentToken].tokenName=="IDENTIFIER")
	{
		assign_stmt();
	}
	else if(tokenVector[currentToken].tokenWord=="print"||tokenVector[currentToken].tokenWord=="PRINT")
	{
		print_stmt();
	}
	else if(tokenVector[currentToken].tokenWord=="while"||tokenVector[currentToken].tokenWord=="WHILE")
	{
		while_stmt();
	}
	else if((tokenVector[currentToken].tokenWord=="if") || (tokenVector[currentToken].tokenWord=="IF"))
	{
		if_stmt();
	}
	else if(tokenVector[currentToken].tokenWord=="switch"||tokenVector[currentToken].tokenWord=="SWITCH")
	{
		switch_stmt();
	}
	else
	{
		error_func();
	}
}

void assign_stmt()
{
	string var = "";
	var = tokenVector[currentToken].tokenWord;
	cout<<var;
	cout<<tokenVector[currentToken].tokenName;
	if(tokenVector[currentToken].tokenName!="IDENTIFIER")
	{
		error_func();
	}
	else
	{
			currentToken++;
	}
	if(tokenVector[currentToken].tokenWord!="=")
	{
		error_func();
	}
	else
	{
		currentToken++;
	}
	if(tokenVector[currentToken + 1].tokenName == "OPERATOR")
	{
		expr();
	}
	else
	{
		primary();		
	}
	cout<<tokenVector[currentToken].tokenWord<<endl;
	if(tokenVector[currentToken].tokenWord!=";")
	{
		error_func();
	}
	else
	{
		currentToken++;
		cout<<tokenVector[currentToken].tokenWord<<endl;
	}
	//outputfile<<"STO " + var + ",0"<<endl;
	programCounter++;
	message = message + "STO " + var + ",0" + "\n";
}

void expr()
{
	primary();
	op();
	currentToken++;
	primary();
	//outputfile<<"OPR "  + var_operator + ",0"<<endl;
	programCounter++;
	message = message + "OPR " + var_operator + ",0" + "\n";
}

void op()
{
	if(tokenVector[currentToken].tokenWord=="+")
	{
		var_operator = "2";
	}
	else if(tokenVector[currentToken].tokenWord=="-")
	{
		var_operator = "3";
	}
	else if(tokenVector[currentToken].tokenWord=="*")
	{
		var_operator = "4";
	}
	else if(tokenVector[currentToken].tokenWord=="/")
	{
		var_operator = "5";
	}
}

void primary()
{
	if(tokenVector[currentToken].tokenName == "IDENTIFIER")
	{
		if(search(tokenVector[currentToken].tokenWord))
		{
		//	outputfile<< "LOD " + tokenVector[currentToken].tokenWord + ",0"<<endl;
			programCounter++;
			message = message + "LOD " + tokenVector[currentToken].tokenWord + ",0" + "\n";
     	}
		else
		{
		
			cout<<"Variable not found in symbol table"<<endl;
			//error_func();
		}
		
		currentToken++;
	}
	else if(tokenVector[currentToken].tokenName == "INT")
	{
		//outputfile << "LIT " + tokenVector[currentToken].tokenWord + ",0"<<endl;	
		programCounter++;
		message = message + "LIT " + tokenVector[currentToken].tokenWord + ",0" + "\n";
		currentToken++;	
	}
	else
	{
		error_func();
	}	
}

void print_stmt()
{
	if(tokenVector[currentToken].tokenWord == "print"|| tokenVector[currentToken].tokenWord == "PRINT")
	{
		currentToken++;
	}
	else
	{
		error_func();
	}
	if(tokenVector[currentToken].tokenName!= "IDENTIFIER")
	{
		error_func();
	}
	else
	{
		programCounter++;
		//outputfile<< "LOD " + tokenVector[currentToken].tokenWord + ",0"<<endl;
		message = message + "LOD " + tokenVector[currentToken].tokenWord + ",0" + "\n";
		currentToken++;
	}
	if(tokenVector[currentToken].tokenWord == ";")
	{
		programCounter++;
	//	outputfile<< "OPR 21,0"<<endl;
		message = message + "OPR 21,0 " + "\n";
		currentToken++;
		//return;
	}
	else
	{
		error_func();
	}
}

void while_stmt()
{
	
	flg = 1;
	if(tokenVector[currentToken].tokenWord== "while"|| tokenVector[currentToken].tokenWord== "WHILE")
	{
		var_while1 = var_while1 + convertInt(++count_while);
		//count_while++;
		stackw1.push(var_while1);
		insert(var_while1, "int", programCounter+1);
		currentToken++;
    }
	else
	{
		error_func();
	}
    
		condition();
		//count_while++;
		var_while2 = var_while2 + convertInt(++count_while);
	//	outputfile<<"JMC " + var_while2 + ",false"<<endl;
		stackw2.push(var_while2);
		programCounter++;
		message = message + "JMC " + var_while2 + ",false" + "\n";
		body();
	programCounter++;
	message = message + string("JMP ") + stackw1.top() + string(",0") + string("\n");
	stackw1.pop();
	insert(stackw2.top(), "int", programCounter+1);
	stackw2.pop();
	//count_while++;
}

void if_stmt()
{
	//int count = 0;
	flg=1;
	if(tokenVector[currentToken].tokenWord== "if" || tokenVector[currentToken].tokenWord== "IF")
	{
		cout<<"%%% m in IF ***"<<endl;
		currentToken++;
		condition();
		programCounter++;
	    var_if = "#i" + convertInt(++count_if);
	    cout<<"****"<<var_if<<endl;
		//outputfile<< "JMC " + var_if + ",false"<<endl;
		message = message + string("JMC ") + var_if + string(",false") + string("\n");
		stack1.push(var_if);
	}
	else
	{
		error_func();
	}
	cout<<tokenVector[currentToken-1].tokenWord<<endl;
	while(tokenVector[currentToken-1].tokenWord != "}")
	{
		cout<<tokenVector[currentToken].tokenWord<<endl;
		cout<<"&&&&&&&&&&&"<<endl;
		body();
	}
	cout<<"here"<<var_if<<" : "<<programCounter<<endl;

	insert(stack1.top(), "int", programCounter+1);
		stack1.pop();
	//count++;
}

void condition()
{
	string var="";
	var = tokenVector[currentToken].tokenWord;	
	primary();	
	var = tokenVector[currentToken].tokenWord;	
    relop();
    currentToken++;
	primary();
	programCounter++;
	message = message + "OPR " + var_relop  + ",0" + "\n";
	
}

void relop()
{
	if(tokenVector[currentToken].tokenWord=="<")
	{
		var_relop = "12";
	}
	else if(tokenVector[currentToken].tokenWord==">")
	{
		var_relop = "11";
	}
	else if(tokenVector[currentToken].tokenWord=="!")
	{
		currentToken++;
		if(tokenVector[currentToken].tokenWord=="=")
		{
			var_relop = "16";
		}
	}	
}


void switch_stmt()
{
    
	if(tokenVector[currentToken].tokenWord== "switch"||tokenVector[currentToken].tokenWord== "SWITCH")
	{
		var_case2 = var_case2 + convertInt(countCase);
		countCase++;
		stackc2.push(var_case2);
		currentToken++;
	}
	else
	{
		error_func();
	}
	if(tokenVector[currentToken].tokenName!= "IDENTIFIER")
	{
		error_func();
	}
	else
	{
		caseVariable =  "LOD " + tokenVector[currentToken].tokenWord  + ",0" + "\n";
		currentToken++;
	}
	if(tokenVector[currentToken].tokenWord=="{")
	{
		currentToken++;
	}
	case_list();
	
	//var = tokenVector[currentToken].tokenWord;
	
	if(tokenVector[currentToken].tokenWord=="}")
	{
		insert(stackc2.top(), "int", programCounter+1);
		stackc2.pop();
		currentToken++;
	}
	else if(tokenVector[currentToken].tokenWord == "default"||tokenVector[currentToken].tokenWord== "DEFAULT")
	{
	    default_case();
		if(tokenVector[currentToken].tokenWord=="}")
	    {
	    	insert(stackc2.top(), "int", programCounter+1);
			stackc2.pop();
			currentToken++;
	    }
	    else
	    {
	    	error_func();
	    }
	}
	else
	{
		error_func();
	}
}

void case_list()
{
	case_stmt();
	//currentToken++;
	if(tokenVector[currentToken].tokenWord == "case"||tokenVector[currentToken].tokenWord== "CASE")
	{
	    case_list();
	}	
	return;
}

void case_stmt()
{
	if(tokenVector[currentToken].tokenWord == "case"||tokenVector[currentToken].tokenWord== "CASE")
	{
		currentToken++;
		countCase++;
		var_case1 = var_case1 + convertInt(countCase);
		
    }
    else
    {
    	error_func();
    }
  	if(tokenVector[currentToken].tokenName == "INT")
    {
    	programCounter++;
    	message = message + caseVariable;
    	programCounter++;
    	message = message + "LIT " + tokenVector[currentToken].tokenWord + ",0" + "\n";
    	programCounter++;
    	message = message + "OPR " + "14,0" + "\n";
    	currentToken++;
    }
    else
    {
    	error_func();
    }
    if(tokenVector[currentToken].tokenWord == ":")
    {
    	currentToken++;
    	programCounter++;
    	message = message + "JMC " + var_case1 + ",false" + "\n";
    	stackc1.push(var_case1);
    	body();
    	programCounter++;
    	message = message + "JMP " + var_case2 + ",0" + "\n";
    	insert(stackc1.top(), "int", programCounter+1);
    	stackc1.pop();
    	
    }
    else
    {
    	error_func();
    }  
	
}

void default_case()
{
	if(tokenVector[currentToken].tokenWord == "default"||tokenVector[currentToken].tokenWord == "DEFAULT")
	{
		currentToken++;
    }
    else
    {
    	error_func();
    }
    if(tokenVector[currentToken].tokenWord == ":")
    {
    	currentToken++;
    }
    else
    {
    	
    	error_func();
    }
    while(tokenVector[currentToken-1].tokenWord != "}")
	{
		body();
	}
	
}

int main(int argc, char *argv[])
{
	string line="";
	ifstream inputfile (argv[1]);
	//ifstream inputfile ("input.txt");
	while(getline(inputfile,line))
	{
		split(line);
	}
    
	outputfile.open(argv[2]);
//	outputfile.open("output.txt");
	//outputfile<<message;
	//outputfile.close();
	
	for(int i=0; i<tokenVector.size();i++)
	{
		cout<< tokenVector[i].tokenName << tokenVector[i].tokenWord << tokenVector[i].tokenLineNumber<<endl;
	}	
	program();
}
