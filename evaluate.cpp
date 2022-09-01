/*
* 04.22.2022
* 
* evaulate.cpp: implementation file for the evaluate method declared in interpreter.h
* 
*/

#include <list>
#include <sstream>
#include <utility>
#include <iostream>
#include "interpreter.h"
#include "Operation.h"

Token evaluate(std::list<Token>& tokens) {
	Token _ret; Operation op;
	std::list<Operation> opstack; opstack.push_front(op);
	while (!tokens.empty()) {
		Token t = tokens.front(); tokens.pop_front();
		//std::cout << "current token: " << t._name << std::endl;
		if (t._type == NUMERIC || t._type == BOOLEAN) { 
			opstack.front().set_operand(t); 
			continue;
		} else if (t._type == OPERATOR) {
			if (t._op._arity == 1) { Operation op_new; opstack.push_front(op_new); }
			opstack.front().set_operator(t);
			continue;
		} else if (t._name == "(") { 
			// push a new Operation frame to the stack
			Operation op_new; opstack.push_front(op_new);
		} else if (t._name == ")" or t._name == ";") {
			Token _result;
			// remove the Operation from the stack once operated upon
			_result = opstack.front().operate();  
			_ret = _result; opstack.pop_front();
			if (!opstack.empty()) { opstack.front().set_operand(_ret); }
		}
	}
	return _ret;
}