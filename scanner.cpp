/*
* 03.25.2022
* 
* scanner.cpp: implementation file for the scan function declared in interpreter.h
* 
*/

/*
performs the lexical analysis/tokenization process.
*/
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <utility>
#include "interpreter.h"

/*
throw_error_message
input: Token queue tokens, a string error message, and the string at fault for the error
output: cleared token queue
goal: outputs an appropriate error message based on error_msg as well as the token string at fault. clears tokens.
*/
std::list<Token> throw_error_message(std::list<Token> tokens, const std::string& error_msg, const std::string& tok) {
	std::cout << error_msg + "[" + tok + "]" << std::endl;  
	tokens.clear();  return tokens;
}
/*
* pinch_search:
* input:
* - const std::vector<Operator>& v: a SORTED vector of valid operators, elements stored as Operator objects
* - int start_point: starting index for the search interval in vector v
* - int end_point: ending index for the search interval in vector v
* - const std::string& key : key to find the in the operators vector, v
* - int index: starting digit index for the given key and elements in vector v
*
* output:
* - index of where the key was found in vector v (exact match)
* - if not found (either by invalid interval range or only a suffix/prefix found), -1 is returned.
*
* goal:
* - searches for key in vector within the interval of [start_point, end_point] 
* - by checking the individual letters/characters ('digits' or 'index') of the key and the current element at some indices i and j, i from start, j from end
* - once a match at a certain digit is found, a new interval is establised for the next digit/index and the function is called again recursively until
* - either start_point and end_point meet or until invalid
*/
int pinch_search(const std::vector<Operator>& v, int start_point, int end_point, const std::string& key, int index) {
	int not_found = -1, i, j; char left_compare_letter, right_compare_letter;
	char key_letter = (index < static_cast<int>(key.length())) ? key[index] : '\0';  // out-of-bounds error avoidance

	// base case(s)
	if (start_point > end_point) { return not_found; }  // base case : invalid interval -> false
	if (start_point == end_point) { return (key == v[start_point]._name) ? start_point : not_found; }  // found iff not suffix/prefix

	// non-recursive case, continue as long as i and j are in a valid interval range
	for (i = start_point, j = end_point; i < static_cast<int>(v.size()) && j >= 0 && i <= j; ) {

		// letters to compare to eachother
		left_compare_letter = (index < static_cast<int>(v[i]._name.length())) ? v[i]._name[index] : '\0';  // out-of-bounds error avoidance
		right_compare_letter = (index < static_cast<int>(v[j]._name.length())) ? v[j]._name[index] : '\0';  // out-of-bounds error avoidance

		if (left_compare_letter == key_letter && right_compare_letter == key_letter) { break; }  // new interval established

		// interval continuation
		if (left_compare_letter != key_letter) { i++; }  // continue searching 'rightward'
		if (right_compare_letter != key_letter) { j--; }  // continue searching 'leftward'
	}
	return pinch_search(v, i, j, key,  index + 1);  // continue searching in the new interval
}
/*
* int search:
* input: vector of operators v, key string
* output: index of where it was found in v, -1 if not found
* goal: wrapper function for pinch_search
*/
int search(const std::vector<Operator>& v, const std::string& key) { return pinch_search(v, 0, v.size() - 1, key, 0); }

/*
* is_numeric:
* input: const std::string& str, token string
* output: true if str is a valid numeric string, false otherwise 
* goal: determines validity via a regular expression
*/
bool is_numeric(const std::string& str) {
	if (str.empty()) { return false; }
	return std::regex_match(str, std::regex(_numeric));
}

/*
* is_num:
* input: char ch, a single character
* output: true if ch is a Numeric character, false otherwise
* goal: determines whether ch is a Numeric character via a switch statement with cases of valid Numeric characters.
*/
bool is_num(char ch) {
	bool numeric = false;
	switch (ch) {
	case '-':
	case '.':
	case 'i':
	case 'j':
		numeric = true; break;
	default:
		if (isdigit(ch)) { numeric = true; } break;
	}
	return numeric;

}

/*
* set_state:
* input: current character
* output: integer enumerated state to continue
* goal: based on the current character, determines the next state of the next token
*/
int set_state(char ch) {
	int state = DEFAULT;
	switch (ch) {
	case '.': case 'i': case 'j': state = INVALID; break;
	case '-': state = OPERATOR; break;
	default: state = INVALID; if (isdigit(ch)) { state = NUMERIC; } break;
	}
	return state;
}

