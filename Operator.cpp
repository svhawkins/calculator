/*
* 04.30.2022
* 
* Operator.cpp: implementation file for Operator object
* 
*/

#include <iostream>
#include "Operator.h"

/*
* clear
* input: none
* output: none
* goal: clears an Operator instance and sets member variables to default values
*/
void Operator::clear(void) { 
	_name.clear(); 
	_sequence.clear(); 
	_arity = 0; 
	_notation = -1;
}

/*
* overloaded insertion
*/
std::istream& operator>>(std::istream& in, Operator& op) { 
	in >> op._name >> op._arity >> op._notation >> op._sequence; 
	return in; 
}

/*
* overloaded extraction
*/
std::ostream& operator<<(std::ostream& out, const Operator& op) { 
	out << "name: " << op._name << "\n\tarity: " << op._arity << "\n\tnotation: " << op._notation << "\n\tsequence:" << op._sequence << std::endl;
	return out; 
}

/*
* overloaded assignment
*/
Operator& Operator::operator=(const Operator& op) {
	this->_arity = op._arity;
	this->_name = op._name;
	this->_notation = op._notation;
	this->_sequence = op._sequence;
	this->_id = op._id;
	return *this;
}