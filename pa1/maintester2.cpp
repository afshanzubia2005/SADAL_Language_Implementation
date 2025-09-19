#include <iostream>
#include "lex.h"
#include <cctype>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <regex>
#include <map>
#include <string>
#include <set>
#include <unordered_set>
#include <cmath>

using std::cout;
using std::istringstream;
using std::map;
using std::ostream;
using std::regex;
using std::regex_match;
using std::set;
using std::string;
using std::transform;
using std::to_string;
using std::floor;
using std::ceil;

int num_of_numerals = 0;      // iconst + fconst
int num_of_chars_strings = 0; // sconst + cconst
int num_of_identifiers = 0;   // identifiers
int num_of_keywords = 0;      // keywords


map<string, Token> reserved = {
    {"procedure", PROCEDURE}, {"String", STRING}, {"else", ELSE},{"put", PUT} ,{"if", IF}, {"Integer", INT}, {"integer", INT}, {"float", FLOAT}, {"Character", CHAR}, {"character", CHAR}, {"Put", PUT}, {"get", GET},{"string", STRING}, {"PutLine", PUTLN}, {"Get", GET}, {"Boolean", BOOL}, {"boolean", BOOL}, {"true", TRUE}, {"false", FALSE}, {"elsif", ELSIF}, {"is", IS}, {"end", END}, {"begin", BEGIN}, {"Begin", BEGIN}, {"then", THEN}, {"constant", CONST}, {"mod", MOD}, {"and", AND}, {"Or", OR}, {"not", NOT}, {"mod", MOD}, {"and", AND}, {"or", OR}, {"not", NOT}, {"true", TRUE}, {"false", FALSE}

};

vector<string> reservedInorder = {"if", "else", "elsif", "put", "putline", "get", "integer", "float",
	"character", "string", "boolean", "procedure", "true", "false", "end",
	"is", "begin", "then", "constant"};


// for each word in reservediNorder, check if its in vector keywords. if yes, then place in keywordsordered and print that out
    



map<string, Token> operators = {
    {"+", PLUS}, {"-", MINUS}, {"*", MULT}, {"/", DIV}, {":=", ASSOP}, {"=", EQ}, {"/=", NEQ}, {"<", LTHAN}, {">", GTHAN}, {"<=", LTE}, {">=", GTE}, {"&", CONCAT}, {"**", EXP}, {",", COMMA}, {";", SEMICOL}, {"(", LPAREN}, {")", RPAREN}, {":", COLON}, {".", DOT}

};


map<Token, string> tokenToStringMap = {
    {IF, "IF"}, {ELSE, "ELSE"}, {ELSIF, "ELSIF"}, {PUT, "PUT"}, {PUTLN, "PUTLN"}, {GET, "GET"}, {INT, "INT"}, {FLOAT, "FLOAT"}, {CHAR, "CHAR"}, {STRING, "STRING"}, {BOOL, "BOOL"}, {PROCEDURE, "PROCEDURE"}, {TRUE, "TRUE"}, {FALSE, "FALSE"}, {END, "END"}, {IS, "IS"}, {BEGIN, "BEGIN"}, {THEN, "THEN"}, {CONST, "CONST"}, {IDENT, "IDENT"}, {ICONST, "ICONST"}, {FCONST, "FCONST"}, {SCONST, "SCONST"}, {BCONST, "BCONST"}, {CCONST, "CCONST"}, {PLUS, "PLUS"}, {MINUS, "MINUS"}, {MULT, "MULT"}, {DIV, "DIV"}, {ASSOP, "ASSOP"}, {EQ, "EQ"}, {NEQ, "NEQ"}, {EXP, "EXP"}, {CONCAT, "CONCAT"}, {GTHAN, "GTHAN"}, {LTHAN, "LTHAN"}, {LTE, "LTE"}, {GTE, "GTE"}, {AND, "AND"}, {OR, "OR"}, {NOT, "NOT"}, {MOD, "MOD"}, {COMMA, "COMMA"}, {SEMICOL, "SEMICOL"}, {LPAREN, "LPAREN"}, {RPAREN, "RPAREN"}, {DOT, "DOT"}, {COLON, "COLON"}, {ERR, "ERR"}, {DONE, "DONE"}};


string toLower(const string& str) {
    string lowstring = str;
    transform(lowstring.begin(), lowstring.end(), lowstring.begin(), [](char c) {
        return tolower(c);
    });
    return lowstring;
}

