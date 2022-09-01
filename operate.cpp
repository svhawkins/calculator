/*
* 04.09.2022
* 
* operate.cpp: implementation file for the operate() method in the Operation class
* 
*/

#pragma once
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cmath>
#include "Operation.h"

// opcodes
#define NOOP -1
#define LNOT 0
#define LNAND 1
#define NEQ 2
#define LXNOR 3
#define LNOR 4
#define REM 5
#define MOD 6
#define BAND 7
#define LAND 8
#define MULT 9
#define EXP 10
#define ADD 11
//#define INC 12
#define SUB 12
//#define DEC 14
#define DIV 13
#define FDIV 14
#define LESS 15
#define LSHIFT 16
#define LEQ 17
#define EQUAL 18
#define GREAT 19
#define RSHIFT 20
#define GEQ 21
#define BXOR 22
#define LXOR 23
#define CBRT 24
#define FACT 25
#define FIB 26
#define LG 27
#define LN 28
#define LOG 29
#define ROOT 30
#define SQRT 31
#define BOR 32
#define LOR 33
#define INV 34
#define BNAND 35
#define BXNOR 36
#define BNOR 37
// end opcodes


// some constants
double PHI = (1 + sqrt(5)) * 0.5;
double PSI = (1 - sqrt(5)) * 0.5;

/*
* decode_bool:
* input: boolean string
* output: bool
* goal: converts boolean string to boolean type
*/
auto decode_bool = [](const std::string& bool_str) { return (bool_str == "true") ? true : false; };

/* 
* encode_bool:
* input: bool b
* output: string
* goal: converts booleans to boolean strings
*/
auto encode_bool = [](bool b) { return (b) ? "true" : "false"; };

/* 
* bxor
* input: bools a and b
* output: bool
* goal: performs a boolean xor operation on its operands
*/

auto bxor = [](bool l, bool r) { return (l && !r) || (!l && r); };

/* 
* fact_int:
* input: integer n
* output: n!
* goal: computes the factorial of n
*/
auto fact_int = [](int n) { int x = 1; for (int i = 1; i <= n; i++) { x *= i; } return x; };

/*
* fdiv:
* input: 2 doubles a and b
* output: a double
* goal: performs floor division for its given operands
*/
auto fdiv = [](double a, double b) { return static_cast<int>(a / b); };

/*
* remain:
* input: 2 doubles a and b
* output: a double
* goal: computes the remainder for its given operands
*/
auto remain = [](double a, double b) { return a - (b * fdiv(a, b)); };

/*
* mod:
* input: 2 doubles a and b
* output: a double
* goal: computes the modulus for its given operands
*/
auto mod = [](double a, double b) { double r = remain(a, b); return ((b > 0 && r < 0) || (b < 0 && r > 0)) ? b + r : r; };


/*
* fact
* input: complex number z or integer i
* output: factorial of the given operand
* goal: wrapper function for fact_int based on operand type
*/
int fact(std::complex<double> z) { return fact_int(static_cast<int>(z.imag())); }
int fact(int i) { return fact_int(i); }



template<typename T>
bool is_zero(T t) { return t == 0; }
template <typename T>
bool is_zero(std::complex<T> z) { return (z.real() == 0 && z.imag() == 0); }

/*template<typename T>
bool is_positive(T t) { return t > 0; }
template <typename T>
bool is_positive(std::complex<T> z) { return (z.real() > 0 && z.imag() > 0); }
*/

bool is_positive(const Token& t) {
	bool pos = false;
	std::complex<double> z;
	switch (t._type) {
	case COMPLEX: z = stoz(t._name);  pos = z.real() > 0 && z.imag() > 0; break;
	case IMAGINARY: pos = stoj(t._name) > 0; break;
	case DECIMAL: case INTEGER: pos = stod(t._name) > 0; break;
	}
	return pos;
}


