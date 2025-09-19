#include <cctype>
#include <map>
#include <iostream>

using std::map;
using namespace std;
#include "lex.h"
#include "parser.h"


map<string, bool> defVar;
static bool if_statement;
static bool successful = true; //changed

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}
static int error_count = 0;
int sign = 1;

LexItem word;

int ErrCount()
{
    return error_count;
}


void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

using namespace std;
// Prog ::= PROCEDURE ProcName IS ProcBody 
bool Prog(istream& in, int& line){

	word= Parser::GetNextToken(in, line);

	if (word != PROCEDURE){
		ParseError(line, "Incorrect compilation file.");
		return false; 
	} 

	word= Parser::GetNextToken(in, line);
	if (word != IDENT){
		ParseError(line, "Missing Procedure Name.");
		return false;
	}
	LexItem procname = word;

	word= Parser::GetNextToken(in, line);
	if (word != IS) {
		return false;
	}

	if(!(ProcBody(in, line))) {
		ParseError(line, "Incorrect Procedure Definition.");
		return false;
	}

	// check if procname from ProcBody is the same as procname from Proc (its not always!)
	cout << "(Done)" << endl; //remove
	return true;

}


// ProcBody ::= DeclPart BEGIN StmtList END ProcName ;
bool ProcBody(istream& in, int& line){
	if(!(DeclPart(in, line))) {
		ParseError(line, "Incorrect procedure body.");
		return false;
	}
	if(word.GetToken() != BEGIN){
		return false;
	}

	if(!(StmtList(in, line))) {
		return false;
	}

	if(word.GetToken() == END && successful == true ){

		return true;
	}

	int curr_line = word.GetLinenum();
	word= Parser::GetNextToken(in, line); 
	word= Parser::GetNextToken(in, line); //remove
	if(word.GetToken() != END){
		if(curr_line != word.GetLinenum()){
			line = curr_line;
		}
		ParseError(line, "Syntactic error in statement list.");
		ParseError(line, "Incorrect Proedure Body.");
		return false;
	}	

	word= Parser::GetNextToken(in, line);
	if (word != IDENT){
		return false;
	}

// check if the word is equal to previous procname?
	word= Parser::GetNextToken(in, line);
	if (word != SEMICOL){
		return false;
	}
	return true;

};

//4. DeclPart ::= DeclStmt { DeclStmt }
 bool DeclPart(istream& in, int& line){

	if (!(DeclStmt(in, line))){
		ParseError(line, "Non-recognizable Declaration Part.");
		return false;
	}
	
	while(DeclStmt(in, line)){
        
		continue;
	}
     
	if(word != BEGIN){
        if(word.GetToken() == GET || word.GetToken() == PUT || word.GetToken() == IF){
          ParseError(line, "Incorrect Declaration Statement Syntax."); 
        }
		ParseError(line, "Non-recognizable Declaration Part.");
		return false;
	}
     

     
	/*
	while(word != BEGIN){
		if(!(DeclStmt(in, line))){
			ParseError(line, "Non-recognizable Declaration Part.");
			return false;
		}
	}
	
	// old vers
	while(DeclStmt(in, line)){
		continue;
	}
 	*/
	return true;
	
 };