double convertScientificToDecimal(const std::string& scientificStr) {
    std::stringstream ss(scientificStr);
    double value;
    ss >> value;  // Convert the scientific notation string to a double
    return value;
}


string formatNumber(const string& numStr) {
    stringstream ss(numStr);
    double value;
    ss >> value; // Convert to double

    // Convert to string without scientific notation
    std::ostringstream out;
    out.precision(15);
    out << std::fixed << value;

    string result = out.str();

    // Remove trailing zeros
    result.erase(result.find_last_not_of('0') + 1, std::string::npos);

    // Remove trailing decimal point if necessary
    if (result.back() == '.') {
        result.pop_back();
    }

    return result;
}

bool isInteger(float f) {
    return floor(f) == f || ceil(f) == f;
}

enum TokState
{
    START,
    IDENTIFIER,
    INTEGER,
    FLOATINT,
    OTHER
};

 set<string> idents;
 set<string> numbers;
 set<string> scontandccont;
 vector<string> keywords;
vector<string> keywordsordered;

LexItem id_or_kw(const string &lexeme, int linenumber)
{
    if (reserved.find(lexeme) != reserved.end())
    {
        Token tokentype = reserved[lexeme];

        if (tokentype == TRUE || tokentype == FALSE)
        {
            LexItem lex(BCONST, lexeme, linenumber);
            return lex;
        }
        num_of_keywords++;
        return LexItem(tokentype, lexeme, linenumber);
    }
    num_of_identifiers++;
    return LexItem(IDENT, lexeme, linenumber);
}

ostream &operator<<(ostream &os, const LexItem &lexitem)
{
    switch (lexitem.GetToken())
    {
    case ICONST:
    case FCONST:
    case BCONST:

        os << tokenToStringMap[lexitem.GetToken()] << ": (" << lexitem.GetLexeme() << ")";
        break;
    case SCONST:
    case CCONST:
        num_of_chars_strings++;
        os << tokenToStringMap[lexitem.GetToken()] << ": " << lexitem.GetLexeme();
        break;
    case ERR:
        os << "ERR: In line " << lexitem.GetLinenum() << ", Error Message {" << lexitem.GetLexeme() << "}";
        break;
    case IDENT:
        os << "IDENT: <" << lexitem.GetLexeme() << ">";
        num_of_identifiers++;
        break;
    default:
        os << tokenToStringMap[lexitem.GetToken()];
        break;
    }
    return os;
}

