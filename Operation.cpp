/*
* 04.09.2022
* 
* Operation.cpp: implementation file for the Operation class
* 
*/

#include <sstream>
#include "Operation.h"

/*
* _update_sequence(const Token& t)
* input: a Token object of any type
* output: a character to push to individual sequence string
* goal: determines what kind of character to return based on Token type
*/
char Operation::_update_sequence(const Token& t) {
	char ch = '\0';
	switch (t._type) {
	case OPERATOR: ch = 'O'; break;
	case BOOLEAN: ch = 'B'; break;
	case NUMERIC: ch = 'N'; break;
	} 
	return ch;
}

/*
* set_operand(const Token& t)
* input: a Token object of non-OPERATOR type
* output: none
* goal:
*  - places the decoded token into the _operands vector
*  - updates the sequence based on the type of the Token
*/
void Operation::set_operand(const Token& t) {
	_operands.push_back(t);
	_sequence.push_back(_update_sequence(t));
}

/*
* set_operator
* input: a Token object with type OPERATOR
* output: none
* goal:
*  - determines which Operator object to store from the operators vector based on the opcode given in the token
*  - updates the sequence string
*/
void Operation::set_operator(const Token& t) {
	_operator._name = t._op._name;
	_operator._arity = t._op._arity;
	_operator._notation = t._op._notation;
	_operator._sequence = t._op._sequence;
	_operator._id = t._op._id;
	Operation::_sequence.push_back(_update_sequence(t));
}