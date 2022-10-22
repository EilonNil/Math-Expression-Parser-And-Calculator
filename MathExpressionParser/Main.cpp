#include "Main.h"

int main() {
	std::string str;
	std::getline(std::cin, str);

	std::vector<token> expression = tokenize(str);

	if (checkValid(expression)) {
		token answer = evaluate(expression);
		switch (answer.type) {
		case (1): {
			std::cout << "= " << answer.i << std::endl;
		}
				break;
		case (2): {
			std::cout << "= " << answer.f << std::endl;
		}
				break;
		}
	}
	else {
		std::cout << "ERROR: INVALID EXPRESSION" << std::endl;
	}


	return 0;
}



token evaluate(const std::vector<token>& exp) {
	for (int i = 0; i < exp.size(); i++) { //finding the closing par so we can go back and find the last opening one, the deepest one
		if (exp[i].type == op_t && exp[i].op == par_close) {
			for (int j = i - 1; j >= 0; j--) {
				if (exp[j].type == op_t && exp[j].op == par_open) {
					std::vector<token> v;
					for (int k = j + 1; k < i; k++) {
						v.push_back(exp[k]); //puts everything between the pars in a new vector
					}
					token instuff = evaluate(v); //evaluates the vector
					v.clear(); //cleans the list
					for (int k = 0; k < j; k++) {
						v.push_back(exp[k]);
					}
					v.push_back(instuff);
					for (int k = i + 1; k < exp.size(); k++) {
						v.push_back(exp[k]);
					} //puts inside v the new expression without the most inner pars
					return evaluate(v); //recalls it
				}

			}
		}
	}
	// not that all the parenthasies are gone i will do the calculations
	// this happens when the function calculates the most inner parenthasies or when it calculates the function after all parenthasies have been calculated
	token t;
	for (int i = 0; i < exp.size(); i++) {
		if (exp[i].type == op_t && (exp[i].op == multiply || exp[i].op == divide)) { //divide and multiply happen first
			if (exp[i].op == multiply) {
				if (exp[i - 1].type == float_type || exp[i + 1].type == float_type) { //multiplying at least one float
					float num1 = exp[i - 1].i + exp[i - 1].f; //one of them will be 0
					float num2 = exp[i + 1].i + exp[i + 1].f; //one of them will be 0
					t = { float_type, 0, num1 * num2, plus };
				}
				else { //multiplying 2 ints
					int num1 = exp[i - 1].i;
					int num2 = exp[i + 1].i;
					t = { int_type, num1 * num2, 0, plus };
				}
			}
			else { //no check for floats or int because even divining 2 ints can give a float
				float num1 = exp[i - 1].i + exp[i - 1].f; //one of them will be 0
				float num2 = exp[i + 1].i + exp[i + 1].f; //one of them will be 0
				t = { float_type, 0, num1 / num2, plus };
			}
			std::vector<token> v;
			for (int j = 0; j < i - 1; j++) {
				v.push_back(exp[j]);
			}
			v.push_back(t);
			for (int j = i + 2; j < exp.size(); j++) {
				v.push_back(exp[j]);
			}
			return evaluate(v);
		}
	}
	for (int i = 0; i < exp.size(); i++) {
		if (exp[i].type == op_t && (exp[i].op == plus || exp[i].op == minus)) {
			if (exp[i].op == plus) {
				if (exp[i - 1].type == float_type || exp[i + 1].type == float_type) { //addition of at least one float
					float num1 = exp[i - 1].i + exp[i - 1].f; //one of them will be 0
					float num2 = exp[i + 1].i + exp[i + 1].f; //one of them will be 0
					t = { float_type, 0, num1 + num2, plus };
				}
				else { //both ints
					int num1 = exp[i - 1].i;
					int num2 = exp[i + 1].i;
					t = { int_type, num1 + num2, 0, plus };
				}
			}
			else {
				if (exp[i - 1].type == float_type || exp[i + 1].type == float_type) { //subtraction of at least one float
					float num1 = exp[i - 1].i + exp[i - 1].f; //one of them will be 0
					float num2 = exp[i + 1].i + exp[i + 1].f; //one of them will be 0
					t = { float_type, 0, num1 - num2, plus };
				}
				else { //both ints
					int num1 = exp[i - 1].i;
					int num2 = exp[i + 1].i;
					t = { int_type, num1 - num2, 0, plus };
				}
			}
			std::vector<token> v;
			for (int j = 0; j < i - 1; j++) {
				v.push_back(exp[j]);
			}
			v.push_back(t);
			for (int j = i + 2; j < exp.size(); j++) {
				v.push_back(exp[j]);
			}
			return evaluate(v);
		}
	}

	if (exp.size() == 0) {
		return { int_type,0,0,plus };
	}
	return exp[0];

}

bool isNumber(const char& c) {
	return c - '0' >= 0 && c - '0' <= 9;
}

