#include <iostream>
#include "lex.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <map>
#include <string>

using std::cout;
using std::regex;
using std::string;
using std::map;
using std::regex_match;
using std::istringstream;



map<string, Token> reserved = {
    {"procedure", PROCEDURE},
    {"String", STRING}, 
    {"else", ELSE},
    {"if", IF},
    {"Integer", INT},
    {"Float", FLOAT},
    {"Character", CHAR},
    {"Put", PUT},
    {"PutLine", PUTLN},
    {"Get", GET},
    {"Boolean", BOOL},
    {"true", TRUE},
    {"false", FALSE},
    {"elsif", ELSIF},
    {"is", IS},
    {"end", END},
    {"begin", BEGIN},
    {"then", THEN},
    {"constant", CONST},
    {"mod", MOD},
    {"and", AND},
    {"Or", OR},
    {"not", NOT},
    {"mod", MOD},
    {"and", AND},
    {"or", OR},
    {"not", NOT},
    {"true", TRUE},
    {"false", FALSE}

};

map<string, Token> operators = {
    {"+", PLUS},
    {"-", MINUS},
    {"*", MULT}, 
    {"/", DIV},
    {":=", ASSOP},
    {"=", EQ},
    {"/=", NEQ},
    {"<", LTHAN},
    {">", GTHAN}, 
    {"<=", LTE},
    {">=", GTE},
    {"&", CONCAT},
    {"**", EXP}

};

map<string, Token> delimiters = {
    {",", COMMA},
    {";", SEMICOL},
    {"(", LPAREN},
    {")", RPAREN},
    {":", COLON},
    {".", DOT}

};

bool ident(string s){
    regex ident("[a-zA-Z][a-zA-Z0-9_]*");
    //use regex search method to see if word matches reg expression structure
    if (regex_match(s, ident)){
        return true;
    }
    return false;
}

bool iconst(string s){
    regex iconst("[0-9]+([eE][-]?[0-9]+)");
    //use regex search method to see if word matches reg expression structure
    if (regex_match(s, iconst)){
        return true;
    }
    return false;
}

bool fconst(string s){
    regex fconst("[0-9]+[.][0-9]+([eE][-]?[0-9]+)");
    //use regex search method to see if word matches reg expression structure
    if (regex_match(s, fconst)){
        return true;
    }
    return false;
}

bool sconst(string s){
    if(s[0] == s[-1] == '\"'){
        return true;
    } else{return false;}
}

bool cconst(string s){
    if(s[0] == s[-1] == '\'' && s.length() ==1){
        return true;
    } else{return false;}
}




/*

int getNextToken(){
    char ch = '<';
    TokState state = START;
    string token;
    while (in.get(ch)){
        string str(1,ch);

        switch(state){
            case START:
                regex r1("[a-zA-Z]");
                regex r2("[0-9]");
                if (regex_match(str, r1)){state = IDENT}
                else if (regex_match(str, r2)) {state = ICONST}
                else if (str is in delimiter list){
                    cout << "delimiter: " << str << endl;
                }
        }
    }
    return 0;
}

*/


/// final method name: LexItem getNextToken(istream& in, ....){
// returns a lexitem with token and lexeme  and the token's linenum while updating linenumber.

// define each state

// reserved word tokens: [PROCEDURE, STRING, ELSE, IF, INT, FLOAT, CHAR, Put, PUTLN, GET, BOOL, TRUE, FALSE, ELSIF, IS, END, BEGIN, THEN, CONST,
//MOD, AND, OR, NOT]

//OPERATORS: PLUS, MINUS, MULT, DIV, ASSOP, EQ, NEQ, LTHAN, GTHAN, LTE, GTE, MOD, CONCAT, AND,
// OR, NOT, EXP

//DELIMITERS: COMMA, SEMICOL, LPARENM RPAREN, COLON, DOT