LexItem getNextToken(string &s, int &linenumber)
{

    istringstream iss(s);
    regex ident("[a-zA-Z]");
    regex ident2("[a-zA-Z0-9_]");
    regex num("[0-9]");
    regex num2("[0-9+-eE]");
    regex flo("[.0-9+-eE]");

    char ch;
    string token = "";
    Token lextoken;
    TokState state = START;

    while (iss.get(ch))
    {

        string str(1, ch);

        switch (state)
        {

        case START:
            // covers IDENT, ICONST, SCONST, CCONST, COMMENT,
            if (regex_match(str, ident))
            {
                state = IDENTIFIER;
                lextoken = IDENT;
                token += str;
            }
            else if (regex_match(str, num))
            {
                state = INTEGER;
                lextoken = ICONST;
                token += str;
            }
            else if (str == "\"")
            {
                token += str;
                while (iss.get(ch))
                {
                    if (ch == '\''){
                        token += ch;
                        s = s.substr(token.length()); 
                        token = " Invalid string constant " + token;
                        LexItem lexitem(ERR, token, linenumber);
                        return lexitem;
                        
                    }
                    else if (ch != '\"')
                    {                // Check if the character is not a double quote
                        token += ch; // Add the character to the token
                    }
                    else
                    {

                        token += ch; // Add the closing double quote
                        s = s.substr(token.length());
                        LexItem lexitem(SCONST, token, linenumber);
                        scontandccont.insert(token);
                        return lexitem; // Return the token
                    }
                }
            }
            else if (str == "\'")
            {
                token += str;
                if (iss.get(ch) && isalpha(ch))
                {
                    token += ch;
                    if (iss.get(ch) && ch == '\'')
                    {
                        token += ch;
                        lextoken = CCONST;
                        s = s.substr(token.length());
                        LexItem lexitem(lextoken, token, linenumber);
                        scontandccont.insert(token);
                        return lexitem;
                    }
                    else
                    {
                        s = s.substr(token.length()); // FIX THIS use helper functin to fix it
                        LexItem lexitem(ERR, token, linenumber);
                        return lexitem;
                    }
                }
                s = s.substr(token.length()); // FIX THIS use helper functin to fix it
                LexItem lexitem(ERR, token, linenumber);
                return lexitem;
            }

            else if (operators.find(str) != operators.end())
            {
                if (str == "-" && static_cast<char>(iss.peek()) == '-')
                {
                    string comment;
                    getline(iss, comment);
                    s = s.substr(comment.length() + 1);
                    LexItem lexitem(DONE, "", linenumber);
                    return lexitem;
                }
                token += str;
                char peekChar = static_cast<char>(iss.peek());
                std::string nextChar(1, peekChar);
                if (operators.find(nextChar) != operators.end())
                {
                    token += nextChar;
                }
                s = s.substr(token.length());
                LexItem lexitem(operators[token], str, linenumber);
                return lexitem;
            }
            else if (str == " " || str == "\n" || str == "\t")
            {
                s = s.substr(1);
                continue;
            }

            else
            {
                state = OTHER;
                token += str;
                s = s.substr(token.length()); 
                return LexItem(ERR, token, linenumber);
            }
            break;
        case IDENTIFIER:
            if (str == "_"){
                if (iss.peek() == '_'){
                    token += str;
                    s = s.substr(token.length());
                    transform(token.begin(), token.end(), token.begin(), ::tolower);
                    LexItem lexitem(lextoken, token, linenumber);
                    idents.insert({token});  
                    return lexitem;
                }
            
            }
            if (regex_match(str, ident2))
            {
                token += str;
            }
            else
            {


                if (token == "true" || token == "false")
                {
                s = s.substr(token.length());
                lextoken = BCONST;
                LexItem lexitem(lextoken, token, linenumber);
                return lexitem;
                }


                if (reserved.find(toLower(token)) != reserved.end()) {
                    s = s.substr(token.length());
                    LexItem lexitem(reserved[toLower(token)], token, linenumber); // Use toLower to check against reserved keywords
                    keywords.push_back(toLower(token));  
                    return lexitem;
                }

                
                iss.putback(ch);
                s = s.substr(token.length());
                transform(token.begin(), token.end(), token.begin(), ::tolower);
                LexItem lexitem(lextoken, token, linenumber);
                idents.insert({token});  
                return lexitem;
            }
            

            break;
        case INTEGER:
            if (str == ".")
            {
                token += str;
                lextoken = FCONST;
                state = FLOATINT;
            }
            else if (regex_match(str, num2))
            {
                token += str;
            }
            else
            {
                iss.putback(ch);
                s = s.substr(token.length()); 
                double value = convertScientificToDecimal(token);
                string convertedValue;
                if (isInteger(value)) {
                    convertedValue = to_string(static_cast<int>(value)); 
                } else {
                    convertedValue = to_string(value);  
                }
              
                LexItem lexitem(ICONST, convertedValue, linenumber);
                num_of_numerals++;
                numbers.insert(convertedValue); 
                return lexitem;
            }
            break;
        case FLOATINT:
            if(str == "."){
                token += str;
                s = s.substr(token.length()); 
                LexItem lexitem(ERR, token, linenumber);
                return lexitem;
            }    
                
            if (regex_match(str, flo))
            {
                token += str;
            }
            else
            {
                iss.putback(ch);
                s = s.substr(token.length()); 
                num_of_numerals++;
                if (token.back() == 'E' || token.back() == 'e') {
                    token = token.substr(0, token.size() - 1);
                    s += 'e';  
                }
                
                
                LexItem lexitem(FCONST, token, linenumber);
              //  numbers.insert(to_string(formatNumber(to_string(convertScientificToDecimal(token))))); 
                numbers.insert(formatNumber(std::to_string(convertScientificToDecimal(token))));

                return lexitem;
            }
            break;

        case OTHER:
            s = s.substr(token.length()); 
            LexItem lexitem(ERR, token, linenumber);
            return lexitem;
        }
    }
    s = s.substr(token.length());
    if (s.length() == 0 && token.length() == 0)
    {
        lextoken = DONE;
    }
    LexItem lexitem(lextoken, token, linenumber);
    if (lexitem.GetToken() == IDENT){
        idents.insert({token});  
    } 
    else if (reserved.find(lexitem.GetLexeme()) != reserved.end()){
        keywords.push_back({token});
    }
    return lexitem;
}



