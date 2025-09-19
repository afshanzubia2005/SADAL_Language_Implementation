#include <iostream>
#include <regex>
#include <string>
#include <map>
#include "lex.h"

using std::string;
using std::regex;
using std::map;

// this is only to practice regular expressions 
// regular expression matching:


//0. if cin == EOF (end of file), return DONE. 

//1. IDENT terminal (language identifier):

bool ident(string s){
    regex ident("[a-zA-Z][a-zA-Z0-9_]*");
    //use regex search method to see if word matches reg expression structure
    if yes, 
        return ident
}

bool iconst(string s){
//2. ICONST (integer constant)
    regex iconst("[0-9]+([E][+-][0-9]+)?");

}

bool fconst(string s){
//3. FCONST (floating-point constant)
    regex fconst("[0-9]+[.][0-9]+([E][+-][0-9]+)?");

}

bool sconst(string s){
//4. SCONST (string literal)
    regex sconst("[\"][.]*[\"]");

}

bool cconst(string s){
//5. CCONST (just one character between single quotes)
    regex cconst("['][.][']");
}

bool bconst(string s){
//6. BCONST(true or false)
    if (word == "true" or word == "false"){
        return Bconst token
    }
}

bool reserved(string s){
//7. RESERVED
    // check if word is in reserved.
    if yes,
        return reserved
}

bool operator(string s){
//8. Operator
// check if word is in operator map.
    if yes,
        return operator
}

bool delimiter(string s){
//9. delimiter
    dict delimiter = {key, value} --. {key = delimiter, value = token}
}

bool comment(string s){
//10.comment
    //regular expression for comment
    if (str[:2] == "--"){
        cin >> // till newline is reached (get the full comment till end of line)
        //return comment token
    };
}


//11. error
     else { 
        return error token
        // if none of above states apply, return error.
     
     }



    //next time, take a few days to understand the assignment and its requirements first.

    // later, make a c++ map with the keys as terminals and values as regex expressions 