/*
enum TokState {START, INID, INSTRING, ININT, IDENT, ICONST, FCONST, SCONST, CCONST, BCONST, PROCEDURE,
STRING, ELSE, IF, INT, FLOAT, CHAR, Put, PUTLN, GET, BOOL, TRUE, FALSE, ELSIF, IS, END, BEGIN, THEN, CONST,
MOD, AND, OR, NOT, PLUS, MINUS, MULT, DIV, ASSOP, EQ, NEQ, LTHAN, GTHAN, LTE, GTE, MOD, CONCAT, AND,
OR, NOT, EXP, COMMA, SEMICOL, LPARENM, RPAREN, COLON, DOT, COMMENT, ERR, DONE};

/// for identifier tokens, check for keyword and then return the keyword token


TokState lexstate = START;
string lexeme;

*/
/*

while(in.get(ch)){
    // read input from inb char by char (get method)
    switch (lexstate){
        case START:
            //code for start block

            //if cin == EOF, return EOF. 
            ///else if cin == single quote, go to case CCONST
            break;
        case ININT:
            //code for ininit block
            break;
        case CCONST:



    }




}

*/
// reads from the file by specifically going to the line that 

/* procedure:

1) write the lex.cpp file
    this file defines the getNextToken function that returns 
2) write the tester file that actually prints the output shown and uses the
 getNextToken method that is defined in cpp


*/

/*
LexItem getNextToken (istream& in, int& linenumber){
    while(in.get(ch))



};

*/

 
            


            
            

// my real version: 
#include <iostream>
#include "lex.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <map>
#include <string>
#include <set>
#include <unordered_set>

using std::cout;
using std::regex;
using std::string;
using std::map;
using std::set;
using std::regex_match;
using std::istringstream;

//enum TokState {START, IDENT, RESERVED, OTHER};

map<string, Token> reserved = {
    {"procedure", PROCEDURE},
    {"String", STRING}, 
    {"else", ELSE},
    {"if", IF},
    {"Integer", INT},
    {"Float", FLOAT},
    {"Character", CHAR},
    {"Put", PUT},
    {"PutLine", PUTLN},
    {"Get", GET},
    {"Boolean", BOOL},
    {"true", TRUE},
    {"false", FALSE},
    {"elsif", ELSIF},
    {"is", IS},
    {"end", END},
    {"begin", BEGIN},
    {"then", THEN},
    {"constant", CONST},
    {"mod", MOD},
    {"and", AND},
    {"Or", OR},
    {"not", NOT},
    {"mod", MOD},
    {"and", AND},
    {"or", OR},
    {"not", NOT},
    {"true", TRUE},
    {"false", FALSE}

};

map<string, Token> operators = {
    {"+", PLUS},
    {"-", MINUS},
    {"*", MULT}, 
    {"/", DIV},
    {":=", ASSOP},
    {"=", EQ},
    {"/=", NEQ},
    {"<", LTHAN},
    {">", GTHAN}, 
    {"<=", LTE},
    {">=", GTE},
    {"&", CONCAT},
    {"**", EXP}

};

map<string, Token> delimiters = {
    {",", COMMA},
    {";", SEMICOL},
    {"(", LPAREN},
    {")", RPAREN},
    {":", COLON},
    {".", DOT}

};

map<Token, string> tokenToStringMap = {
    {IF, "IF"}, {ELSE, "ELSE"}, {ELSIF, "ELSIF"}, {PUT, "PUT"}, {PUTLN, "PUTLN"},
    {GET, "GET"}, {INT, "INT"}, {FLOAT, "FLOAT"}, {CHAR, "CHAR"}, {STRING, "STRING"},
    {BOOL, "BOOL"}, {PROCEDURE, "PROCEDURE"}, {TRUE, "TRUE"}, {FALSE, "FALSE"},
    {END, "END"}, {IS, "IS"}, {BEGIN, "BEGIN"}, {THEN, "THEN"}, {CONST, "CONST"},
    {IDENT, "IDENT"}, {ICONST, "ICONST"}, {FCONST, "FCONST"}, {SCONST, "SCONST"},
    {BCONST, "BCONST"}, {CCONST, "CCONST"}, {PLUS, "PLUS"}, {MINUS, "MINUS"},
    {MULT, "MULT"}, {DIV, "DIV"}, {ASSOP, "ASSOP"}, {EQ, "EQ"}, {NEQ, "NEQ"},
    {EXP, "EXP"}, {CONCAT, "CONCAT"}, {GTHAN, "GTHAN"}, {LTHAN, "LTHAN"}, {LTE, "LTE"},
    {GTE, "GTE"}, {AND, "AND"}, {OR, "OR"}, {NOT, "NOT"}, {MOD, "MOD"},
    {COMMA, "COMMA"}, {SEMICOL, "SEMICOL"}, {LPAREN, "LPAREN"}, {RPAREN, "RPAREN"},
    {DOT, "DOT"}, {COLON, "COLON"}, {ERR, "ERR"}, {DONE, "DONE"}
};