// DeclStmt ::= IDENT {, IDENT } : [CONSTANT] Type [(Range)] [:= Expr] ;
 bool DeclStmt(istream& in, int& line) {

	map<string, bool> currVar; // list of variables defined in this current iteration 
	 // bool defined = false; // tells whether variables in this statement were defined  //unused

	word= Parser::GetNextToken(in, line);
	if (word == BEGIN){
		return false;
	}
	if(word != IDENT){
		return false;
	} else {
		if(defVar.find(word.GetLexeme()) != defVar.end()) {
			ParseError(line, "Variable Redefinition");
			ParseError(line, "Incorrect identifiers list in Declaration Statement.");
			return false;
		} else {
			defVar.insert({word.GetLexeme(), false});
			currVar.insert({word.GetLexeme(), false});
		}
		
	}
	word= Parser::GetNextToken(in, line);
	while(word == COMMA){
		word= Parser::GetNextToken(in, line);
		if(word != IDENT){
			return false;
		} else {
			if(defVar.find(word.GetLexeme()) != defVar.end()) {
				ParseError(line, "Variable Redefinition");
				ParseError(line, "Incorrect identifiers list in Declaration Statement.");
				return false;
			} else {
				defVar.insert({word.GetLexeme(), false});
				currVar.insert({word.GetLexeme(), false});
			}
		}
		word= Parser::GetNextToken(in, line);
	}
	if(word != COLON){
		if(word == IDENT){ParseError(line, "Missing comma in declaration statement.");}
        else {
            
            ParseError(line, "Invalid name for an Identifier:");
            cout << "\n(" << word << ")" << endl;
        
        } 
		ParseError(line, "Incorrect identifiers list in Declaration Statement.");
		return false;
	}
	word= Parser::GetNextToken(in, line);
	if (word == CONST){
		word= Parser::GetNextToken(in, line);
		
	} 

	if(!(Type(in, line))){
		return false;
	}

	word= Parser::GetNextToken(in, line);
	if(word == LPAREN){
		if(!(Range(in, line))){
			return false;
		}
		word= Parser::GetNextToken(in, line);

		if(word != RPAREN){
			return false;
		}

		word= Parser::GetNextToken(in, line);

	}

	if (word == ASSOP){
		if(!(Expr(in, line))){
			return false;
		}
		// defined = true; // unused
		word= Parser::GetNextToken(in, line);
	}
	
	if (word != SEMICOL){
		return false;
	}
	for (auto it = currVar.begin(); it != currVar.end(); ++it){
		defVar[it->first] = true;
	}
	return true;
	

 };


 // Type ::= INTEGER | FLOAT | BOOLEAN | STRING | CHARACTER
 bool Type(istream& in, int& line){

	//word= Parser::GetNextToken(in, line);
	if(word == INT || word == FLOAT || word == BOOL || word == STRING || word == CHAR ){
		return true;
	}
	ParseError(line, "Incorrect Declaration Type.");
	return false;
 };
// StmtList ::= Stmt { Stmt }
 bool StmtList(istream& in, int& line){

		// check the first word
	if (!(Stmt(in, line)))	{
		return false;
	} 

	// first stmt passed
	// second stmt = put stmt failed 
	while(Stmt(in, line)){

		continue;
	}

	return true;

 };

 //Stmt ::= AssignStmt | PrintStmts | GetStmt | IfStmt
 bool Stmt(istream& in, int& line){ 
	if (!(GetStmt(in, line)) && !(PrintStmts(in, line)) &&  !(IfStmt(in, line)) && !(AssignStmt(in, line))){
		return false;
	}
	return true;
 };

