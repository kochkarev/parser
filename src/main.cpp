#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "exception.hpp"

int main() {
	LexSequence seq;
	try {
		Parser parser(seq);
		parser.analyze();
		//parser.print_poliz();
		parser.print_expr();
	}
	catch(Exception &err) {
		std::cout << err.what() << std::endl;
	}
	return 0;
}