#include <iostream>

#include "lexer.hpp"
#include "exception.hpp"

std::ostream& operator << (std::ostream& os, Lex lex) {
    switch (lex.lex_type) {
       /* case LEX_FIN :
            os << "LEX_FIN" << lex.name;
            break;*/
        case LEX_IDENT :
            os << lex.name;
            break;
        case LEX_NUM :
            os << lex.name;
            break;
        case LEX_COMMA :
            os << ",";
            break;
        case LEX_ASSIGN :
            os << "=";
            break;
        case LEX_PLUS :
            os << "+";
            break;
        case LEX_LPAREN :
            os << "(";
            break;
        case LEX_RPAREN :
            os << ")";
            break;
        /*case LEX_NULL :
            os << "LEX_NULL" << lex.name;
            break;*/
        /*default :
            os << lex.lex_type << " # " << lex.name;
        */
        case LEX_FIN:
        case LEX_NULL:
        default:
            break;
    }
    return os;
}

Lex LexSequenceIterator::operator*() noexcept {
    if (cur_lex.get_lex_type() == LEX_FIN) {
        cur_lex_num = -1;
    }
    return cur_lex;
}

LexSequenceIterator& LexSequenceIterator::operator++() {
    if (cur_lex_num != -1) {
        cur_lex_num++;
    }
    cur_lex = get_lex();
    return *this;
}
 
LexSequenceIterator::LexSequenceIterator(int lex_num) {
    cur_lex_num = lex_num;
    buf.clear();
}

Lex LexSequenceIterator::get_lex () {
    CS = H;
    do {
        if (read_flg) {
            gc();
        } else {
            read_flg = true;
        }
        switch(CS) {
            case H:
                if (isspace(c)) { 
                    if (c == '\n') {
                        ++str_num;
                        sym_num = 0;
                    }
                    break;
                }
                else if (isalpha(c)) {
                    buf.clear();
                    buf += c;
                    CS = IDENT;
                    break;
                }
                else if (isdigit(c)) {
                    buf.clear();
                    buf += c;
                    CS = NUMB;
                    break;
                }
                else if (c == EOF) {
                    buf.clear();
                    return Lex(LEX_FIN);
                }
                else {
                    CS = DELIM;
                    read_flg = false;
                }
                break;
            case IDENT:
                if (isalpha(c) || isdigit(c)) {
                    buf += c;
                }
                else {
                    read_flg = false;
                    return Lex(LEX_IDENT, buf);
                }
                break;
            case NUMB:
                if (isdigit(c)) {
                    buf += c;
                }
                else {
                    read_flg = false;
                    return Lex(LEX_NUM, buf);
                }
                break;
            case DELIM:
                switch(c) {
                    case '+':
                        return Lex(LEX_PLUS);
                    case '=':
                        return Lex(LEX_ASSIGN);
                    case ',':
                        return Lex(LEX_COMMA);
                    case '(':
                        return Lex(LEX_LPAREN);
                    case ')':
                        return Lex(LEX_RPAREN);
                    default:
                        throw LexerException("Undefined symbol", str_num, sym_num);
                }
                break;
        }
  } while (true);
}

std::ostream& operator<<(std::ostream& os, Type print_type) {
    if (print_type.if_func) {
        os << "Function with a return value: ";
    }
    switch (print_type.type) {
    case TYPE_INT :
        os << "integer";
        return os;
    case TYPE_STRING :
        os << "string";
        return os;
    default :
        os << print_type.type;
    return os;
    }
}