//PrintStmts ::= (PutLine | Put) ( Expr) ;
 bool PrintStmts(istream& in, int& line){
	word= Parser::GetNextToken(in, line);
	if (word != PUTLN && word != PUT){
		Parser::PushBackToken(word);
		return false;
	}
	word= Parser::GetNextToken(in, line);
	if(word.GetToken() != LPAREN){
		ParseError(line, "Missing Left Parenthesis"); 
		ParseError(line, "Invalid put statement."); 
		return false;
	}

	if(!(Expr(in, line))){
		ParseError(line, "Invalid put statement."); 
		return false;
	}

	word= Parser::GetNextToken(in, line);
	if(word.GetToken() != RPAREN){
		ParseError(line, "Missing Right Parenthesis"); 
		ParseError(line, "Invalid put statement."); 
		return false;
	}
	//debug line num here:
	int curr_line_num = word.GetLinenum();
	word= Parser::GetNextToken(in, line);
	if (word != SEMICOL){
		Parser::PushBackToken(word);
		if(line != curr_line_num){
			line = curr_line_num;
		}
		ParseError(line, "Missing semicolon at end of statement"); 
		ParseError(line, "Invalid put statement."); 
		return false;
	}
	return true;

 };

 // GetStmt := Get (Var) ;
 bool GetStmt(istream& in, int& line){
	word = Parser::GetNextToken(in, line);
	if(word != GET){
		Parser::PushBackToken(word); 
		return false;
	}
	word= Parser::GetNextToken(in, line);
	if(word != LPAREN){
		return false;
	}
	
	if(!(Var(in, line))){
		return false;
	}
	word= Parser::GetNextToken(in, line);
	if(word != RPAREN){
		return false;
	}

	word= Parser::GetNextToken(in, line);
	if (word != SEMICOL){
		return false;
	}
	return true;

 };

 //IfStmt ::= IF Expr THEN StmtList { ELSIF Expr THEN StmtList } [ELSE StmtList ] END IF ;
 bool IfStmt(istream& in, int& line){
	word= Parser::GetNextToken(in, line);
	if (word != IF){
		Parser::PushBackToken(word);
		return false;
	}

	if(!(Expr(in, line))){
		ParseError(line, "Missing if statement condition"); //change
		ParseError(line, "Invalid If statement."); //change
        
        if(word.GetToken() == SEMICOL){
			ParseError(line, "Syntactic error in statement list."); //change
			ParseError(line, "Missing Statement for If-Stmt Then-clause"); //change
			ParseError(line, "Invalid If statement."); //change

		}
        
		successful = false;
		return false;
	}
	word= Parser::GetNextToken(in, line);
	if (word != THEN){
		return false;
		
	}

	if(!(StmtList(in, line))){
		return false;
	}
	word= Parser::GetNextToken(in, line);
	while (word == ELSIF){
		if(!(Expr(in, line))){
			return false;
		}
		word= Parser::GetNextToken(in, line);
		if (word != THEN){
			return false;
		}
		if(!(StmtList(in, line))){
			return false;
		}
		word= Parser::GetNextToken(in, line);
	}
	if (word == ELSE){
		if(!(StmtList(in, line))){
			return false;
		}
		word= Parser::GetNextToken(in, line);
	}

	if(word != END){
      if_statement = true;
	  successful = false;

		// case 16: at this point word is else and line 24
		if(word.GetToken() == ELSE){

		ParseError(line, "Missing closing END IF for If-statement.");
		ParseError(line, "Invalid If statement.");
		word = Parser::GetNextToken(in, line); }//change
		return false;

	}

	word= Parser::GetNextToken(in, line);
	if(word != IF){
		return false;
	}

	word= Parser::GetNextToken(in, line);
	if(word != SEMICOL){
		return false;
	}

	return true;

 };

 // AssignStmt ::= Var := Expr ;
 bool AssignStmt(istream& in, int& line){
	
	if(!Var(in, line)){
		return false;
	}
	
	word= Parser::GetNextToken(in, line);
	if(word != ASSOP){
        ParseError(line, "Missing Assignment Operator");
		ParseError(line, "Invalid assignment statement.");
		return false;
	}

	if(!Expr(in, line)){
		ParseError(line, "Invalid assignment statement.");
		return false;
	}
	word= Parser::GetNextToken(in, line);
	if (word != SEMICOL){
		return false;
	}

	return true;
	
	
 };

 //Var ::= IDENT
 bool Var(istream& in, int& line){

	word= Parser::GetNextToken(in, line);
	if (word != IDENT){
		Parser::PushBackToken(word);
		return false;
	}
	return true;

 };

 // Expr ::= Relation {(AND | OR) Relation }
 bool Expr(istream& in, int& line){
	if(!(Relation(in, line))){
		//ParseError(line, "Missing Expression in Assignment Statement");  //change
		return false;
	}
	/*
	// FIX THIS LATER
	word= Parser::GetNextToken(in, line);
	while(word == AND || word == OR){
		if(!(Relation(in, line))){
			return false;
		}
		word= Parser::GetNextToken(in, line);
	}
	//put back token
	Parser::PushBackToken(word);
	*/
	return true;

 };

 bool Relation(istream& in, int& line){
	if(!(SimpleExpr(in, line))){
		return false;
	}
	
	// FIX THIS
	word= Parser::GetNextToken(in, line);
	if(word == EQ || word == NEQ || word == LTHAN || word == LTE || word == GTHAN || word == GTE ){
		cout << "reached " << endl; //remove
		cout << word << word.GetLinenum() << endl; //remove

		if(!(SimpleExpr(in, line))){
			ParseError(line, "Missing right parenthesis after expression");  //change
			return false;
		}
	} else { 
		Parser::PushBackToken(word);
	} 
	return true;
 };

 //6. SimpleExpr ::= STerm { ( + | - | & ) STerm }
 bool SimpleExpr(istream& in, int& line){
	if(!(STerm(in, line))){
		return false;
	}
	
	word= Parser::GetNextToken(in, line);
	while (word == PLUS || word == MINUS || word == AND){
		if(!(STerm(in, line))){
			return false;
		}
		word= Parser::GetNextToken(in, line);
	}
	// changed this
	if(word.GetToken() == EQ){
		return false;
	}
	// changed htis 

	Parser::PushBackToken(word);
	
	return true;

 };

 //STerm ::= [ ( + | - ) ] Term
 bool STerm(istream& in, int& line){
	word= Parser::GetNextToken(in, line);
	if(word == MINUS){
		sign = -1;
		word= Parser::GetNextToken(in, line);
	}
	if(!(Term(in, line, sign))){
		return false;
	}
	return true;
 };

 // Term ::= Factor { ( * | / | MOD ) Factor }
 bool Term(istream& in, int& line, int sign){

	if(!(Factor(in, line, sign))){
		return false;
	}
	word= Parser::GetNextToken(in, line);
	while(word == MULT || word == DIV || word == MOD){
		word= Parser::GetNextToken(in, line);

		if(!(Factor(in, line, sign))){
			ParseError(line, "Missing operand after operator");
			ParseError(line, "Missing Expression in Assignment Statement"); //change
			return false;
		}
		word= Parser::GetNextToken(in, line);

	}
     if (word.GetToken() == IDENT){
        ParseError(line, "Illegal expression for an assignment statement");
       // ParseError(line, "Invalid assignment statement."); //change
        return false;
    }
	Parser::PushBackToken(word);


	return true;
 };

 //Factor ::= Primary [** Primary ] | NOT Primary
 bool Factor(istream& in, int& line, int sign){
	if(Primary(in, line, sign)){
		// case 18: eq failed primary
		
		/*
		word = Parser::GetNextToken(in, line);
		//word will be MULT
		if(word == MULT){
			word = Parser::GetNextToken(in, line);
			if(word == MULT){
				if(!(Primar))
			} 
			else{

				return false;
			}
		}


		
		// REMOVE FOR NOW BUT THEN REINSTALL
		word= Parser::GetNextToken(in, line);
		if(word == MULT){
			word= Parser::GetNextToken(in, line);
			if(word != MULT){
				return false;
			}
			word= Parser::GetNextToken(in, line);
			if(!(Primary(in, line, sign))){
				return false;
			}
		}
		Parser::PushBackToken(word); //fix
		*/

		return true;
	} else if (word == NOT){
		if(!(Primary(in, line, sign))){
			return false;
		}
		return true;
	} else {
		cout << "zubi word" << word << endl; //remove
		ParseError(line, "Incorrect operand");
		return false;}

 };
 //Primary ::= Name | ICONST | FCONST | SCONST | BCONST | CCONST | (Expr)
