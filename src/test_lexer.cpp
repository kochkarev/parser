#include <iostream>

#include "lexer.hpp"
#include "exception.hpp"

int main() {
	LexSequence seq;
	try {
		for (auto c: seq) {
			std::cout << c << std::endl;
		}
	}
	catch(LexerException &err) {
		std::cout << err.what() << std::endl;
	}
	return 0;
}