/*
* Operation::set_default_arguments:
* input: nothing (private member function)
* output: SUCCESS (1) or FAILURE (0)
* goal: tries to attempt to set default arguments for some operations, returns FAILURE if:
* 	- operator does not have the option to set default arguments
* 	- sequence is invalid
* 	FAILURE suggests further error in the Operation object which is to be inspected.
*/
const int SUCCESS = 1, FAILURE = 0;
int Operation::set_default_arguments() {
	int status = SUCCESS;
	if ((_operator._id == LSHIFT || _operator._id == RSHIFT) && _sequence == "NON") {
		Token t(std::to_string(2), INTEGER); _operands.push_back(t); std::swap(_operands[_operands.size() - 1], _operands[_operands.size() - 2]); _sequence.push_back('N');
	}
	else if (_operator._id == LOG &&_sequence == "ON") {  Token t(std::to_string(10), INTEGER); _operands.push_back(t);  _sequence.push_back('N'); }
	else if (_operator._id == LOG && _sequence == "ONN") { std::swap(_operands[_operands.size() - 1], _operands[_operands.size() - 2]); } // so it wont operate backwards
	else if (_operator._id == LG && _sequence == "ON") { Token t(std::to_string(2), INTEGER); _operands.push_back(t);  _sequence.push_back('N'); }
	else if (_operator._id == LN && _sequence == "ON") { Token t(std::to_string(e), INTEGER); _operands.push_back(t);  _sequence.push_back('N'); }
	else if (_operator._id == ROOT && _sequence == "ON") { Token t(std::to_string(2), INTEGER); _operands.push_back(t); _sequence.push_back('N'); }
	else if (_operator._id == ROOT && _sequence == "ONN") { std::swap(_operands[_operands.size() - 1], _operands[_operands.size() - 2]); } // so it wont operate backwards
	else if (_operator._id == SQRT && _sequence == "ON") { Token t(std::to_string(2), INTEGER); _operands.push_back(t); _sequence.push_back('N'); }
	else if (_operator._id == CBRT && _sequence == "ON") { Token t(std::to_string(3), INTEGER); _operands.push_back(t); _sequence.push_back('N'); }
	else { status = FAILURE; }
	return status;
}


/*
Operation::inspect_error
input: nothing (private member function)
output: empty Token object
goal: determines the source of the error of the sequence/operator sequence inequality and gives corresponding error message
*/
Token Operation::inspect_error() {
	if (_operands.size() != _operator._arity) { return throw_error_message(arity_error(_operator._arity, _operands.size())); }
	char e = '\0', r = '\0'; Token tok;
	for (int i = 0; i < static_cast<int>(_operator._sequence.length()); i++) {
		r = (i < static_cast<int>(_sequence.length())) ? _sequence[i] : r; 
		e = _operator._sequence[i];
		if (r == e) { continue; }
		switch (e) {
		case 'O': if (r != 'O') { tok = Token(throw_error_message(syntax_error(_operator._notation))); } break;
		case 'B': if (r != 'B') { tok = Token(throw_error_message(type_error(e, r))); } break;
		case 'N': if (r != 'N') { tok = Token(throw_error_message(type_error(e, r))); } break;
		}
	}
	return tok;
}


