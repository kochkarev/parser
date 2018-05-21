#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#include <iostream>
#include <fstream>
#include "iterable.hpp" 

enum LexType {LEX_NULL, LEX_FIN, LEX_COMMA, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_NUM, LEX_IDENT, LEX_PLUS};
enum Types {TYPE_NULL, TYPE_INT, TYPE_STRING, TYPE_FUNCTION};

class Type {
public :
    Types type;
    bool if_func;
    Type(Types type = TYPE_NULL, bool if_func = false) : type(type), if_func(if_func) {}
    bool operator==(const Type &op_type) const noexcept { return (type == op_type.type); }
    bool operator!=(const Type &op_type) const noexcept { return (type != op_type.type); }
    friend std::ostream& operator<<(std::ostream& os, Type print_type);
};

class Lex {
public:
    Lex(LexType t = LEX_NULL, const std::string &name = "", const Type &type = Type()): lex_type (t), name(name), type(type) {}
    LexType get_lex_type() const noexcept { return lex_type; }
    Type get_type() const noexcept { return type; }
    std::string get_name() const noexcept { return name; }
    void set_type(Type new_type) noexcept { type = new_type; }
    friend std::ostream& operator<<(std::ostream &os, Lex lex);
private:
    LexType lex_type;
    std::string name;
    Type type;
};

class LexSequenceIterator : public std::iterator<std::input_iterator_tag, std::string> {
public:
    LexSequenceIterator(int lex_num);
    int get_str_num () const noexcept { return str_num; }
    int get_sym_num () const noexcept { return sym_num; }
    Lex operator*() noexcept;
    LexSequenceIterator& operator++();
    bool operator==(LexSequenceIterator op) const noexcept { return cur_lex_num == op.cur_lex_num; }
    bool operator!=(LexSequenceIterator op) const noexcept { return cur_lex_num != op.cur_lex_num; }
    Lex get_lex();
private:
    Lex cur_lex;
    int cur_lex_num;
    int str_num = 1;
    int sym_num = 0;
    enum state {H, IDENT, NUMB, DELIM};
    state CS = H;
    char c;
    std::string buf;
    bool read_flg = true;
    void gc() { c = getchar(); sym_num++; }
};

class LexSequence : public Iterable<LexSequenceIterator, LexSequenceIterator, Lex> {
public:
    LexSequenceIterator begin() { return LexSequenceIterator(0); }
    LexSequenceIterator begin() const { return LexSequenceIterator(0); }
    LexSequenceIterator end() { return LexSequenceIterator(-1); }
    LexSequenceIterator end() const { return LexSequenceIterator(-1); }
};

#endif