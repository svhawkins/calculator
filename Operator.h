/*
* 04.30.2022
* 
* Operator.h: header file or Operator object
* 
*/

#pragma once
#include <string>

/*
* struct Operator
* structure to hold components of an operator
* 
* members:
*   clear() : sets member variables to default values
*   _name : string name of the operator
*   _arity : integer number of operands operator requires (0 -3)
*   _notation : (enumerated constant) position of the operator in the operation (PREFIX = -1, INFIX = 0, POSTIFX = 1)
*   _id : integer op-code of the operator, index of the operator in the operators vector
*   _sequence : string defining the order of operands and operator
*/

enum { PREFIX = -1, INFIX = 0, POSTFIX = 1 };
struct oppp {
	std::string _name;
	int _arity = 0;
	int _notation = -1;
	int _id = -1;
	std::string _sequence;
	void clear();
	struct oppp& operator=(const struct oppp& op);
}; typedef struct oppp Operator;
std::istream& operator>>(std::istream& in, Operator& op);
std::ostream& operator<<(std::ostream& out, const Operator& op);