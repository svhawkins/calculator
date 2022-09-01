/*
* 03.25.2022
* 
* Operation.h: header file for Operation class
* 
*/

#pragma once
#include <vector>
#include "interpreter.h"


// constants
const int MAX_ARITY = 3;

/*
* class Operation:
*	Is the class that holds data for a single operation. Such as the operator used to perform the operation and the operands used.
* 
* methods:
*  Operation() : default constructor
*  set_operand(Token t) : given a Token, places it into the _operands vector
*  set_operator(Token t) : given an operator Token, stores the corresponding Operator object as its own
*  operate() : operates on the operands and the operator, knowing the opcode, returns and stores the resultant if syntatically and semantically correct.
*  _set_default_arguments() (private) : sets default arguments for an operation if given operands is too little
*  _inspect error() (private) : if there is an error regarding the sequence and/or operands, it is inspected and a corresponding error is thrown via an invalid/JUNK Token
* 
* member variables:
*  _operands : vector of Token objects holding the operand Tokens
*  _operator is an Operator object, meaning it has fields of its own privy to it.
*  _sequence : the string of token types ('N' for numeric, 'B' for boolean, 'O' for operator) from each set_operand/operator. Used for syntax and semantic checking.
*  _resultant : Token holding the resultant Token returned from operate()
*
*/
class Operation {
private:
	static char _update_sequence(const Token& t);
public:
	Operation() : _resultant(), _sequence() { _operands.reserve(MAX_ARITY); };
	void set_operand(const Token& t);
	void set_operator(const Token& t);
	Token operate(void);
private:
	std::vector<Token> _operands;
	Operator _operator;
	Token _resultant;
	std::string _sequence;
	int set_default_arguments();
	Token inspect_error();
};