bool ident(string s){
    regex ident("[a-zA-Z][a-zA-Z0-9_]*");
    //use regex search method to see if word matches reg expression structure
    if (regex_match(s, ident)){
        return true;
    }
    return false;
}

bool iconst(string s){
    regex iconst("([0-9]+([eE][-]?[0-9]+)?)");
    //use regex search method to see if word matches reg expression structure
    if (regex_match(s, iconst)){
        return true;
    }
    return false;
}

bool fconst(string s){
    regex fconst("[0-9]+[.][0-9]+([eE][-]?[0-9]+)?");
    //use regex search method to see if word matches reg expression structure
    if (regex_match(s, fconst)){
        return true;
    }
    return false;
}

bool sconst(string s){
    if(s[0] == '\'' && s[s.length()-1] == '\"'){
        return true;
    } else{return false;}
}


bool cconst(string s){
    if(s[0] == '\'' && s[2] == '\'' && s.length() == 3){
        return true;
    } else{return false;}
}

int main(int argc, char* argv[]){

    
  // initialize flag booleans 
     static bool all = false;
     static bool num = false;
     static bool str = false;
     static bool id = false;
     static bool kw = false;
    
    // no file given case
    if (argc < 2){
        cout << "No specified input file." << endl;
        return -1;
    }

    string filename = argv[1]; 

    ifstream file(filename);

     if (!file){
        // cant open the file
        cout << "CANNOT OPEN THE FILE " << filename << endl;
        return -1;
    }
    
    
    
    
    string line;
    string word;
    int line_num = 0;

   map<string, string> tokenmap;
    
    int i = 2;
    
    set<string> flags;
    
    while(i < argc){
        flags.insert(argv[i]);
        i++;
       
    }
    
    
    //define a map of flag strings & references to their respective boolean values

    map<string, bool*> bools = {
        {"-all", &all},
        {"-num", &num},
        {"-str", &str},
        {"-id", &id},
        {"-kw", &kw}
    };
      

    // if a flag is given as an argument, the respective boolean variable is turned true --> it will print out its output
    // if flag is unrecognized, it results in error
    for (const string& flag : flags){
       
        
        if(bools.find(flag)!= bools.end()){
            *bools[flag] = true;

        } 
        else if (flag[0] == '-'){
            cout << "Unrecognized flag {" << flag << "}" << endl; 
            return -1;
            
            
        } else{
            // another filename was given
            cout << "Only one file name is allowed." << endl;
            return -1;
        
        } 

    }
    
    
       while (getline(file, line)){
        line_num++;
        
        istringstream ipp(line);
        while (ipp >> word){
            
            // first check if its a comment
            if (word.length() >= 2 && word.substr(0,2) == "--"){
                    break;
            }
            
           if(reserved.find(word) != reserved.end()){
              tokenmap[word] = tokenToStringMap[reserved[word]];
                
            } else  if (operators.find(word) != operators.end()){
                tokenmap[word] = tokenToStringMap[operators[word]];
            } else if (delimiters.find(word) != delimiters.end()){
               
                tokenmap[word] = tokenToStringMap[delimiters[word]];
            } else if(ident(word)){
                tokenmap[word] = "IDENT";
            }
                else if(iconst(word)){
                tokenmap[word] = "ICONST";
            } else if(fconst(word)){
                    
                tokenmap[word] = "FCONST";
            } else if(sconst(word)){
                tokenmap[word] = "SCONST";
            }
              else if(cconst(word)){
                tokenmap[word] = "CCONST";
            }
            
            
        }
       }
     
    //print out map contents
    
    for(map<string, string>:: iterator it = tokenmap.begin(); it!= tokenmap.end(); ++it){

        cout << "Key: " << it->first << " value: \t" << it->second << endl; 
    }
    
    
    // print out stats about file    
    
    if (line_num == 0){
        cout << "Empty file." << endl;
        return -1;
        }
    
     cout << "Lines: " << line_num << endl;
    cout << "Total Tokens: " << tokenmap.size() << endl;
    
    return 0;
    }     
   
   