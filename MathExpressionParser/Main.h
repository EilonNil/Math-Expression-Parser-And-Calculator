#pragma once

#include <iostream>
#include <string>
#include <vector>

enum op_type {
	plus, minus, multiply, divide, par_open, par_close
};

enum token_t {
	op_t, int_type, float_type
};


struct token {
	token_t type;

	int i;
	float f;
	op_type op;


};

std::vector<token> tokenize(const std::string& str);
bool checkValid(const std::vector<token>& experssion);
token evaluate(const std::vector<token>& expression);
bool isNumber(const char& c);
bool isValidOperator(const char& c);