#include <iostream>
#include <cmath>
#include "val.h"

using std::string;
using namespace std;

    // numeric overloaded add this to op
    Value Value::operator+(const Value& op) const {
        Value v;
        
        if (GetType() == VREAL){
            if (op.GetType() == VREAL){
            v.SetType(VREAL);
            v.SetReal(GetReal() + op.GetReal());}
            else {return v;}
           
        }
        
        if (GetType() == VINT) {
            if (op.GetType() == VINT)
            { v.SetType(VINT);
            v.SetInt(GetInt() + op.GetInt());}
            else {return v;}
        
        }
        
        return v;
        
    };
    
    // numeric overloaded subtract op from this
    Value Value::operator-(const Value& op) const {
        Value v;
        
        if (GetType() == VREAL){
             if (op.GetType() == VREAL){
            v.SetType(VREAL);
            v.SetReal(GetReal() - op.GetReal());}
            else {return v;}
        }
        
        if (GetType() == VINT) {
            if (op.GetType() == VINT){
            v.SetType(VINT);
            v.SetInt(GetInt() - op.GetInt());}
               else {return v;}        
        }
        return v;
    };
    
    // numeric overloaded multiply this by op
    Value Value::operator*(const Value& op) const {
        Value v;
        
        if (GetType() == VREAL){
            if (op.GetType() == VREAL){
            v.SetType(VREAL);
            v.SetReal(GetReal() * op.GetReal());}
            else {return v;}
        }
        
        if (GetType() == VINT) {
            if (op.GetType() == VINT){
            v.SetType(VINT);
            v.SetInt(GetInt() * op.GetInt());}
            else {return v;}
        
        }
        return v;
       
    
    };
    
    // numeric overloaded divide this by oper
    Value Value::operator/(const Value& op) const {
           Value v;
        
        if(GetType() == VREAL) {
            if (op.GetType() == VREAL){
            v.SetType(VREAL);
            v.SetReal(GetReal() / op.GetReal());}
                else {return v;}
        }
        
        if(GetType() == VINT){
            if (op.GetType() == VINT){
            v.SetType(VINT);
            v.SetInt(GetInt() / op.GetInt());}
            else {return v;}
        
        }
        return v;
    };
    
    //numeric MOD: overloaded Remainder this by op
    Value Value::operator%(const Value & op) const { 
           Value v;
        if(GetType() != VINT){ 
                //error
            return v;
        }
            v.SetType(VINT);
            v.SetInt( GetInt() % op.GetInt() );
            return v;
        
    };
         
    //(Relational = (Equal): overloaded equality operator of this with op
    Value Value::operator==(const Value& op) const {
        bool value;
        Value v;
        v.SetType(VBOOL);
        
        if(GetType() == VINT && op.GetType() == VINT){
             if( GetInt() == op.GetInt()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
        else if(GetType() == VREAL && op.GetType() == VREAL){
             if( GetReal() == op.GetReal()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
         else if(GetType() == VSTRING && op.GetType() == VSTRING){
             if( GetString() == op.GetString()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
        else if(GetType() == VCHAR && op.GetType() == VCHAR){
             if( GetChar() == op.GetChar()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
         else if(GetType() == VBOOL && op.GetType() == VBOOL){
             if( GetBool() == op.GetBool()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        } else {
             value = false;
             v.SetBool(value);
         }
     
        return v;
        
    };
    
	//Relational /= (Not equal): overloaded inequality operator of this with op
    Value Value::operator!=(const Value& op) const {
         bool value;
        Value v;
        v.SetType(VBOOL);
        
        if(GetType() == VINT && op.GetType() == VINT){
             if( GetInt() != op.GetInt()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
        else if(GetType() == VREAL && op.GetType() == VREAL){
             if( GetReal() != op.GetReal()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
         else if(GetType() == VSTRING && op.GetType() == VSTRING){
             if( GetString() != op.GetString()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
        else if(GetType() == VCHAR && op.GetType() == VCHAR){
             if( GetChar() != op.GetChar()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
         else if(GetType() == VBOOL && op.GetType() == VBOOL){
             if( GetBool() != op.GetBool()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        } else {
             value = false;
             v.SetBool(value);
         }
     
        return v;
    
    };
    
	//overloaded greater than operator of this with op
	Value Value::operator>(const Value& op) const {
         bool value;
        Value v;
        v.SetType(VBOOL);
        
        if(GetType() == VINT && op.GetType() == VINT){
             if( GetInt() > op.GetInt()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
        else if(GetType() == VREAL && op.GetType() == VREAL){
             if( GetReal() > op.GetReal()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
         else if(GetType() == VSTRING && op.GetType() == VSTRING){
             if( GetString() > op.GetString()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
        else if(GetType() == VCHAR && op.GetType() == VCHAR){
             if( GetChar() > op.GetChar()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        } else {
             value = false;
             v.SetBool(value);
         }
     
        return v;
        
    };
	
	//overloaded less than operator of this with op
	Value Value::operator<(const Value& op) const {
         bool value;
        Value v;
         v.SetType(VBOOL);
        
        if(GetType() == VINT && op.GetType() == VINT){
             if( GetInt() < op.GetInt()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
        else if(GetType() == VREAL && op.GetType() == VREAL){
             if( GetReal() < op.GetReal()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
         else if(GetType() == VSTRING && op.GetType() == VSTRING){
             if( GetString() == op.GetString()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
        else if(GetType() == VCHAR && op.GetType() == VCHAR){
             if( GetChar() == op.GetChar()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }else {
             value = false;
             v.SetBool(value);
         }
     
        return v;
    
    };
	
	//Relational <= (less than or equal): overloaded less than or equal operator of this with op
	Value Value::operator<=(const Value& op) const {
         bool value;
        Value v;
         v.SetType(VBOOL);
        
        if(GetType() == VINT && op.GetType() == VINT){
             if( GetInt() <= op.GetInt()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
        else if(GetType() == VREAL && op.GetType() == VREAL){
             if( GetReal() <= op.GetReal()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
         else if(GetType() == VSTRING && op.GetType() == VSTRING){
             if( GetString() <= op.GetString()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
        else if(GetType() == VCHAR && op.GetType() == VCHAR){
             if( GetChar() <= op.GetChar()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        } else {
             value = false;
             v.SetBool(value);
         }
     
        return v;
    
    };
	
	//Relational >= (Greater than or equal): overloaded Greater than or equal operator of this with op
	Value Value::operator>=(const Value& op) const {
         bool value;
        Value v;
        v.SetType(VBOOL);

        
        if(GetType() == VINT && op.GetType() == VINT){
             if( GetInt() >= op.GetInt()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
        else if(GetType() == VREAL && op.GetType() == VREAL){
             if( GetReal() >= op.GetReal()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
         else if(GetType() == VSTRING && op.GetType() == VSTRING){
             if( GetString() >= op.GetString()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        }
        
        else if(GetType() == VCHAR && op.GetType() == VCHAR){
             if( GetChar() >= op.GetChar()){
             
             value = true;
             } else {value = false;}
            
             v.SetBool(value);
     
        } else {
             value = false;
             v.SetBool(value);
         }
     
        return v;
    
    };
	
	//Logical and: overloaded logical Anding operator of this with op
    Value Value::operator&&(const Value& op) const {
        bool value;
        Value v;
        v.SetType(VBOOL);

        if( IsBool() == true && op.IsBool() == true){
            if ( GetBool() && op.GetBool()){
                value = true;
            } else {value = false;}
            
            v.SetBool(value);
        } 
        
        return v;
    
    
    };
    
	//Logical or: overloaded logical Oring operator of this with op
	Value Value::operator||(const Value& op) const {
        
        bool value;
        Value v;
        v.SetType(VBOOL);
        
        if( IsBool() == true && op.IsBool() == true){
            if ( GetBool() || op.GetBool()){
                value = true;
            } else {value = false;}
            
            v.SetBool(value);
        } 
        
        return v;
    
    };
	
	//Logical not: overloaded logical Complement operator of this object
	Value Value::operator!(void) const {
        
        Value v;
        v.SetType(VBOOL);
        
        if( !( IsBool() )  ){
            //error
             return v;
        }
        if ( GetBool() == true){
            v.SetBool(false);
        } else {
            v.SetBool(true);
        }
        return v;
    
    };
	
	//Concatenation &: Concatenate this with op
    Value Value::Concat(const Value & op) const {
        Value v;
        string s = "";
        
        if ( IsChar() && op.IsChar() ){
     
            v.SetType(VSTRING);
            s = s+ GetChar() + op.GetChar();
            v.SetString(s) ;
            
        } else if (GetType() == VSTRING){
          // one is string or both are strings
              if ( op.IsString() ){
                  //both are string
                      v.SetType(VSTRING); 
                      v.SetString( GetString() + op.GetString());
                        
                    }
            
                else if (op.GetType() == VCHAR){
                    // other one is char
                   v.SetType(VSTRING);
                   v.SetString( GetString() + op.GetChar());
                     
                     }   
                
                }
        
        
          else if(op.GetType() == VSTRING){
              
                  if (IsChar()){
                   v.SetType(VSTRING);
                   v.SetString( GetChar() + op.GetString());
                  
                  }
               }
        
            return v;
    
    };
    
    //Exponentiation **: raise this to the power of op
    Value Value::Exp(const Value & op) const {
        
        Value v;
        if (! (IsReal()) || ! (op.IsReal()) ){
            // error
            return v;
        }
        v.SetType(VREAL);
        v.SetReal( pow(GetReal(), op.GetReal()));
        return v;
    };