bool isValidOperator(const char& c) {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

std::vector<token> tokenize(const std::string& str) {
	std::vector<token> t;

	for (int i = 0; i < str.size(); i++) {
		if (str[i] != ' ') {
			if (isNumber(str[i])) { //if number. ascii value of digit i is 20+i. if we remove the ascii value of 0, 20, we get the number itself
				if (str[i + 1] != '.' && !isNumber(str[i + 1])) { //one digit number that isnt a float
					t.push_back({ int_type, str[i] - '0', 0, plus });
				}
				else { //dealing with the entire number
					for (int j = i; j < str.size(); j++) {
						int firstDigit = i;
						if (str[j] == '.') { //dealing with a float
							float num = 0; //value before the 0 added to the number
							for (int k = firstDigit; k < j; k++) {
								num += (str[k] - '0') * pow(10, j - k - 1);
							}
							int k = j + 1;
							while (isNumber(str[k])) { //while it's digits
								num += (str[k] - '0') * pow(10, -(k - j));
								k++;
							}
							t.push_back({ float_type, 0, num, plus });
							i = k - 1; // moving i to where k managed to get in the string
							j = str.size(); // exiting the j for loop to go back to the main for loop
						}
						else if (str[j] == ' ' || isValidOperator(str[j])) { //needs fixing
							int num = 0;
							for (int k = firstDigit; k < j; k++) {
								num += (str[k] - '0') * pow(10, j - k - 1);
							}
							t.push_back({ int_type, num, 0, plus });
							i = j - 1; //jumping to there because i already dealt with what happens until there
							j = str.size(); // exiting the j for loop to go back to the main for loop
						}
						else if (j == str.size() - 1 && isNumber(str[j])) { //end of string and its a number
							int num = 0;

							for (int k = firstDigit; k <= j; k++) {
								num += (str[k] - '0') * pow(10, j - k);

							}
							t.push_back({ int_type, num, 0, plus });
							i = j; //jumping to there because i already dealt with what happens until there
							j = str.size(); // exiting the j for loop to go back to the main for loop
						}
					}
				}

			}
			else {
				switch (str[i]) { //dealing with all the operators
				case '(':
				{
					if (i != 0) {
						if (t[t.size() - 1].type != op_t) {
							t.push_back({ op_t, 0, 0, multiply });
						}
					}
					t.push_back({ op_t, 0, 0, par_open });
				}
				break;
				case ')':
				{
					t.push_back({ op_t, 0, 0, par_close });
				}
				break;
				case '+':
				{
					t.push_back({ op_t, 0, 0, plus });
				}
				break;
				case '-':
				{
					t.push_back({ op_t, 0, 0, minus });
				}
				break;
				case '*':
				{
					t.push_back({ op_t, 0, 0, multiply });
				}
				break;
				case '/':
				{
					t.push_back({ op_t, 0, 0, divide });
				}
				break;
				}
			}
		}
	}
	return t;
}




bool checkValid(const std::vector<token>& exp) {
	int par_open_count = 0;
	int par_close_count = 0;
	//checks for:
	//number of par_opens is equal to par_closes
	//no 2 non-parenthasies operators one next to the other (operator and then parenthasies is allowed)
	// no empty parehnthasies
	// no number after close parenthasies
	// no close parenthrasies before open parenthasies
	for (int i = 0; i < exp.size(); i++) {
		if (exp[i].type == op_t && exp[i].op == par_open) {
			par_open_count++;
		}
		else if (exp[i].type == op_t && exp[i].op == par_close) {
			par_close_count++;
		}
		if (i < exp.size() - 1) { //not end of vector
			if (exp[i].type == op_t && exp[i].op == par_open && exp[i + 1].type == op_t && exp[i + 1].op == par_close) {
				std::cout << "error: empty parenthasies" << std::endl;
				return false; //empty parenthasies
			}
		}
		if (par_close_count > par_open_count) {
			std::cout << "error: close parenthasies before open parenthasies" << std::endl;
			return false; // close parenthasies before open parenthasies or too many close parenthasies
		}
		if (exp[i].type == op_t && exp[i].op == par_close) { //number right after close parenthasies
			if (i < exp.size() - 1) { //not the end of the vector
				if (exp[i + 1].type != op_t) {
					std::cout << "error: number after close parenthasies" << std::endl;
					return false;
				}
			}
		}
		if (exp[i].type == op_t && (exp[i].op != par_close && exp[i].op != par_open)) {
			if (i < exp.size() - 1) { //not the end of the vector
				if (exp[i + 1].type == op_t && (exp[i + 1].op != par_close && exp[i + 1].op != par_open)) {
					std::cout << "error: two non parenthasies expressions in a row" << std::endl;
					return false; //checks for 2 following non-par operators
				}
			}
		}
	}


	if (par_close_count != par_open_count) {
		std::cout << "error: unequal amount of open and close parenthasies" << std::endl;
		return false;
	}
	return true;
}