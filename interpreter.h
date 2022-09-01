/*
* 03.25.2022
* 
* interpreter.h: header file for the functions scan() and evaluate()
* 
*/

#pragma once
#include <vector>
#include <string>
#include <list>
#include <utility>
#include <exception>
#include <iterator>
#include <memory>
#include "Token.h"
#include "Types.h"
#include "Operator.h"


/*
* scan
* input: input string 'input', operators vector 'v'
* output: an updated Token queue full of valid tokens parsed from the input string
* goal: parses the sequences of characters in input into valid Token objects
*/
std::list<Token> scan(const std::string& input, const std::vector<Operator>& operators);

/*
* evaluate
* input: Token queue t generated from scan()
* output: a single resultant Token, acting as the result of the expression from the initial input string
* goal: operates on the tokens to produce a single result using a stack of Operation objects. If any error occurs, an invalid Token is returned.
*/
Token evaluate(std::list<Token>& tokens);