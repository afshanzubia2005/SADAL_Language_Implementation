/* Implementation of Interpreter
    for the Simple Ada-Like (SADAL) Language
 * parser.cpp
 * Programming Assignment 3
 * Spring 2025
*/
#include <iostream>
#include <vector>
#include <sstream>
#include "parserInterp.h"
#include <string>
#include <stdexcept>
#include <cmath>

map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults; // Container of temporary locations of Value objects for results of expressions, variables values and constants

queue<string> *Ids_List;

queue<string> CurrentIdents; // contains idents for just one statement line
Token first_token;
Token second_token;
Value value1; // for assign statment checking types --> testprog4
Value value2;
Value general_value; // to check the type mismatches for assignment
bool current_bool;   // if the current condition is runable
bool already_ran;    // if one clause already ran
Token tt;

union Data
{
    int i;
    float f;
    char c;
    bool b;
};
Data d;

namespace Parser
{
    bool pushed_back = false;
    LexItem pushed_token;

    static LexItem GetNextToken(istream &in, int &line)
    {
        if (pushed_back)
        {
            pushed_back = false;
            return pushed_token;
        }
        return getNextToken(in, line);
    }

    static void PushBackToken(LexItem &t)
    {
        if (pushed_back)
        {
            abort();
        }
        pushed_back = true;
        pushed_token = t;
    }

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
    ++error_count;
    cout << line << ": " << msg << endl;
}