/*
* Operation::operate
* input: none (member function)
* output: Token result as the resultant of the instance of the Operation obejct
* goal: operates on the operands given by _operands via _operator, identifiable by the op-code of the _operator's _id member variable.
*/
Token Operation::operate() {
	Token result{ "", JUNK };
	if (_operator._id != NOOP) { if (!set_default_arguments() && _sequence != _operator._sequence) { return inspect_error(); } }
	for (int i = 0; i < static_cast<int>(_operands.size()); i++) { _operands[i]._type = infer_type(_operands[i]._name); }
	result._type = get_resultant_type(_operands);

	//std::cout << " ID: [" << _operator._id << "]\tSEQUENCE: [" << _sequence << "]\nOPERANDS (n=" << _operands.size() << "):\t";
	//std::for_each(_operands.begin(), _operands.end(), [](const Token& t) { std::cout << t._name << " (type: " << t._type << ")\t"; });
	//std::cout << "resultant type: (" << result._type << ")\n";

	switch (_operator._id) {
	case NOOP: result = (!_operands.empty()) ? _operands[0] : result; break;
	case ADD: 
		switch (result._type) {
		case COMPLEX: case IMAGINARY: result._name = to_string(stoz(_operands[0]._name) + stoz(_operands[1]._name)); break;
		case DECIMAL: result._name = std::to_string(std::stod(_operands[0]._name) + std::stod(_operands[1]._name)); break;
		case INTEGER: result._name = std::to_string(std::stoi(_operands[0]._name) + std::stoi(_operands[1]._name)); break;
		} 
		break;
	case SUB:
		switch (result._type) {
		case COMPLEX: case IMAGINARY: result._name = to_string(stoz(_operands[0]._name) - stoz(_operands[1]._name)); break;
		case DECIMAL: result._name = std::to_string(std::stod(_operands[0]._name) - std::stod(_operands[1]._name)); break;
		case INTEGER: result._name = std::to_string(std::stoi(_operands[0]._name) - std::stoi(_operands[1]._name)); break;
		} 
		break;
	case MULT:
		switch (result._type) {
		case COMPLEX: case IMAGINARY: result._name = to_string(stoz(_operands[0]._name) * stoz(_operands[1]._name)); break;
		case DECIMAL: result._name = std::to_string(std::stod(_operands[0]._name) * std::stod(_operands[1]._name)); break;
		case INTEGER: result._name = std::to_string(std::stoi(_operands[0]._name) * std::stoi(_operands[1]._name)); break;
		} 
		break;
	case DIV:
		switch (result._type) {
		case COMPLEX: case IMAGINARY:
		if (is_zero(stoj(_operands[1]._name)) || is_zero(stoz(_operands[1]._name))) { return throw_error_message(undefined_error(_operands, ZERO)); }
		result._name = to_string(stoz(_operands[0]._name) / stoz(_operands[1]._name)); break;
		case DECIMAL: case INTEGER:
		if (is_zero(std::stod(_operands[1]._name))) { return throw_error_message(undefined_error(_operands, ZERO)); }
		result._name = std::to_string(std::stod(_operands[0]._name) / std::stod(_operands[1]._name)); break;
		} 
		break;
	case LOG:
	case LG:
	case LN:
		switch (result._type) {
		case COMPLEX: case IMAGINARY:
		if (!is_positive(_operands[0]) || !is_positive(_operands[1])) { return throw_error_message(undefined_error(_operands, NONPOSITIVE)); }
		result._name = to_string((log(stoz(_operands[0]._name))) / (log(stoz(_operands[1]._name))));
		break;
		case DECIMAL: case INTEGER:
		if (!is_positive(_operands[0]) || !is_positive(_operands[1])) { throw_error_message(undefined_error(_operands, NONPOSITIVE)); }
		result._name = std::to_string(log(stod(_operands[0]._name)) / log(stod(_operands[1]._name))); break;
		}
		break;
	case ROOT:
	case SQRT:
	case CBRT:
		switch (result._type) {
		case COMPLEX: case IMAGINARY:
		result._name = to_string(pow(stoz(_operands[0]._name), double(1) / stoz(_operands[1]._name))); break;
		case DECIMAL: case INTEGER:
		result._name = std::to_string(pow(std::stod(_operands[0]._name), double(1) / std::stod(_operands[1]._name))); break;
		} 
		break;
	case EXP:
		switch (result._type) {
		case COMPLEX: case IMAGINARY: result._name = to_string(pow(stoz(_operands[0]._name), stoz(_operands[1]._name))); break;
		case DECIMAL: case INTEGER: result._name = (std::to_string(pow(std::stod(_operands[0]._name), std::stod(_operands[1]._name)))); break;
		}
		break;
	case FIB:
		switch (result._type) {
		case COMPLEX: case IMAGINARY: result._name = to_string((pow(PHI, stoz(_operands[0]._name)) - pow(PSI, stoz(_operands[0]._name))) / (PHI - PSI)); break;
		case DECIMAL: result._name = std::to_string((pow(PHI, std::stod(_operands[0]._name)) - pow(PSI, std::stod(_operands[0]._name))) / (PHI - PSI)); break;
		case INTEGER: result._name = std::to_string(static_cast<int>((pow(PHI, std::stod(_operands[0]._name)) - pow(PSI, std::stod(_operands[0]._name))) / (PHI - PSI))); break;
		}
		break;
	case LSHIFT:
		switch (result._type) {
		case COMPLEX: case IMAGINARY: result._name = to_string(stoz(_operands[0]._name) * pow(stoz(_operands[1]._name), stoz(_operands[2]._name))); break;
		case DECIMAL: case INTEGER: result._name = std::to_string(std::stod(_operands[0]._name) * pow(std::stod(_operands[1]._name), std::stod(_operands[2]._name))); break;
		}
		break;
	case RSHIFT:
		switch (result._type) {
		case COMPLEX: case IMAGINARY: result._name = to_string(stoz(_operands[0]._name) / (pow(stoz(_operands[1]._name), stoz(_operands[2]._name)))); break;
		case DECIMAL: case INTEGER: result._name = std::to_string(std::stod(_operands[0]._name) / (pow(std::stod(_operands[1]._name), std::stod(_operands[2]._name)))); break;
		}
		break;

	// real numbers only
	case FDIV:
		switch (result._type) {
		case DECIMAL: case INTEGER: 
		if (is_zero(std::stod(_operands[1]._name))) { return throw_error_message(undefined_error(_operands, ZERO)); }
		result._name = std::to_string(fdiv(std::stod(_operands[0]._name), std::stod(_operands[1]._name))); break;
		default: return throw_error_message(nonreal_error(_operands));
		}
		break;
	case REM:
		switch (result._type) {
		case DECIMAL: case INTEGER: 
		if (is_zero(std::stod(_operands[1]._name))) { return throw_error_message(undefined_error(_operands, ZERO)); }
		result._name = std::to_string(remain(std::stod(_operands[0]._name), std::stod(_operands[1]._name))); break;
		default: return throw_error_message(nonreal_error(_operands));
		}
		break;
	case MOD:
		switch (result._type) {
		case DECIMAL: case INTEGER:
		if (is_zero(std::stod(_operands[1]._name))) { return throw_error_message(undefined_error(_operands, ZERO)); }
		result._name = std::to_string(mod(std::stod(_operands[0]._name), std::stod(_operands[1]._name))); break;
		default: return throw_error_message(nonreal_error(_operands));
		}
		break;

	// whole numbers only (real xor imaginary)
	case FACT:
		if (!is_whole(_operands[0]._name)) { return throw_error_message(noninteger_error(_operands)); }
		switch (result._type) {
		case IMAGINARY: result._name = to_string((fact(stoz(_operands[0]._name)))); break;
		case INTEGER: result._name = std::to_string(fact(std::stoi(_operands[0]._name))); break;
		}
		break;
	case INV:
		if (!is_whole(_operands[0]._name)) { return throw_error_message(noninteger_error(_operands)); }
		switch (result._type) {
		case IMAGINARY: result._name = to_string(~static_cast<int>((stoz(_operands[0]._name).imag()))); break;
		case INTEGER: result._name = std::to_string(~(std::stoi(_operands[0]._name))); break;
		}
		break;
	case BAND:
		if (!(is_whole(_operands[0]._name) && is_whole(_operands[1]._name))) { return throw_error_message(noninteger_error(_operands)); }
		switch (result._type) {
		case IMAGINARY: result._name = to_string(static_cast<int>(stoz(_operands[0]._name).imag()) & static_cast<int>((stoz(_operands[1]._name).imag()))); break;
		case INTEGER: result._name = std::to_string((std::stoi(_operands[0]._name)) & std::stoi(_operands[1]._name)); break;
		}
		break;
	case BOR:
		if (!(is_whole(_operands[0]._name) && is_whole(_operands[1]._name))) { return throw_error_message(noninteger_error(_operands)); }
		switch (result._type) {
		case IMAGINARY: result._name = to_string(static_cast<int>(stoz(_operands[0]._name).imag()) | static_cast<int>((stoz(_operands[1]._name).imag()))); break;
		case INTEGER: result._name = std::to_string((std::stoi(_operands[0]._name)) | std::stoi(_operands[1]._name)); break;
		}
		break;
	case BXOR:
		if (!(is_whole(_operands[0]._name) && is_whole(_operands[1]._name))) { return throw_error_message(noninteger_error(_operands)); }
		switch (result._type) {
		case IMAGINARY: result._name = to_string(static_cast<int>(stoz(_operands[0]._name).imag()) ^ static_cast<int>((stoz(_operands[1]._name).imag()))); break;
		case INTEGER: result._name = std::to_string((std::stoi(_operands[0]._name)) ^ std::stoi(_operands[1]._name)); break;
		}
		break;
	case BNAND:
		if (!(is_whole(_operands[0]._name) && is_whole(_operands[1]._name))) { return throw_error_message(noninteger_error(_operands)); }
		switch (result._type) {
		case IMAGINARY: result._name = to_string(~(static_cast<int>(stoz(_operands[0]._name).imag()) & static_cast<int>((stoz(_operands[1]._name).imag())))); break;
		case INTEGER: result._name = std::to_string(~((std::stoi(_operands[0]._name)) & std::stoi(_operands[1]._name))); break;
		}
		break;
	case BNOR:
		if (!(is_whole(_operands[0]._name) && is_whole(_operands[1]._name))) { return throw_error_message(noninteger_error(_operands)); }
		switch (result._type) {
		case IMAGINARY: result._name = to_string(~(static_cast<int>(stoz(_operands[0]._name).imag()) | static_cast<int>((stoz(_operands[1]._name).imag())))); break;
		case INTEGER: result._name = std::to_string(~((std::stoi(_operands[0]._name)) | std::stoi(_operands[1]._name))); break;
		}
		break;
	case BXNOR:
		if (!(is_whole(_operands[0]._name) && is_whole(_operands[1]._name))) { return throw_error_message(noninteger_error(_operands)); }
		switch (result._type) {
		case IMAGINARY: result._name = to_string(~(static_cast<int>(stoz(_operands[0]._name).imag()) ^ static_cast<int>((stoz(_operands[1]._name).imag())))); break;
		case INTEGER: result._name = std::to_string(~((std::stoi(_operands[0]._name)) ^ std::stoi(_operands[1]._name))); break;
		}
		break;

	// relational operators, non-complex only, operands must be both real or both imaginary
	case EQUAL:
		switch (result._type) {
		case COMPLEX: case IMAGINARY: result._name = encode_bool(stoz(_operands[0]._name) == stoz(_operands[1]._name)); break;
		default: result._name = encode_bool(std::stod(_operands[0]._name) == std::stod(_operands[1]._name)); break;
		}
		result._type = BOOL;
		break;
	case NEQ:
		switch (result._type) {
		case COMPLEX: case IMAGINARY: result._name = encode_bool(stoz(_operands[0]._name) != stoz(_operands[1]._name)); break;
		default: result._name = encode_bool(std::stod(_operands[0]._name) != std::stod(_operands[1]._name)); break;
		}
		result._type = BOOL;
		break;
	case GREAT:
		switch (result._type) {
		case IMAGINARY: result._name = encode_bool(stoj(_operands[0]._name) > stoj(_operands[1]._name)); break;
		case DECIMAL: case INTEGER: result._name = encode_bool(std::stod(_operands[0]._name) > std::stod(_operands[1]._name)); break;
		case COMPLEX: return throw_error_message(nonreal_error(_operands)); break;
		}
		result._type = BOOL;
		break;
	case GEQ:
		switch (result._type) {
		case IMAGINARY: result._name = encode_bool(stoj(_operands[0]._name) >= stoj(_operands[1]._name)); break;
		case DECIMAL: case INTEGER: result._name = encode_bool(std::stod(_operands[0]._name) >= std::stod(_operands[1]._name)); break;
		case COMPLEX: return throw_error_message(nonreal_error(_operands)); break;
		}
		result._type = BOOL;
		break;
	case LESS:
		switch (result._type) {
		case IMAGINARY: result._name = encode_bool(stoj(_operands[0]._name) < stoj(_operands[1]._name)); break;
		case DECIMAL: case INTEGER: result._name = encode_bool(std::stod(_operands[0]._name) < std::stod(_operands[1]._name)); break;
		case COMPLEX: return throw_error_message(nonreal_error(_operands)); break;
		} 
		result._type = BOOL;
		break;
	case LEQ:
		switch (result._type) {
		case IMAGINARY: result._name = encode_bool(stoj(_operands[0]._name) <= stoj(_operands[1]._name)); break;
		case DECIMAL: case INTEGER: result._name = encode_bool(std::stod(_operands[0]._name) <= std::stod(_operands[1]._name)); break;
		case COMPLEX: return throw_error_message(nonreal_error(_operands)); break;
		}
		result._type = BOOL;
		break;

	// logical operators, boolean operands only
	case LNOT: result._name = encode_bool(!decode_bool(_operands[0]._name)); break;
	case LAND: result._name = encode_bool(decode_bool(_operands[0]._name) && decode_bool(_operands[1]._name)); break;
	case LOR: result._name = encode_bool(decode_bool(_operands[0]._name) || decode_bool(_operands[1]._name)); break;
	case LXOR: result._name = encode_bool(bxor(decode_bool(_operands[0]._name), decode_bool(_operands[1]._name))); break;
	case LNAND: result._name = encode_bool(!(decode_bool(_operands[0]._name) && decode_bool(_operands[1]._name))); break;
	case LNOR: result._name = encode_bool(!(decode_bool(_operands[0]._name) || decode_bool(_operands[1]._name))); break;
	case LXNOR: result._name = encode_bool(!bxor(decode_bool(_operands[0]._name), decode_bool(_operands[1]._name))); break;
	}


	// so evaluate() has an easier time with the tokens. its a bit redundant, but its a bandaid fix and its staying. also if i try to fix, evaluate breaks apart.
	if (result._type == BOOL) { result._type = BOOLEAN; } else { result._type = NUMERIC; }
	return result;
}