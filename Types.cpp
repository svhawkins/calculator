/*
* 08.07.2022
* 
* Types.cpp: implementation file for function declarations in Types.h
* 
*/

#include <iostream>
#include "Types.h"

int infer_type(const std::string& name) {
	int type = 0;
	if (std::regex_match(name, std::regex(_boolean))) { type = BOOL; }
	else if (std::regex_match(name, std::regex(_complex))) { type = COMPLEX; }
	else if (std::regex_match(name, std::regex(_imaginary))) { type = IMAGINARY; }
	else if (std::regex_match(name, std::regex(_decimal))) { type = DECIMAL; }
	else { type = INTEGER; }
	return type;
}

int determine_type(int operand) {
	int type = 0;
	switch (operand) {
	case BOOL: type = BOOL; break;
	case COMPLEX: type = COMPLEX; break;
	case IMAGINARY: type = IMAGINARY; break;
	case DECIMAL: type = DECIMAL; break;
	case INTEGER: type = INTEGER; break;
	}
	return type;
}
int determine_type(int left, int right) {
	int type = 0;
	if (is_boolean(left, right)) { type = BOOL; }
	else if (is_complex(left, right)) { type = COMPLEX; }
	else if (is_imaginary(left, right)) { type = IMAGINARY; }
	else if (is_decimal(left, right)) { type = DECIMAL; }
	else { type = INTEGER; }
	return type;
}
int determine_type(int a, int b, int c) {
	int type = 0;
	if (a == COMPLEX || b == COMPLEX || c == COMPLEX) { type = COMPLEX; }
	if (a == IMAGINARY && b == IMAGINARY && c == IMAGINARY) { type = IMAGINARY; }
	if (a == DECIMAL || b == DECIMAL || c == DECIMAL) { type = DECIMAL; }
	if ((a == INTEGER && b == INTEGER) && c == INTEGER) { type = INTEGER; }
	return type;
}

int get_resultant_type(const std::vector<Token>& operands) {
	int result_type = 0;
	switch (static_cast<int>(operands.size())) {
	case 0: break;
	case 1: result_type = determine_type(operands[0]._type); break;
	case 2: result_type = determine_type(operands[0]._type, operands[1]._type); break;
	case 3: result_type = determine_type(operands[0]._type, operands[1]._type, operands[2]._type); break;
	default: break;
	}
	return result_type;
}

// error functions
auto notation_str = [](int n) {return std::move((n == 1) ? std::string("PREFIX") : ((n == -1) ? std::string("POSTFIX") : std::string("INFIX"))); };
auto num_bool_str = [](char ch) { return std::move((ch == 'B') ? std::string("BOOLEAN") : std::string("NUMERIC")); };
auto operands_str = [](const std::vector<Token>& operands) {
	std::string str; 
	for (int i = 0; i < static_cast<int>(operands.size()); i++) { 
		if (i) str += " , "; str += "[" + operands[i]._name + "]";
	}
	return str;
};

Token throw_error_message(const std::string& errormsg) { std::cerr << errormsg; return Token(); }
std::string arity_error(int e, int r) { 
	return std::move(std::string("ARITY ERROR: Expected [" + std::to_string(e) + "] operand(s), recieved: [" + std::to_string(r) + "] operand(s).\n")); \
}
std::string syntax_error(int e_notation) { 
	std::string e = notation_str(e_notation); 
	return std::move(std::string("SYNTAX ERROR: misplaced operator. Expected [" + e + "] notation.\n")); 
}
std::string type_error(char e, char r) { 
	std::string e_str = num_bool_str(e), r_str = num_bool_str(r); 
	return std::move(std::string("TYPE ERROR: Expected type [" + e_str + "], recieved type [" + r_str + "]\n"));
}
std::string nonreal_error(const std::vector<Token>& operands) { 
	return "DOMAIN ERROR: " + std::move(std::string("Operand(s) " + operands_str(operands) + " must be real and not complex/imaginary.\n")); 
}
std::string noninteger_error(const std::vector<Token>& operands) { 
	return "DOMAIN ERROR: "  + std::string("Operand(s) " + operands_str(operands) + " must be whole.\n"); 
}
std::string undefined_error(const std::vector<Token>& operands, int state) {
	std::string str = "UNDEFINED ERROR: Operands(s) " + operands_str(operands) + " must be ";
	switch (state) {
	case NONPOSITIVE: str += "positive (greater than 0).\n"; break;
	case ZERO: str += "non-zero.\n"; break;
	}
	return std::move(str);
}


// lambdas involving strings

/*
* inum
* input: imaginary number string name
* output: a string holding the value held by the string as a real number
* goal: copies contents of name up to i/j character and returning resulting string
*/
auto inum = [](const std::string& name) { 
	std::string str; str.resize(name.length() - 1); 
	for (int i = 0; i < static_cast<int>(str.length()); i++) { str[i] = name[i]; } 
	return str;
};

/*
* store_as
* input: numeric string name
* output: a double-typed value
* goal: determines whether name should be stored as an integer or a double based on whether the double version is equivalent to its integer counterpart
*/
auto store_as = [](const std::string name) { return (std::stod(name) == floor(std::stod(name))) ? std::stoi(name) : std::stod(name); };


std::string to_string(int t) { return std::to_string(t) + "i"; }
std::string to_string(double t) { return std::to_string(t) + "i"; }
std::string to_string(const std::complex<double>& z) {
	std::string str;
	std::string real = std::to_string((z.real())), imag = std::to_string((z.imag()));
	if (z.real() == static_cast<int>(z.real())) { real = std::to_string(static_cast<int>(z.real())); }
	if (z.imag() == static_cast<int>(z.imag())) { imag = std::to_string(static_cast<int>(z.imag())); }

	if (z.real() == 0) { real.clear(); }
	if (z.imag() == 0) { imag.clear(); }
	if (real.empty() && imag.empty()) { real = "0"; }

	if (!real.empty()) { str += real; }
	if (!imag.empty() && !real.empty()) { str += " + "; }
	if (!imag.empty()) { str += imag + "i"; }
	return str;
}

std::complex<double> stoz(const std::string& s) {
	std::stringstream ss; ss << s;
	std::vector<std::string> strs; std::string str;
	while (!(ss >> str).fail()) { strs.push_back(str); }

	std::complex<double> z;
	switch (strs.size()) {
	case 1:
		if (std::regex_match(strs[0], std::regex(_imaginary))) { z.real(0); z.imag(store_as(inum(strs[0]))); }
		else { z.real(store_as(strs[0])); z.imag(0); } break;
	case 3:
		z.real((std::regex_match(strs[0], std::regex(_imaginary)) ? store_as(strs[2]) : store_as(strs[0])));
		z.imag((std::regex_match(strs[0], std::regex(_imaginary)) ? store_as(inum(strs[0])) : store_as(inum(strs[2]))));
		break;
	}
	return z;
}

double stoj(const std::string& s) { return std::stod(inum(s)); }
