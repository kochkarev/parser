#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <vector>
#include <stack>
#include "lexer.hpp"

enum OpType {ASSIGN, SUM};

class Parser {
public:
    Parser(LexSequence& lex_seq) : cur_pos(lex_seq.begin()) {}
    void analyze();
    void print_poliz() const noexcept;
    void print_expr() const noexcept; 
private:
    std::vector<Lex> poliz;
    std::stack<Type> st_type;
    std::stack<OpType> st_oper;
    Lex cur_lex;
    LexSequenceIterator cur_pos;
    LexType cur_type;
    bool l_value = true;
    void S();
    void E(); 
    void E_1();
    void T(); 
    void F();
    void check_lex();
    void check_types();
    void gl();
};

#endif
