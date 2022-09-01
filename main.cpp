/*
* 03.25.2022
*/

#include <iostream>
#include <fstream>
#include "interpreter.h"

std::vector<Operator> get_operators(void);
std::list<Token> get_tokens(const std::vector<Operator>& operators);
void print_operators(const std::vector<Operator>& operators);
void print_tokens(const std::list<Token>& tokens);

int main() {
	std::vector<Operator> operators = get_operators();
	std::list<Token> tokens = get_tokens(operators);
	Token result = evaluate(tokens);
	if (!result.empty()) { std::cout << result._name << "\n"; }
	return 0;
}
std::vector<Operator> get_operators(void) {
	std::vector<Operator> operators;
	std::ifstream infile; infile.open("operators.txt"); if (infile.fail()) { throw std::ios_base::failure("failed to open operation file\n"); }
	Operator op; while (!(infile >> op).fail()) { operators.push_back(op); op.clear(); }  infile.close();
	return operators;
}

void print_operators(const std::vector<Operator>& operators) {
	std::cout << "OPERATION\tARITY\tNOTATION\tSEQUENCE" << std::endl;
	for (int i = 0; i < 50; i++) { std::cout << "-"; } std::cout << std::endl;
	for (int i = 0; i < static_cast<int>(operators.size()); i++) { std::cout << i << ": " << operators[i] << std::endl; }
	for (int i = 0; i < 50; i++) { std::cout << "-"; } std::cout << std::endl;
}

void print_tokens(const std::list<Token>& tokens) {
	for (auto it = tokens.begin(); it != tokens.end(); it++) {
		std::cout << "name: [" << (it)->_name << "] : type: " << it->_type << std::endl;
		if ((it)->_type == OPERATOR) { std::cout << (it)->_op << "\tindex: " << (it)->_op._id << std::endl; }
	}

}

std::list<Token> get_tokens(const std::vector<Operator>& operators) {
	//  character stream -> token queue
	std::list<Token> tokens;
	std::string str;
	do {
		// read in a string from the user, standardise with terminal character if need be
		std::getline(std::cin, str);
		if (str.empty() || str[str.length() - 1] != ';') { str.push_back(';'); }
		tokens = scan(std::move(str), operators);
		if (tokens.empty()) {
			std::cerr << "Error: Invalid string entered. Please enter again.\n";
		}
	} while (tokens.empty());
	return tokens;
}