#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__

class Exception {
public:
	virtual const std::string what() const noexcept = 0;
protected:
	std::string msg;
	int str_num;
	int sym_num;
};

class LexerException : public Exception {
public:
	LexerException(const std::string &mes, int strnum, int symnum) { msg = mes; str_num = strnum; sym_num = symnum; }
	const std::string what() const noexcept {
		std::string tmp = "Lexical error: ";
		tmp += msg; 
		tmp += ":";
		tmp += std::to_string(str_num);
		tmp += ":";
		tmp += std::to_string(sym_num);
		return tmp;
	}
};

class SyntaxException : public Exception {
public:
	SyntaxException(const std::string &mes, int strnum, int symnum) { msg = mes; str_num = strnum; sym_num = symnum; }
	const std::string what() const noexcept {
		std::string tmp = "Syntax error: ";
		tmp += msg; 
		tmp += ":";
		tmp += std::to_string(str_num);
		tmp += ":";
		tmp += std::to_string(sym_num);
		return tmp;
	}
};

class SemanticException : public Exception {
public:
	SemanticException(const std::string &mes, int strnum, int symnum) { msg = mes; str_num = strnum; sym_num = symnum; }
	const std::string what() const noexcept {
		std::string tmp = "Semantic error: ";
		tmp += msg; 
		tmp += ":";
		tmp += std::to_string(str_num);
		tmp += ":";
		tmp += std::to_string(sym_num);
		return tmp;
	}
};

#endif