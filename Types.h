/*
* 08.07.2022
* 
* Types.h: header file for functions, constants, and lambda expressions regarding Numeric types and their resulting errors
* 
*/

#pragma once
#include <string>
#include <regex>
#include <complex>
#include "Token.h"

// constants and lambdas
// types of types
const double e = 2.71828;
const double PI = 3.14159;
enum { INTEGER = 490, DECIMAL, IMAGINARY, COMPLEX, BOOL };
enum { NONPOSITIVE = 300, ZERO };

// regex expressions for types
const std::string _imaginary(".*[IiJj]$");
const std::string _decimal("(.*[.]{1}.*)");
const std::string _int("^[1-9][0-9]*");
const std::string _real("(" + _int + ")|(" + _decimal + ")");
const std::string _complex("^$");
const std::string _boolean("([Ff][Aa][Ll][Ss][Ee])|([Tt][Rr][Uu][Ee])");
const std::string _i_int(_int + _imaginary);
const std::string _numeric("^[-]?\\d*[.]?\\d*[ij]?$");

/*
* is_[numeric_type]
* input: integers l and r, enumerated constants of operand types
* output: bool
* goal: determines if resultant type is of type [numeric_type] (as indicated in function name) based on the types of given operands
*/
auto is_complex = [](int l, int r) {return (l == COMPLEX) || (r == COMPLEX) || ((l == IMAGINARY && r != IMAGINARY) || (l != IMAGINARY && r == IMAGINARY)); };
auto is_imaginary = [](int l, int r) { return (l == IMAGINARY && r == IMAGINARY); }; // false iff operation is *
auto is_decimal = [](int l, int r) { return l == DECIMAL || r == DECIMAL; };
auto is_integer = [](int l, int r) { return l == INTEGER && r == INTEGER; };
auto is_boolean = [](int l, int r) { return (l == BOOL && r == BOOL); };
auto is_bool = [](int l) { return l == BOOL; };
auto is_whole = [](const std::string& name) { return std::regex_match(name, std::regex(_int)) || std::regex_match(name, std::regex(_i_int)); };
auto is_real = [](int l) { return !(l == COMPLEX || l == IMAGINARY); };

// error functions
/*
throw_error_message:
input: error message string
output: empty/default/invalid token
goal: sends the given errormsg string to the error output stream, returning an invalid token in the process.
*/
Token throw_error_message(const std::string& errormsg);

/*
.*_error:
input: expected, recieved (of varying type)
output: an error message string
goal: concatenates message and operands to a single string to send to the stream for throw_error_message()
*/
std::string arity_error(int e, int r);
std::string syntax_error(int e_notation);
std::string type_error(char e, char r);
std::string nonreal_error(const std::vector<Token>& operands);
std::string noninteger_error(const std::vector<Token>& operands);
std::string undefined_error(const std::vector<Token>& operands, int state);


// typing functions
/*
infer_type:
input: numeric Token T
output: integer-enumerated value describing further specified numeric type: COMPLEX, IMAGINARY, DECIMAL, INTEGER (the previous NUMERIC type is then overwritten)
goal: determines the true numeric type of some Token T via regular expressions
*/
int infer_type(const std::string& name);

/*
determine_type:
input: 2 intgers, left and right, values from the enumeration from infer_type
output: resultant type within the enum: COMPLEX, IMAGINARY, DECIMAL, INTEGER
goal: determines resultant type based on the inferred types of the 2 operands
*/
int determine_type(int operand);
int determine_type(int left, int right);
int determine_type(int a, int b, int c);


/*
get_resultant_type
input: vector of operand Tokens
output: type of the resultant based on the now inferred types of the operands
goal: wrapper function for the overloads of determine_type
*/
int get_resultant_type(const std::vector<Token>& operands);


// string functions
/*
stoj (string to imaginary)
input: imaginary number string s
output: numerical form of s (stored as a double)
goal: converts the string form of an imaginary number to its (real) numeric form
*/
double stoj(const std::string& s);

/*
stoz (string to complex)
input: complex number string s
output: numerical form of s (stored as std::complex<double>)
goal: converts the string form of a complex number to its numeric real and imaginary components
*/
std::complex<double> stoz(const std::string& s);


/*
to_string
input: either an imaginary int/double t, or a complex double z
output: a string form of the operand in (a + bi) form (or just bi)
goal: converts numerical operand to a string
*/
std::string to_string(int t);
std::string to_string(double t);
std::string to_string(const std::complex<double>& z);