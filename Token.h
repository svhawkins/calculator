#pragma once
#include <string>
/*
* 04.30.2022
* 
* Token.h: header file for Token class
* 
*/

#include "Operator.h"

// Token error strings
const std::string BAD_NUM = "invalid number entered:\t";
const std::string BAD_OP = "invalid operation entered:\t";

// token states (types)
enum states { DEFAULT = 0, NUMERIC = 1, BOOLEAN = 2, OPERATOR = 3, DELIMITER = 4, TERMINAL = 5, JUNK = 6, INVALID = -1 };

/*
* class Token
*   holds data relating to Tokens
* 
* member functions:
*   Token(const std::string& str, int state, const Operator op) : constructor for OPERATOR tokens
*   Token(const std::string& str, int state) : constructor for non-OPERATOR tokens
*   Token(): default constructor
*   clear() : clears _name component, invalidates token by setting it to JUNK
* 
* member variables:
*   _name : name of the Token taken from the scanner
*   _type : type of the Token
*   _op: (OPERATOR tokens) copied Operator object component from the operators vector
*/
class Token {
public:
	Token(const std::string& str, int state, const Operator op) : _name(str), _type(state), _op(op) {}
	Token(const std::string& str, int state) : _name(str), _type(state) {}
	Token() : Token("", JUNK) {}
	void clear() { _name.clear(); _type = DEFAULT;}
	bool empty() { return _name.empty() && _type == JUNK; }
	std::string _name;
	int _type;
	Operator _op;
};