int main(int argc, char *argv[]){

    // initialize flag booleans
    static bool all = false;
    static bool num = true;
    static bool str = true;
    static bool id = true;
    static bool kw = true;

    
    // no file given case
    /*
    if (argc < 2){
        cout << "No specified input file." << endl;
        return -1;
    }
    
*/
    string filename = "/Users/afshanzubia/cs280_projects/pa1/text.txt";
    ifstream file(filename);

    if (!file)
    {
        // cant open the file
        cout << "CANNOT OPEN THE FILE " << filename << endl;
        return -1;
    }

    string line;
    string word;
    int line_num = 0;
    int total_tokens = 0;

    map<string, string> tokenmap;

    int i = 2;

    set<string> flags;

    while (i < argc)
    {
        flags.insert(argv[i]);
        i++;
    }

    // define a map of flag strings & references to their respective boolean values

    map<string, bool *> bools = {
        {"-all", &all},
        {"-num", &num},
        {"-str", &str},
        {"-id", &id},
        {"-kw", &kw}};

    // if a flag is given as an argument, the respective boolean variable is turned true --> it will print out its output
    // if flag is unrecognized, it results in error
    for (const string &flag : flags)
    {

        if (bools.find(flag) != bools.end())
        {
            *bools[flag] = true;
        }
        else if (flag[0] == '-')
        {
            cout << "Unrecognized flag {" << flag << "}" << endl;
            return -1;
        }
        else
        {
            // another filename was given
            cout << "Only one file name is allowed." << endl;
            return -1;
        }
    }

    while (getline(file, line))
    {
        
        line_num++;
        while (line.length() != 0)
        {
            LexItem tokenitem;
            tokenitem = getNextToken(line, line_num);
            if (tokenitem.GetToken() == DONE)
            {
                continue;
            }
            else 
            {
                if(all){
                cout << tokenitem << endl;
                if (tokenitem.GetToken() == ERR){
                    return -1;
                }
            }
           
            total_tokens++;
        }
    }

 

    }
    
      if (line_num == 0)
    {
        cout << "Empty file." << endl;
        return -1;
    }

    
     cout << "\nLines: " << line_num << endl;
    cout << "Total Tokens: " << total_tokens << endl;
    cout << "Numerals: " << num_of_numerals << endl;
    cout << "Characters and Strings : " << scontandccont.size() << endl;
    cout << "Identifiers: " << idents.size() << endl;
    cout << "Keywords: " << keywords.size() << endl;

 

    if(id) {
        bool first = true;
        cout << "IDENTIFIERS: " << endl;
        for(set<string>::iterator it = idents.begin(); it != idents.end(); ++it){
            if(!first){
                cout << ", ";
            }
            cout <<*it;
            first = false;
        } 
        cout << "\n";
    }   
    if(kw){
        bool first = true;
        cout << "KEYWORDS: " << endl;
        
        /*
           // put keywords in order into keywordsordered : 
        
        // make keywords lowercase
        
        for(vector<string>::iterator it = keywords.begin(); it!= keywords.end(); ++it){
       for (char* c: *it){    
              c = tolower(c);
        
        }
        }
        
        
        
        
        // for each word in reservedInOrder,     
          //         if word is in the keywords2 (unsorted), && word is not in keywrodsordered
                    //     keywordsordered.append(word)
        
        
        // print out keywords ordered by iterating
        
        */
        
        set<string> keywords2;
        
        for(vector<string>::iterator it = keywords.begin(); it!= keywords.end(); ++it){
        
            keywords2.insert(*it);
        
        }
        
          for (const string& keyword : reservedInorder) {
                if (keywords2.find(keyword) != keywords2.end()) {
                keywordsordered.push_back(keyword);
                }
           }

        
        for(vector<string>::iterator it = keywordsordered.begin(); it!= keywordsordered.end(); ++it){
            if(!first){
                cout << ", ";
            }
            cout <<*it;
            first = false;
        }
        cout << "\n";
    }
    if(num){
        bool first = true;
        cout << "NUMERIC CONSTANTS: " << endl;
        for(set<string>::iterator it = numbers.begin(); it!= numbers.end(); ++it){
            if(!first){
                cout << ", ";
            }
            cout <<*it;
            first = false;
        }
        cout << "\n";
    }
    if(str){
        bool first = true;
        cout << "CHARACTERS AND STRINGS: " << endl;
        for(set<string>::iterator it = scontandccont.begin(); it!= scontandccont.end(); ++it){
            if(!first){
                cout << ", ";
            }
            cout <<*it;
            first = false;
        }
        cout << "\n";
    }
    
    
    
    
    return 0;
}