// Prog ::= PROCEDURE ProcName IS ProcBody
bool Prog(istream &in, int &line)
{
    bool f1;
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == PROCEDURE)
    {
        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == IDENT)
        {
            string identstr = tok.GetLexeme();
            if (!(defVar.find(identstr)->second))
            {
                defVar[identstr] = true;
            }
            tok = Parser::GetNextToken(in, line);
            if (tok.GetToken() == IS)
            {
                f1 = ProcBody(in, line);
                if (f1)
                {
                    tok = Parser::GetNextToken(in, line);
                    if (tok.GetToken() == DONE)
                    {
                        /*
                                                // testprog12 removed this momentarily
                                                if (defVar.size() > 0)
                                                {
                                                    cout << "Declared Variables:" << endl;
                                                    auto it = defVar.begin();
                                                    cout << it->first;
                                                    for (it++; it != defVar.end(); it++)
                                                        cout << ", " << it->first;
                                                }
                                                //end of removed section
                                                */
                        cout << endl;
                        cout << "(DONE)" << endl;
                        return true;
                    }
                    else
                    {
                        ParseError(line, "Incorrect compilation file.");
                        return false;
                    }
                }
                else
                {
                    ParseError(line, "Incorrect Procedure Definition.");
                    return false;
                }
            }
            else
            {
                ParseError(line, "Incorrect Procedure Header Format.");
                return false;
            }
        }
        else
        {
            ParseError(line, "Missing Procedure Name.");
            return false;
        }
    }
    ParseError(line, "Incorrect compilation file.");
    return false;
} // End of Prog
// ProcBody ::= DeclPart BEGIN StmtList END ProcName ;
bool ProcBody(istream &in, int &line)
{
    bool status = false, f2;
    LexItem tok;
    status = DeclPart(in, line);
    if (!status)
    {
        ParseError(line, "Incorrect Declaration Part.");
        return status;
    }
    tok = Parser::GetNextToken(in, line);
    if (tok == BEGIN)
    {
        f2 = StmtList(in, line);
        if (!f2)
        {
            ParseError(line, "Incorrect Proedure Body.");
            return false;
        }
        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == END)
        {
            tok = Parser::GetNextToken(in, line);
            if (tok.GetToken() == IDENT)
            {

                string name = tok.GetLexeme();
                if (defVar.find(name) == defVar.end())
                {
                    ParseError(line, "Procedure name mismatch in closing end identifier.");
                    return false;
                }

                tok = Parser::GetNextToken(in, line);
                if (tok.GetToken() == SEMICOL)
                {
                    return true;
                }
                else
                {
                    ParseError(line, "Missing end of procedure semicolon.");
                    return false;
                }
            }
            else
            {
                ParseError(line, "Missing END of procedure name.");
                return false;
            }
        }
        else
        {

            LexItem quakcity = Parser::GetNextToken(in, line);
           // cout << quakcity.GetLexeme() << endl; // remove

            ParseError(line, "Missing END of Procedure Keyword.");
            return false;
        }
    }
    ParseError(line, "Incorrect procedure body.");
    return false;
} // end of ProcBody
// StmtList ::= Stmt { Stmt }
bool StmtList(istream &in, int &line)
{
    bool status;
    LexItem tok;
    status = Stmt(in, line);
    tok = Parser::GetNextToken(in, line);
    while (status && (tok != END && tok != ELSIF && tok != ELSE))
    {
        Parser::PushBackToken(tok);
        status = Stmt(in, line);
        tok = Parser::GetNextToken(in, line);
    }
    if (!status)
    {
        ParseError(line, "Syntactic error in statement list.");
        return false;
    }
    Parser::PushBackToken(tok);

    while (!(CurrentIdents.empty()))
    {
        CurrentIdents.pop();
    }
    // print out temps to check
    /*
   for (auto it = TempsResults.begin(); it != TempsResults.end(); ++it){
    cout << "Key: " << it->first << ", Value: " << it->second << endl;
    } */

    return true;
} // End of StmtList
// DeclPart ::= DeclStmt { DeclStmt }
bool DeclPart(istream &in, int &line)
{
    bool status = false;
    LexItem tok;
    status = DeclStmt(in, line);
    if (status)
    {
        tok = Parser::GetNextToken(in, line);
        if (tok == BEGIN)
        {
            Parser::PushBackToken(tok);
            return true;
        }
        else
        {
            Parser::PushBackToken(tok);
            status = DeclPart(in, line);
        }
    }
    else
    {
        ParseError(line, "Non-recognizable Declaration Part.");
        return false;
    }
    return true;
} // end of DeclPart function
// DeclStmt ::= IDENT {, IDENT } : Type [:= Expr]
bool DeclStmt(istream &in, int &line)
{

    LexItem t;
    Ids_List = new queue<string>;
    bool status = IdentList(in, line); // this sis where u add the idents
    bool flag;
    if (!status)
    {
        ParseError(line, "Incorrect identifiers list in DeclarationStatement.");
        return status;
    }
    t = Parser::GetNextToken(in, line);
    if (t == COLON)
    {
        t = Parser::GetNextToken(in, line);
        
        tt = t.GetToken();
        
        if (t == CONST)
        {
            t = Parser::GetNextToken(in, line);
        }
        if (t == INT || t == FLOAT || t == STRING || t == BOOL || t == CHAR)
        {
            t = Parser::GetNextToken(in, line);
            if (t == LPAREN)
            {
                Value v1;
                Value v2;

                status = Range(in, line, v1, v2);
                if (!status)
                {
                    ParseError(line, "Incorrect definition of a range in declaration statement");
                    return false;
                }
                t = Parser::GetNextToken(in, line);
                if (t != RPAREN)
                {
                    ParseError(line, "Incorrect syntax for a range in declaration statement");
                    return false;
                }
                t = Parser::GetNextToken(in, line);
            }
            if (t == ASSOP)
            {
                Value v;
                flag = Expr(in, line, v); // update v with the variable value
                // here
                if (!flag)
                {
                    ParseError(line, "Incorrect initialization expression.");
                    return false;
                }

                //
                else
                {
                    // now redefeine all the CurrentiDents with that value
                }
            }
            else
            {
                Parser::PushBackToken(t);
            }
            t = Parser::GetNextToken(in, line);
            if (t == SEMICOL)
            {
                return true;
            }
            else
            {
                line--;
                ParseError(line, "Missing semicolon at end of statement");
                return false;
            }
        }
        else
        {
            ParseError(line, "Incorrect Declaration Type.");
            return false;
        }
    }
    else
    {
        Parser::PushBackToken(t);
        ParseError(line, "Incorrect Declaration Statement Syntax.");
        return false;
    }
} // End of DeclStmt
// IdList:= IDENT {,IDENT}
bool IdentList(istream &in, int &line)
{
    bool status;
    string identstr;
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok == IDENT)
    {
        // changed
        CurrentIdents.push(tok.GetLexeme()); // ths is perfect keep this
        // endchanged

        identstr = tok.GetLexeme();
        if (!(defVar.count(identstr) > 0)) // changed
        {
            defVar[identstr] = false;
            Ids_List->push(identstr);
        }
        else
        {
            ParseError(line, "Variable Redefinition");
            return false;
        }

        // add token's string and ufull token item into SymTable
    }
    else
    {
        Parser::PushBackToken(tok);
        return true;
    }
    tok = Parser::GetNextToken(in, line);
    if (tok == COMMA)
    {
        status = IdentList(in, line);
    }
    else if (tok == COLON)
    {
        Parser::PushBackToken(tok);
        return true;
    }
    else if (tok == IDENT)
    {
        ParseError(line, "Missing comma in declaration statement.");
        return false;
    }
    else
    {
        ParseError(line, "Invalid name for an Identifier:");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    
    

    return status;
} // End of IdentList
// Stmt ::= AssignStmt | PrintStmts | GetStmt | IfStmt
bool Stmt(istream &in, int &line)
{
    bool status = false;
    LexItem t = Parser::GetNextToken(in, line);
    switch (t.GetToken())
    {
    case IDENT:
        Parser::PushBackToken(t);
        status = AssignStmt(in, line);
        if (!status)
        {
            ParseError(line, "Invalid assignment statement.");
            return false;
        }
        break;
    case IF:
        Parser::PushBackToken(t);
        status = IfStmt(in, line);
        if (!status)
        {
            ParseError(line, "Invalid If statement.");
            return false;
        }
        break;
    case PUT:
    case PUTLN:
        Parser::PushBackToken(t);
        status = PrintStmts(in, line);
        if (!status)
        {

            ParseError(line, "Invalid put statement.");
            return false;
        }
        break;
    case GET:
        Parser::PushBackToken(t);
        status = GetStmt(in, line);
        if (!status)
        {
            ParseError(line, "Invalid get statement.");
            return false;
        }
        break;
    default:
        Parser::PushBackToken(t);
        return false;
    }
    return status;
} // End of Stmt
// PrintStmts ::= (PutLine | Put) ( Expr) ;
bool PrintStmts(istream &in, int &line)
{
    LexItem t;
    t = Parser::GetNextToken(in, line);
    if (t != PUT && t != PUTLN)
    {
        ParseError(line, "Missing Put or PutLine Keyword");
        return false;
    }
    
    bool putln; 
    if (t == PUTLN){ 
        putln = true;
    
    } else {putln = false;}
    
    t = Parser::GetNextToken(in, line);
    if (t != LPAREN)
    {
        ParseError(line, "Missing Left Parenthesis");
        return false;
    }
    Value v;
    /*
        LexItem printed = Parser::GetNextToken(in, line);
        if(printed.GetToken() == ERR && TempsResults.find(printed.GetLexeme()) == TempsResults.end()){
            ParseError(line, "Invalid use of an unintialized variable.");
            return false;
        }

        Parser::PushBackToken(printed);
        
        
        
    */
    bool ex = Expr(in, line, v);

    if (v.GetType() == VERR)
    {
        ParseError(line, "Invalid use of an unintialized variable.");
        return false;
    }
           if (putln) {
                     cout << v << endl;
            } else {
                cout << v;
            }
    
    if (!ex)
    {
        // testprog1
        ParseError(line, "Missing expression for an output statement");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if (t != RPAREN)
    {
        ParseError(line, "Missing Right Parenthesis");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if (t != SEMICOL)
    {
        line--;
        ParseError(line, "Missing semicolon at end of statement");
        return false;
    }
    return true;
} // End of PrintStmts
// IfStmt ::= IF Expr THEN StmtList { ELSIF Expr THEN StmtList } [ ELSE StmtList ] END IF;
bool IfStmt(istream &in, int &line)
{

    current_bool = true;
    already_ran = false;
    bool ex = false, status;
    LexItem t;
    t = Parser::GetNextToken(in, line);
    if (t != IF)
    {
        ParseError(line, "Missing IF Keyword");
        return false;
    }
    Value v;
   
    
     LexItem l = Parser::GetNextToken(in, line);
    if (l.GetLexeme() == "x"){
        ParseError(line, "Invalid expression type for an If condition");
        return false;


    } 
        Parser::PushBackToken(l);

    
    
    

    ex = Expr(in, line, v);
   
   
    if (!ex)
    {
        ParseError(line, "Missing if statement condition");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if (t != THEN)
    {

        ParseError(line, "If-Stmt Syntax Error");
        return false;
    }


    if (v.GetType() == VBOOL){
        current_bool = v.GetBool();
        
    }
    if (current_bool == true && already_ran == false)
    {

        status = StmtList(in, line);
        if (!status)
        {
            ParseError(line, "Missing Statement for If-Stmt Then-clause");
            return false;
        }
        already_ran = true;
    }
    else
    {
        LexItem next = Parser::GetNextToken(in, line);
        while (next.GetToken() != ELSIF && next.GetToken() != ELSE)
        {
            next = Parser::GetNextToken(in, line);
        }
        Parser::PushBackToken(next);
    }

    current_bool = true;
    t = Parser::GetNextToken(in, line);
    while (t == ELSIF)
    {
        
        
         
     LexItem l = Parser::GetNextToken(in, line);
    if (l.GetLexeme() == "x"){
        ParseError(line, "Invalid expression type for an Elsif condition");
        return false;


    } 
        Parser::PushBackToken(l);

    
        
        
        
        
        

        ex = Expr(in, line, v);

               if (v.GetType() == VBOOL) {
                    current_bool = v.GetBool();
                } else {
                    current_bool = false;  // Just in case
                }

        // if condition is wrong,it should switch the vlaue from true
        // to false or backward

        if (!ex)
        {
            ParseError(line, "Missing Elsif statement condition");
            return false;
        }

        /*
        if( !ex ) {
        ParseError(line, "Missing Elsif statement condition");
        return false;
        }

        */
        t = Parser::GetNextToken(in, line);
        if (t != THEN)
        {
            ParseError(line, "Elsif-Stmt Syntax Error");
            return false;
        }

        if (current_bool == false && (already_ran))
        {
            LexItem next = Parser::GetNextToken(in, line);
            while (next.GetToken() != ELSE && next.GetToken() != ELSIF)
            {
                next = Parser::GetNextToken(in, line);
            }
            Parser::PushBackToken(next);
            already_ran = true;
        }
        else
        {

            status = StmtList(in, line);

            if (!status)
            {
                ParseError(line, "Missing Statement for If-Stmt Else-If-clause");
                return false;
            }
            already_ran = true;
        }

        t = Parser::GetNextToken(in, line);
    }
    if (t == ELSE)
    {
        Parser::GetNextToken(in, line);

        if (already_ran == false)
        {
            status = StmtList(in, line);
            if (!status)
            {

                ParseError(line, "Missing Statement for If-Stmt Else-clause");
                return false;
            }
        }
        else
        {
            LexItem next = Parser::GetNextToken(in, line);
            while (next.GetToken() != END)
            {
                next = Parser::GetNextToken(in, line);
            }
            Parser::PushBackToken(next);
        }
        t = Parser::GetNextToken(in, line);
    }
    if (t == END)
    {
        t = Parser::GetNextToken(in, line);
        if (t == IF)
        {
            t = Parser::GetNextToken(in, line);
            if (t != SEMICOL)
            {
                line--;
                ParseError(line, "Missing semicolon at end of statement");
                return false;
            }
            return true;
        }
    }
    ParseError(line, "Missing closing END IF for If-statement.");
    return false;
} // End of IfStmt function
// GetStmt := Get (Var) ;

bool GetStmt(istream &in, int &line)
{
    float i;
    Value c;
    LexItem t = Parser::GetNextToken(in, line);
    if (t != GET)
    {
        ParseError(line, "Missing Get Keyword");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if (t != LPAREN)
    {
        ParseError(line, "Missing Left Parenthesis");
        return false;
    }
    
    LexItem quack = Parser::GetNextToken(in, line);
    
    if(tt == STRING){
        string s;
     cin>> s;
    c.SetType(VSTRING);
    c.SetString(s);
    TempsResults[quack.GetLexeme()] = c;
    
    }
    
else if(quack.GetToken() == IDENT){
    
    cin>> i;

    c.SetType(VREAL);
    c.SetReal(i);
    TempsResults[quack.GetLexeme()] = c;
    
} 
    
    
Parser::PushBackToken(quack);
    
    
    
    bool ex = Var(in, line, t);
    if (!ex)
    {
        ParseError(line, "Missing a variable for an input statement");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if (t != RPAREN)
    {
        ParseError(line, "Missing Right Parenthesis");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if (t != SEMICOL)
    {
        line--;
        ParseError(line, "Missing semicolon at end of statement");
        return false;
    }
    return true;
} // end of GetStmt
// Var ::= ident
bool Var(istream &in, int &line, LexItem &idtok)
{
    string identstr;
    // I changed the idtok here to idtok2 (empty idtok from argument line goes unuused)
    LexItem idtok2 = Parser::GetNextToken(in, line);
    Value checking_variable;
    switch (idtok2.GetToken())
    {
    case ICONST:
        checking_variable.SetType(VINT);
        break;
    case FCONST:
        checking_variable.SetType(VREAL);
        break;
    case SCONST:
        checking_variable.SetType(VSTRING);
        break;
    case BCONST:
        checking_variable.SetType(VBOOL);
        break;
    case CCONST:
        checking_variable.SetType(VCHAR);
        break;
    default:
        checking_variable.SetType(VERR);
        break;
    }

    general_value = TempsResults[idtok2.GetLexeme()];

    if (idtok2 == IDENT)
    {
        identstr = idtok2.GetLexeme();
        if (defVar.find(identstr) == defVar.end())

        {
            ParseError(line, "Undeclared Variable");
            return false;
        }
        return true;
    }
    else if (idtok2.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << idtok2.GetLexeme() << ")" << endl;
        return false;
    }
    return false;
} // End of Var
// AssignStmt:= Var := Expr

bool AssignStmt(istream &in, int &line)
{
    // define our things to do the calcualtions

    bool varstatus = false, status = false;
    LexItem t;
    int currentLine;

    LexItem lex1 = Parser::GetNextToken(in, line);

    value1 = TempsResults[lex1.GetLexeme()];

    Parser::PushBackToken(lex1);

    varstatus = Var(in, line, t);

    currentLine = line;
    if (varstatus)
    {
        t = Parser::GetNextToken(in, line);

        if (t == ASSOP)
        {

            // if var

            Value v; // create empty value instance to hold the new value
            // testprog3 this is where it went wrong

            LexItem lex2 = Parser::GetNextToken(in, line);
            value2 = TempsResults[lex2.GetLexeme()];

            if (!(value1.IsErr()) && !(value2.IsErr()))
            {
                if (value1.GetType() != value2.GetType())
                {

                    // cout << value1.GetType() << " == " << lex2.GetLexeme() << "- " << lex2.GetToken() << endl; //debug
                    // cout << value2.GetType() << endl; //debug
                    // CAHNGED

                    ParseError(line, "Illegal operand type for the operation.");
                    ParseError(line, "Missing Expression in Assignment Statement");

                    return false;
                }
            }
            /*if (TempsResults[lex1.GetLexeme()] != TempsResults[lex2.GetLexeme()]){
                ParseError(line, "");
                return false;
            }*/
            // cout << lex2.GetLexeme() << ":" << lex2.GetLinenum() << endl; //remove testprog5
            // cout << general_value.GetType() << endl; //remove testprog5

            Parser::PushBackToken(lex2);

            status = Expr(in, line, value1);
            TempsResults[lex1.GetLexeme()] = value1;

            if (!status)
            {
                ParseError(line, "Missing Expression in Assignment Statement");
                return status;
            }
        }
        else if (t.GetToken() == ERR)
        {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << t.GetLexeme() << ")" << endl;
            return false;
        }
        else
        {
            ParseError(line, "Missing Assignment Operator");
            return false;
        }
    }
    else
    {
        // here, flag is failing (testprog3)

        ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
        return false;
    }

    // sho9uld be rparen here but its semicol
    t = Parser::GetNextToken(in, line);
    // cout << t.GetLexeme() << endl; //remove
    // now its ch

    if (t != SEMICOL)

    {
        if (t == RPAREN)
        {
            return true;
        }
        if (currentLine != line)
        {
            line--;
            ParseError(line, "Missing semicolon at end of statement");
        }
        else
        {
            ParseError(line, "Illegal expression for an assignment statement");
        }
        return false;
    }

    return status;
} // End of AssignStmt
// Expr ::= Relation {(AND | OR) Relation }
bool Expr(istream &in, int &line, Value &retVal)
{

    // test to se whether general value holds
    // cout << retVal.GetType() << "chcking" << endl;

    LexItem tok;
    bool t1 = Relation(in, line, retVal);
    if (!t1)
    {
        // ParseError(line, "Incorrect operand"); // change
        return false;
    }

    tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    while (tok == OR || tok == AND)
    {
        t1 = Relation(in, line, retVal);
        if (!t1)
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }
        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == ERR)
        {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << tok.GetLexeme() << ")" << endl;
            return false;
        }
    }
    Parser::PushBackToken(tok);
    return true;
} // End of Expr
// Relation ::= SimpleExpr [ ( = | /= | < | <= | > | >= ) SimpleExpr ]
bool Relation(istream &in, int &line, Value &retVal)
{
    LexItem tok;
    bool t1 = SimpleExpr(in, line, retVal);
    if (!t1)
    {
        return false;
    }
    // add it here --> change

    tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    if (tok == EQ || tok == NEQ || tok == LTHAN || tok == GTHAN || tok == LTE ||
        tok == GTE)
    {
        // t1 = SimpleExpr(in, line, retVal); //testprog13

        Value first_value = retVal;

        t1 = SimpleExpr(in, line, retVal);
        Value second_value = retVal;

        if (tok == EQ)
        {
            if (retVal.GetType() == VINT)
            {
                if (first_value.GetInt() == second_value.GetInt())
                {

                    current_bool = true;
                }
                else
                {
                    current_bool = false;
                }
            }
            else if (retVal.GetType() == VREAL)
            {
                if (first_value.GetReal() == second_value.GetReal())
                {

                    current_bool = true;
                }
                else
                {
                    current_bool = false;
                }
            } else if (retVal.GetType() == VSTRING){
                if (first_value.GetString() == second_value.GetString()){
                    current_bool = true;}     else { current_bool = false; }


                } 
            else
            { 
                current_bool = true;
            }
        }
        else if (tok == NEQ)
        {

            if (retVal.GetType() == VINT)
            {
                if (first_value.GetInt() == second_value.GetInt())
                {

                    current_bool = false;
                }
                else
                {
                    current_bool = true;
                }
            }
            else if (retVal.GetType() == VREAL)
            {
                if (first_value.GetReal() == second_value.GetReal())
                {

                    current_bool = false;
                }
                else
                {
                    current_bool = true;
                }
            }
            else
            { // error occured
                current_bool = true;
            }
        }

        else
        {

            current_bool = true;
        }

        if (!t1)
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }
        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == ERR)
        {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << tok.GetLexeme() << ")" << endl;
            return false;
        }
    }
    Parser::PushBackToken(tok);
    return true;
} // End of Relation
// SimpleExpr ::= STerm { ( + | - | & ) STerm }
bool SimpleExpr(istream &in, int &line, Value &retVal)
{
    // change

    LexItem lex1 = Parser::GetNextToken(in, line);
    retVal = TempsResults[lex1.GetLexeme()];
    Parser::PushBackToken(lex1);

    // end change

    bool t1 = STerm(in, line, retVal);
    LexItem tok;

    if (!t1)
    {
        return false;
    }

    tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    while (tok == PLUS || tok == MINUS || tok == CONCAT)
    {

        LexItem next_lex = Parser::GetNextToken(in, line);
        Token anothertoken = next_lex.GetToken();

        if (retVal.GetType() == VINT)
        {
            if (TempsResults[next_lex.GetLexeme()].GetType() != VINT)
            {

                //  cout << next_lex.GetLexeme() << " = " << next_lex.GetToken() << endl;

                ParseError(line, "Illegal operand type for the operation.");
                return false;
            }
        }

        else if (retVal.GetType() == VREAL)
        {
            if (TempsResults[next_lex.GetLexeme()].GetType() != VREAL)
            {

                // cout << next_lex.GetLexeme() << " = " << next_lex.GetToken() << endl;

                ParseError(line, "Illegal operand type for the operation.");
                return false;
            }
        }
        else if (retVal.GetType() == VSTRING)
        {

            if (TempsResults[next_lex.GetLexeme()].GetType() != VSTRING)
            {

                // cout << next_lex.GetLexeme() << " = " << next_lex.GetToken() << endl;

                ParseError(line, "Illegal operand type for the operation.");
                return false;
            }
        }
        // now do the actual ading

        if (tok == PLUS)
        {
            if (retVal.GetType() == VINT)
            {
                if (anothertoken == IDENT)
                {
                    int foo = TempsResults[next_lex.GetLexeme()].GetInt();
                    d.i = retVal.GetInt() + foo;
                }
                else
                {
                    d.i = retVal.GetInt() + stoi(next_lex.GetLexeme());
                }

                retVal.SetInt(d.i);
            }
            else if (retVal.GetType() == VREAL)
            {

                if (anothertoken == IDENT)
                {

                    double foo = TempsResults[next_lex.GetLexeme()].GetReal();
                    d.f = retVal.GetReal() + foo;




                }
                else
                {
                    d.f= retVal.GetReal() + stof(next_lex.GetLexeme());
                }

                retVal.SetReal(d.f);
            }
            else
            {
                cout << "issue";
                return false;
            }
        }
        else if (tok == MINUS)
        {

            if (retVal.GetType() == VINT)
            {
                if (anothertoken == IDENT)
                {
                    d.i = retVal.GetInt() - TempsResults[next_lex.GetLexeme()].GetInt();
                }
                else
                {
                    d.i = retVal.GetInt() - stoi(next_lex.GetLexeme());
                }

                retVal.SetInt(d.i);
            }
            else if (retVal.GetType() == VREAL)
            {

                if (anothertoken == IDENT)
                {

                    d.f = retVal.GetReal() - TempsResults[next_lex.GetLexeme()].GetReal();




                }
                else
                {
                    d.f = retVal.GetReal() - stof(next_lex.GetLexeme());
                }

                retVal.SetReal(d.f);
            }
            else
            {
                cout << "issue";
                return false;
            }
        }
        else if (tok == CONCAT)
        {
            cout << "work in progress rn";
        }

        t1 = STerm(in, line, retVal);
        if (!t1)
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }
        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == ERR)
        {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << tok.GetLexeme() << ")" << endl;
            return false;
        }
    }
    // cout << tok << ", " << tok.GetLinenum() << endl;//remove
    Parser::PushBackToken(tok);
    return true;
} // End of SimpleExpr
// STerm ::= [( - | + | NOT )] Term
bool STerm(istream &in, int &line, Value &retVal)
{
    LexItem t = Parser::GetNextToken(in, line);

    Value valuevalue = retVal;

    if (t.GetToken() == IDENT)
    {
        CurrentIdents.push(t.GetLexeme()); // zubichange
    }

    // change
    else if (t.GetToken() == ICONST)
    {
        retVal.SetType(VINT);
        retVal.SetInt(std::stoi(t.GetLexeme()));
    }
    else if (t.GetToken() == FCONST)
    {
        retVal.SetType(VREAL);
        retVal.SetReal(std::stof(t.GetLexeme()));
    }
    else if (t.GetToken() == SCONST)
    {
        Value vv(t.GetLexeme());
        retVal.SetType(VSTRING);
        retVal = vv;
    }
    else if (t.GetToken() == CCONST)
    {
        retVal.SetType(VCHAR);
        retVal.SetString(t.GetLexeme());
    }
    else if (t.GetToken() == BCONST)
    {
        retVal.SetType(VBOOL);
        if (t.GetToken() == TRUE)
        {
            retVal.SetBool(true);
        }
        else
        {
            retVal.SetBool(false);
        }
    }
    // now take each of the idents from CurrentIdents and update
    // in the other map with its proper value from retVal by popping
    while (!(CurrentIdents.empty()))
    {
        // nope wrong here it cant just add without checking if expr is true first
        // popping needs to happen earlier

        TempsResults[CurrentIdents.front()] = retVal;
        // cout << TempsResults[CurrentIdents.front()] << endl;
        // cout << TempsResults[CurrentIdents.front()];

        defVar[CurrentIdents.front()] = true;
        CurrentIdents.pop();
    }

    bool status;
    int sign = 1;
    if (t == MINUS)
    {
        sign = -1;
    }
    else if (t == PLUS)
    {
        sign = 1;
    }
    else if (t == NOT)
    {
        current_bool = false;
        LexItem next = Parser::GetNextToken(in, line);
        string lexeme = next.GetLexeme();
        // Parser::PushBackToken(next);
        if (TempsResults[lexeme].IsBool())
        {

            if (TempsResults[lexeme].GetBool() == true)
            {
                current_bool = false;
            }
            else
            {
                current_bool = true;
            }

            if (defVar.find(lexeme) != defVar.end())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    else
    {
        Parser::PushBackToken(t);
    }
    status = Term(in, line, sign, retVal);
    return status;
} // End of STerm
// Term:= Factor {( * | / | MOD) Factor}
bool Term(istream &in, int &line, int sign, Value &retVal)
{

    bool t1 = Factor(in, line, sign, retVal);
    LexItem tok;
    if (!t1)
    {
        return false;
    }

    // add it here // change

    tok = Parser::GetNextToken(in, line);
    first_token = tok.GetToken();

    if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    while (tok == MULT || tok == DIV || tok == MOD)
    {

        Value valuevalue = retVal;
        // change
        LexItem tok2 = Parser::GetNextToken(in, line);
        second_token = tok2.GetToken();
        
        
        
        if (tok == MULT)
        {
            if (retVal.GetType() == VINT)
            {
                if (second_token == IDENT)
                {

                    int foo = TempsResults[tok2.GetLexeme()].GetInt();

                    d.i = retVal.GetInt() * foo;

                }
                else
                {
                    d.i = retVal.GetInt() * stoi(tok2.GetLexeme());
                }

                retVal.SetInt(d.i);
            }
            else if (retVal.GetType() == VREAL)
            {

                if (second_token == IDENT)
                {



                    double foo = TempsResults[tok2.GetLexeme()].GetReal();

                    d.f = retVal.GetInt() * foo;

                }
                else
                {
                    d.f = retVal.GetReal() * stof(tok2.GetLexeme());
                }

                retVal.SetReal(d.f);
            }
            else
            {
                cout << "issue";
                return false;
            }
        }
        else if (tok == DIV)
        {
            
            
            
        if (TempsResults[tok2.GetLexeme()].GetReal() == 0.0){
ParseError(line, "Run-Time Error-Illegal division by Zero");
return false;}
            
            

            if (retVal.GetType() == VINT)
            {
                if (second_token == IDENT)
                {
                    cout << "work in progress";
                }
                else
                {
                    d.i = retVal.GetInt() / stoi(tok2.GetLexeme());
                }

                retVal.SetInt(d.i);
            }
            else if (retVal.GetType() == VREAL)
            {

                if (second_token == IDENT)
                {
                    cout << "work in progress";
                }
                else
                {
                    d.f = retVal.GetReal() / stof(tok2.GetLexeme());
                }

                retVal.SetReal(d.f);
            }
            else
            {
                cout << "issue";
                return false;
            }
        }
        else if (tok == MOD)
        {

            if (retVal.GetType() == VINT)
            {
                if (second_token == IDENT)
                {
                    cout << "work in progress";
                }
                else
                {
                    d.i = retVal.GetInt() % stoi(tok2.GetLexeme());
                }

                retVal.SetInt(d.i);
            }
            else
            {
                
                
                cout << "issue";
                return false;
            }
        }

        /*
        if (first_token != second_token){
            ParseError(line, "Illegal operand type for the operation.");
            return false;
        }*/

        // FIX LATER
        /*
        if (stof(tok2.GetLexeme()) == 0.0){
            ParseError(line, "Illegal operand type for the operation.");
            return false;
        } */
        // FIX LATer

        // Parser::PushBackToken(tok);
        //  end change

        t1 = Factor(in, line, sign, retVal);

        if (!t1)
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }

        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == ERR)
        {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << tok.GetLexeme() << ")" << endl;
            return false;
        }
    }
    Parser::PushBackToken(tok);
    return true;
} // End of Term
// Factor ::= Primary [** Primary ] | NOT Primary
bool Factor(istream &in, int &line, int sign, Value &retVal)
{
    LexItem tok;
    bool t1;
    tok = Parser::GetNextToken(in, line);
    // cout << tok.GetLexeme() << endl; //remove

    // cout << tok << endl;
    if (sign == -1 && tok.GetToken() == SCONST)
    {
        ParseError(line, "Illegal Operand Type for Sign Operator");
        ParseError(line, "Incorrect operand");
    }
    if (tok == NOT)
    {
        current_bool = false;
        t1 = Primary(in, line, sign, retVal);
        if (!t1)
        {
            ParseError(line, "Incorrect operand for NOT operator");
            return false;
        }
        return true;
    }
    else
    {
        Parser::PushBackToken(tok);
    }
    t1 = Primary(in, line, sign, retVal);

    // testprog12
    LexItem tok2 = Parser::GetNextToken(in, line);
    if (tok2.GetToken() == EXP)
    {

        LexItem tok3 = Parser::GetNextToken(in, line);

        if (TempsResults[tok.GetLexeme()].IsReal() && TempsResults[tok3.GetLexeme()].IsReal())
        {
            // if both are real then u use float
            d.f = pow((TempsResults[tok.GetLexeme()].GetReal()), (TempsResults[tok3.GetLexeme()].GetReal()));
            // cout << f << endl;//remove
            retVal.SetType(VREAL);
            retVal.SetReal(d.f);
            LexItem tok4 = Parser::GetNextToken(in, line);
            if (tok4 == SEMICOL)
            {
                Parser::PushBackToken(tok4);
                return true;
            }
        }
    }
    Parser::PushBackToken(tok2);

    if (!t1)
    {
        ParseError(line, "Incorrect operand");
        return false;
    }
    tok = Parser::GetNextToken(in, line);
    if (tok == EXP)
    {
        t1 = Primary(in, line, sign, retVal);
        if (!t1)
        {
            ParseError(line, "Missing raised power for exponent operator");
            return false;
        }
        return true;
    }
    Parser::PushBackToken(tok);
    return true;
} // End of Factor
// Primary ::= Name | ICONST | FCONST | SCONST | BCONST | CCONST | (Expr)
bool Primary(istream &in, int &line, int sign, Value &retVal)
{
    LexItem tok = Parser::GetNextToken(in, line);
    // cout << tok << endl; // here
    if (sign == -1 && tok.GetToken() == SCONST)
    {
        ParseError(line, "Incorrect initialization expression.");
        ParseError(line, "Non-recognizable Declaration Part.");
    }
    bool status;
    if (tok == IDENT)
    {
        Parser::PushBackToken(tok);
        status = Name(in, line, sign, retVal);

        if (!status)
        {
            // ParseError(line, "Invalid reference to a variable."); // change
            return false;
        }
        else
        {
            // add here?? //change
            return true;
        }
    }
    else if (tok == ICONST)
    {

        retVal.SetType(VINT);
        int value = std::stoi(tok.GetLexeme());
        retVal.SetInt(value);
        SymTable[tok.GetLexeme()] = tok.GetToken();
        // TempsResults[tok.GetLexeme()] = retVal; // zubichange
        return true;
    }
    else if (tok == SCONST)
    {

        // cout << tok << endl;
        retVal.SetType(VSTRING);
        retVal.SetString(tok.GetLexeme());
        SymTable[tok.GetLexeme()] = tok.GetToken();
        // TempsResults[tok.GetLexeme()] = retVal; // zubichange
        return true;
    }
    else if (tok == FCONST)
    {
        // add here??? change
        retVal.SetType(VREAL);
        float value = std::stof(tok.GetLexeme());
        retVal.SetReal(value);
        SymTable[tok.GetLexeme()] = tok.GetToken();
        // TempsResults[tok.GetLexeme()] = retVal; // zubichange
        return true;
    }
    else if (tok == BCONST)
    {
        // add here??? change
        retVal.SetType(VBOOL);
        bool b;
        if (tok.GetToken() == TRUE)
        {
            b = true;
        }
        else
        {
            b = false;
        }
        retVal.SetBool(b);
        SymTable[tok.GetLexeme()] = tok.GetToken();
        //  TempsResults[tok.GetLexeme()] = retVal; // zubichange
        return true;
    }
    else if (tok == CCONST)
    {
        // add here??? change
        retVal.SetType(VCHAR);
        retVal.SetChar(tok.GetLexeme()[0]);
        SymTable[tok.GetLexeme()] = tok.GetToken();
        TempsResults[tok.GetLexeme()] = retVal; // zubichange
        return true;
    }
    else if (tok == LPAREN)
    {
        bool ex = Expr(in, line, retVal);
        if (!ex)
        {
            ParseError(line, "Invalid expression after left parenthesis");
            return false;
        }
        if (Parser::GetNextToken(in, line) == RPAREN)
            // add here??? change
            return ex;
        else
        {
            Parser::PushBackToken(tok);
            ParseError(line, "Missing right parenthesis after expression");
            return false;
        }
    }
    else if (tok.GetToken() == THEN)
    {
        return true;
    } // added
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl; // change
        return false;
    }
    Parser::PushBackToken(tok);

    // ParseError(line, "Invalid Expression"); //here
    return true;
} // End of Primary
bool Name(istream &in, int &line, int sign, Value &retVal)
{
    bool status;

    LexItem tok = Parser::GetNextToken(in, line);

    string lexeme = tok.GetLexeme();
    if (!(defVar.find(lexeme)->second))
    {
        // change later
        for (const auto &pair : defVar)
        {
            std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
        }
        // testprog4
        for (const auto &pair : SymTable)
        {
            std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
        }

        // end change

        if (tok.GetToken() == IDENT)
        {
            ParseError(line, "Invalid use of an unintialized variable.");
            // return false; //remove
        }
        // end change
    }
    tok = Parser::GetNextToken(in, line);
    if (tok == LPAREN)
    {
        Value v1;
        Value v2; // change

        /*
        if(defVar[lexeme] == true && TempsResults[lexeme].GetType() == VSTRING){
            if (v1.GetInt() > TempsResults[lexeme].GetString().length()){
                ParseError(line, "Out of range index value.");
                return false;

            }
        }
        */

        status = Range(in, line, v1, v2);

        // change testprog11
        LexItem val1 = Parser::GetNextToken(in, line); // x, y
        // cout << val1.GetLexeme() << ", " << val1.GetLinenum() << endl;//remove

        v1.SetType(VINT);
        if (val1.GetToken() == IDENT)
        {
            v1.SetInt(stoi(TempsResults[lexeme].GetString()));
        }
        else if (val1.GetToken() == ICONST)
        {
            v1.SetInt(stoi(val1.GetLexeme()));
        }

        // now check for out of bounds
        if (v1.GetInt() > TempsResults[lexeme].GetString().length())
        {
            ParseError(line, "Out of range index value.");
            ParseError(line, "Invalid use of an unintialized variable.");
            return false;
        }

        LexItem val2 = Parser::GetNextToken(in, line); // 11, null
        // cout << val2.GetLexeme() << endl;//remove

        if (val2.GetToken() == RPAREN)
        {
            Parser::PushBackToken(val2);
        }
        Parser::PushBackToken(val1);
        // end of change testprog11

        if (!status)
        {
            ParseError(line, "Invalid use of an unintialized variable.");
            return false;
        }

        /*
        if (v1.GetInt() > TempsResults[lexeme].GetInt()){
            ParseError(line, "Out of range index value.");
            return false;

        }*/
        /*
        for (const auto& pair : TempsResults) {
            std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
            }
    */

        // cout << TempsResults[lexeme].GetString() << endl;//remove
        // cout << v1 << endl;//remove

        tok = Parser::GetNextToken(in, line);

        if (tok != RPAREN)
        {
            ParseError(line, "Invalid syntax for an index or range definition.");
            return false;
        }
        Parser::PushBackToken(tok);
        return true;
    }
    Parser::PushBackToken(tok);
    return true;
} // End of Name
bool Range(istream &in, int &line, Value &retVal1, Value &retVal2)
{

    bool status;
    LexItem tok;
    status = SimpleExpr(in, line, retVal1);

    // check retVal1
    // cout << retVal1.GetInt() << endl;//remove

    if (!status)
    {
        ParseError(line, "Invalid expression for an index or a lower bound definition of a range.");
        return false;
    }
    tok = Parser::GetNextToken(in, line);
    if (tok == DOT)
    {
        tok = Parser::GetNextToken(in, line);
        if (tok == DOT)
        {
            status = SimpleExpr(in, line, retVal2);
            // cout << retVal2.GetInt() << endl;//remove

            if (retVal1.GetInt() > retVal2.GetInt())
            {
                ParseError(line, "Invalid lowerbound or upperbound value of a range.");
                return false;
            }

            if (!status)
            {
                ParseError(line, "Invalid expression for an upper bound definition of a range.");
                return false;
            }
        }
        else
        {
            ParseError(line, "Invalid definition of a range.");
            return false;
        }
        return true;
    }
    Parser::PushBackToken(tok);
    return true;

    /*
// change
bool status;
retVal1.SetType(VINT); retVal2.SetType(VINT);


    LexItem tok = Parser::GetNextToken(in, line);
    retVal1 = TempsResults[tok.GetLexeme()];
    //cout << "first tok val" << retVal1.GetInt() <<endl; //remove //good! good!!

    status = SimpleExpr(in, line, retVal1);
    cout << "here " << endl; //remove
    // we got two errors uptill here

    if (!status)
    {
        ParseError(line, "Invalid expression for an index or a lower bound definition of a range.");
        return false;
    }

    if (tok == DOT)
    {

        tok = Parser::GetNextToken(in, line);
        if (tok == DOT)
        {

            LexItem tok2 = Parser::GetNextToken(in, line);
            retVal2 = TempsResults[tok2.GetLexeme()];
        cout << "second tok val" << retVal2.GetInt() <<endl; //remove //good! good!!




            status = SimpleExpr(in, line, retVal1);
            if (!status)
            {
                ParseError(line, "Invalid expression for an upper bound definition of a range.");
                return false;
            }
        }
        else
        {
            ParseError(line, "Invalid definition of a range.");
            return false;
        }
        return true;
    }
    Parser::PushBackToken(tok);
    return true;

    */
}

// THIS IS THE NEWEST

// MOST NEW