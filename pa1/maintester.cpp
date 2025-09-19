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

map<string, Token> reserved = {
    {"procedure", PROCEDURE},{"String", STRING}, {"else", ELSE},{"if", IF},{"Integer", INT},{"Float", FLOAT},{"Character", CHAR},
    {"Put", PUT}, {"PutLine", PUTLN},{"Get", GET},{"Boolean", BOOL},{"true", TRUE}, {"false", FALSE},{"elsif", ELSIF},
    {"is", IS},{"end", END}, {"begin", BEGIN}, {"then", THEN},{"constant", CONST},{"mod", MOD},{"and", AND},{"Or", OR},
    {"not", NOT},{"mod", MOD}, {"and", AND}, {"or", OR},{"not", NOT}, {"true", TRUE}, {"false", FALSE}

};

map<string, Token> operators = {{"+", PLUS},{"-", MINUS},{"*", MULT},  {"/", DIV},{":=", ASSOP}, {"=", EQ},{"/=", NEQ},
    {"<", LTHAN},{">", GTHAN}, {"<=", LTE},{">=", GTE}, {"&", CONCAT},{"**", EXP}

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


// do header and file guards and all that

enum TokState {START, IDENTIFIER, INTEGER, FLOATINT, OTHER};

string getNextToken(string& s){
    //why string& s? We are not creating another STRING copy. We are using a reference to original string.
    

    istringstream iss(s);
    regex ident("[a-zA-Z0-9]");
    regex ident2("[a-zA-Z0-9_]");
    regex num("[0-9+-eE]");
    regex flo("[.0-9+-eE]");


    char ch;
    string token = "";
    TokState state = START;


    while (iss.get(ch)){
    
    string str(1, ch);
    
    switch(state){

        case START:
            if(regex_match(str, ident)){
                state = IDENTIFIER;
                token += str;
            } else if(regex_match(str, num)){
                state = INTEGER;
                token += str;
            } else if(operators.find(str) != operators.end()){
                s = s.substr(token.length());
                cout << "Lexeme: " << token << endl;
                return "Token: " + tokenToStringMap[operators[str]];
            }
            
            else {
                state = OTHER;
                s = s.substr(token.length());
                return "This is not an identifier. ";

            } 
            break;
        case IDENTIFIER:
            if(regex_match(str, ident2)){
                token += str;

            } else if(reserved.find(token) != reserved.end()){
                cout << "Lexeme: " << token << endl;
                return "Token : " + tokenToStringMap[reserved[token]];

            } else {
               iss.putback(ch);
               s = s.substr(token.length());
               cout << s << endl;
                cout << "Lexeme: " << token << endl;
                return "Token: IDENT";
            }

            break;
        case INTEGER:
            if (regex_match(str, num)){
                token += str;
            } else if (str == "."){
                token += str;
                state = FLOATINT;
            } else {
               iss.putback(ch);
               s = s.substr(token.length());
                cout << "Lexeme: " << token << endl;
                return "Token: ICONST";
                }
            break;
        case FLOATINT: 
            if (regex_match(str, flo)){
                token += str;
            } else {
                iss.putback(ch);
               s = s.substr(token.length());
                cout << "Lexeme: " << token << endl;
                return "Token: FCONST";
            }
            break;
        
        case OTHER:
            return "Invalid input.";
            break;
        }


    
    } 
        cout << "Issue with token processing. " << endl;
        return "No valid identifier found.";


}
    
int main(){

    string s = "imanidentifier8+2";
    
    // Call the function multiple times to process the input
    string result = getNextToken(s);
    cout << result << endl;  // Will print: "Token: IDENT" and "Lexeme: thisisanident244"

    cout << "this is rest of stirng: " << endl;
    cout << s << endl;


//step 1: regular expression writing
// step 2:  write the flex.1 file
// step 3: integrate flex1. file into the lex.cpp file
// step 4: write main file
// step 5: debug 
    return 0;

}