/*
* tokenise (has 2 signatures)
* input: token string 'tok_str', current integer enumerated state 'state', Token queue 'tokens', and an Operator 'op'
* output: DEFAULT state
* goal: constructs a new Token and gives it attributes based on its given parameters. then pushed to queue, tok_str is cleared.
*/
int tokenise(std::list<Token>& tokens, std::string& tok_str, int state) {  // NOLINT
	Token tok(std::move(tok_str), state); tokens.push_back(tok); tok_str.clear();
	return DEFAULT;
}
int tokenise(std::list<Token>& tokens, std::string& tok_str, int state, const Operator& op) {  // NOLINT
	Token tok(std::move(tok_str), state, op); tokens.push_back(tok); tok_str.clear();
	return DEFAULT;
}

std::list<Token> scan(const std::string& input, const std::vector<Operator>& operators) {
	int current_state = DEFAULT, index = -1, STANDALONE = 1;
	std::string tok_str; std::list<Token> tokens;
	for (int i = 0; i < static_cast<int>(input.length()) && current_state != TERMINAL; i++) {
		char ch = input[i]; Operator op;
		if (std::regex_match(tok_str, std::regex(_boolean))) { current_state = tokenise(tokens, tok_str, BOOLEAN); }
		switch (ch) {
		// delimiting characters
		case '(':
		case ')':
		case ';':
		case ' ':
			if (!tok_str.empty()) {
				// token can be either: OPERATOR or NUMERIC if not empty.
				switch (current_state) {
				case OPERATOR:
					index = search(operators, tok_str);
					if (index <= INVALID) { return throw_error_message(tokens, BAD_OP, tok_str); }
					op = operators[index]; op._id = index;
					current_state = tokenise(tokens, tok_str, OPERATOR, op);
					break;
				case NUMERIC:
					if (!is_numeric(tok_str)) { return throw_error_message(tokens, BAD_NUM, tok_str); }
					if (static_cast<int>(tok_str.length()) == STANDALONE) {
						switch (set_state(tok_str[0])) {
						case OPERATOR:
							index = search(operators, tok_str);
							op = operators[index]; op._id = index;
							current_state = tokenise(tokens, tok_str, OPERATOR, op); break;
						case INVALID: return throw_error_message(tokens, BAD_NUM, tok_str);
						case NUMERIC: current_state = tokenise(tokens, tok_str, NUMERIC); break;
						}
					}
					else { current_state = tokenise(tokens, tok_str, NUMERIC); }
					break;
				}
			}
			if (ch == ' ') { continue; }
			current_state = DELIMITER; if (ch == ';') { current_state = TERMINAL; } // ternary wont work for some reason
			tok_str.clear();  tok_str.push_back(ch); current_state = tokenise(tokens, tok_str, current_state);
			break;
		default:
			// not any delimiting characters
			if (is_numeric(tok_str)) { current_state = NUMERIC; tok_str.push_back(ch); }
			else {
				bool is_num_tok = is_numeric(tok_str), is_num_ch = is_num(ch); // is_numeric is just a wrapper for std::regex(tok_str, std::regex(_numeric))
				if (!is_num_tok && (ch == 'i' || ch == 'j')) { is_num_ch = false; }
				if (is_num_tok && is_num_ch) {
					if (ch != '-') { return throw_error_message(tokens, BAD_NUM, tok_str + ch); }
					current_state = tokenise(tokens, tok_str, NUMERIC);
					tok_str.push_back(ch); current_state = set_state(ch);
				}
				else if (is_num_tok && !is_num_ch) {
					// tokenise current string as NUMERIC, ch starts new token string of OPERATOR
					current_state = tokenise(tokens, tok_str, NUMERIC);
					tok_str.push_back(ch); current_state = OPERATOR;
				}
				else if (!is_num_tok && is_num_ch) {
					// tokenise current string as OPERATOR, search first (if not empty), ch is new tok str based on character
					if (!tok_str.empty()) {
						index = search(operators, tok_str);
						if (index <= INVALID) { return throw_error_message(tokens, BAD_OP, tok_str + ch); }
						op = operators[index]; op._id = index;
						current_state = tokenise(tokens, tok_str, OPERATOR, op);
					}
					tok_str.push_back(ch); current_state = set_state(ch);
				}
				else {
					// is operator
					// if alphabetic->non-alphabetic OR non-alphabetic->alphabetic: tokenise, ch starts new token as operator
					// otherwise continue as operator
					char n = (!tok_str.empty()) ? tok_str[tok_str.length() - 1] : '\0';
					if ((isalpha(n) && !isalpha(ch)) || (!isalpha(n) && isalpha(ch))) {
						if (!tok_str.empty()) {
							index = search(operators, tok_str);
							if (index <= INVALID) { return throw_error_message(tokens, BAD_OP, tok_str); }
							op = operators[index]; op._id = index;
							current_state = tokenise(tokens, tok_str, OPERATOR, op);
						}
					}
					if (!isspace(ch)) { tok_str.push_back(ch); } current_state = OPERATOR;
				}
			}
			break;
		}
	}
	return (tokens);
}