bool Primary(istream& in, int& line, int sign){
	//word= Parser::GetNextToken(in, line); //fix
	 // bool status; //unused
	if(word == ICONST || word == FCONST || word == SCONST || word == BCONST || word == CCONST ){
		return true;
	} else if(Name(in, line)){ 
		return true;}
	else if(word == LPAREN){
		// here // 
		if(!(Expr(in, line))){
			//case 14 should fail here 
			return false;
		}
		// word should be end of expr

		//change
		
		//change

		cout << "wird after eq" << word << endl;//remove

		word= Parser::GetNextToken(in, line);
		
		// word should be rparen 
		if(word != RPAREN){
			cout << "weird word " << word << endl;//remove
			return false;
		}
		return true;
	}
	else {
      if(word.GetToken() == SEMICOL){ParseError(line, "Invalid Expression");}
        return false;}
	
 };

 //Name ::= IDENT [ ( Range ) ]
 bool Name(istream& in, int& line){
	if(word != IDENT){
		return false;
	}

	if(defVar.find(word.GetLexeme()) == defVar.end()) {
		ParseError(line, "Using Undefined Variable");
		ParseError(line, "Invalid reference to a variable.");
		return false;
	}

	word= Parser::GetNextToken(in, line);

	if(word == LPAREN){
		if(!(Range(in, line))){
			return false;
		}
		word= Parser::GetNextToken(in, line);
		if(word != RPAREN){
			return false;
		}
		word= Parser::GetNextToken(in, line);
	}

	Parser::PushBackToken(word);

	return true;

 };

 //Range ::= SimpleExpr [. . SimpleExpr ]
bool Range(istream& in, int& line){
	if(!(SimpleExpr(in, line))){
		return false;
	}
	word= Parser::GetNextToken(in, line);
	if(word == DOT){
		word= Parser::GetNextToken(in, line);
		if(word != DOT){
			return false;
		}
		if(!(SimpleExpr(in, line))){
			return false;
		}
		word= Parser::GetNextToken(in, line);
	}
	//putback word
	Parser::PushBackToken(word);
	return true;
};
