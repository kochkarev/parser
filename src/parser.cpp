#include <iostream>

#include "parser.hpp"
#include "exception.hpp"

void Parser::S() {
        E();
        while (st_oper.size() != 0) {
            if (st_oper.top() == ASSIGN) {
                poliz.push_back(Lex(LEX_ASSIGN));
            }
            st_oper.pop();
        }
        l_value = true;
}

void Parser::E() {
    E_1();
    while (cur_type == LEX_ASSIGN) {
        if (!l_value) {
            throw SemanticException("In Parser::E(): assignment of non-l_value expression", cur_pos.get_str_num(), cur_pos.get_sym_num());
        }
        gl();
        E_1();
        check_types();
        while(st_oper.size() != 0 && st_oper.top() == SUM) {
            st_oper.pop();
        }
        st_oper.push(ASSIGN);
    }
}

void Parser::E_1() {
    T();
    while (cur_type == LEX_PLUS) {
        l_value = false;
        gl();
        T();
        poliz.push_back(Lex(LEX_PLUS));
        check_types();
        while(st_oper.size() != 0 && st_oper.top() == SUM) {
            st_oper.pop();
        }
        st_oper.push(SUM);
    }
}

void Parser::T() {
    if (cur_type == LEX_IDENT) {
        check_lex();
        st_type.push(cur_lex.get_type());
        poliz.push_back(cur_lex);
        gl();
        F();
    } 
    else if (cur_type == LEX_NUM) {
        l_value = false;
        st_type.push(Type(TYPE_INT, 0));
        poliz.push_back(cur_lex);
        gl();
    }
    else {
        throw SyntaxException("In Parser::T(): expected Identifier", cur_pos.get_str_num(), cur_pos.get_sym_num());
    }
}

void Parser::F() {
    if (cur_type == LEX_LPAREN) {
        std::string func_name = poliz[poliz.size() - 1].get_name();
        poliz.push_back(cur_lex);
        gl();
        for (unsigned i = 2; i < func_name.size(); i++){
            if (func_name[i] == 'i' || func_name[i] == 'j' || func_name[i] == 'k') {
                st_type.push(Type(TYPE_INT, 0));
                E();
                check_types();
            } else if (func_name[i] == 's' || func_name[i] == 't') {
                st_type.push(Type(TYPE_STRING, 0));
                E();
                check_types();
                if (i > 1) {
                    st_type.pop();
                }
            } else if (func_name[i] == 'f') {
                throw SyntaxException("In Parser::F(): wrong function identifier", cur_pos.get_str_num(), cur_pos.get_sym_num());
            }
            if (cur_type != LEX_COMMA && cur_type != LEX_RPAREN) {
                throw SyntaxException("In Parser::F(): wrong actual function parameters", cur_pos.get_str_num(), cur_pos.get_sym_num());
            }
            poliz.push_back(cur_lex);
            gl();
        }
    }
}

void Parser::gl() {
    ++cur_pos;
    cur_lex = *cur_pos;
    cur_type = cur_lex.get_lex_type();
}
 
void Parser::analyze() {
    gl();
    S();
    if (cur_lex.get_lex_type() != LEX_FIN) {
        throw SyntaxException("In Parser::analyze(): wrong lexeme", cur_pos.get_str_num(), cur_pos.get_sym_num());
    }
}

void Parser::check_lex()
{
    std::string name = cur_lex.get_name();
    char sym = 0;
    int f_cnt = 0;
    bool func_flag = false;
    for (auto c : name) {
        if (c != 'i' && c != 'j' && c != 'k' && c != 's' && c != 't' && c != 'a' && c != 'f') {
            throw SyntaxException("In Parser::check_lex(): Undefined lexeme name", cur_pos.get_str_num(), cur_pos.get_sym_num());
        } else if (c == 'a') {
            throw SemanticException("In Parser::check_lex(): Unknown type", cur_pos.get_str_num(), cur_pos.get_sym_num());
        } else if (c == 'f') {
            f_cnt++;
        }
    }
    if (name[0] == 'f') {
        func_flag = true;
    }
    if (func_flag) {
        sym = name[1];
    } else {
        sym = name[0];
    }
    if (!func_flag && name.length() > 1) {
        throw SyntaxException("In Parser::check_lex(): Undefined lexeme name", cur_pos.get_str_num(), cur_pos.get_sym_num());
    }
    if (sym == 'i' || sym == 'j' || sym == 'k') {
        cur_lex.set_type(Type(TYPE_INT, func_flag));
    } else if (sym == 's' || sym == 't') { 
        cur_lex.set_type(Type(TYPE_STRING, func_flag));
    } else {
        throw SyntaxException("In Parser::check_lex(): Wrong lexeme name", cur_pos.get_str_num(), cur_pos.get_sym_num());
    }
}

void Parser::print_poliz () const noexcept {
    std::cout << "Poliz: ";
    for(const Lex& lex: poliz) {
        std::cout << lex << ' ';
    }
    std::cout << std::endl << std::endl;
}

void Parser::check_types()
{
    Type type1, type2;
    type2 = st_type.top();
    st_type.pop();
    type1 = st_type.top();
    st_type.pop();
    if (type1 == type2) {
        st_type.push(type1);
    } else {
        throw SemanticException("In Parser::check_types(): Wrong types", cur_pos.get_str_num(), cur_pos.get_sym_num());
    }
}

void Parser::print_expr() const noexcept{
    std::stack<Lex> st_poliz;
    Lex lex0;
    int i = 1;
    for (Lex cur_lex : poliz) {
        LexType lex_type = cur_lex.get_lex_type();
        if (lex_type != LEX_PLUS && lex_type != LEX_ASSIGN && lex_type != LEX_RPAREN) {
            if (lex_type == LEX_LPAREN) {
                Lex lex1 = st_poliz.top();
                st_poliz.pop();
                Type type = lex1.get_type();
                lex1.set_type(type);
                st_poliz.push(lex1);
            }
            st_poliz.push(cur_lex);
        } else {
            if (lex_type == LEX_RPAREN) {
                do {
                    lex0 = st_poliz.top();
                    st_poliz.pop();
                } while (lex0.get_lex_type() != LEX_LPAREN);
            } else {
                Lex lex1 = st_poliz.top();
                st_poliz.pop();
                Lex lex2 = st_poliz.top();
                st_poliz.pop();
                std::cout << i << ") " << lex2 << ' ' << cur_lex << ' ' << lex1 << " : " << lex2.get_type() << std::endl;
                std::string temp_lex = (cur_lex.get_lex_type() == LEX_PLUS ? " + " : " = ");
                std::string temp_name = lex2.get_name() + temp_lex;
                st_poliz.push(Lex(lex1.get_lex_type(), temp_name + lex1.get_name(), lex1.get_type()));
                ++i;
            }
        }
    }
    std::cout << std::endl << "Expressions: " << std::endl;
    while (!st_poliz.empty()) {
        Lex lex = st_poliz.top();
        std::cout << st_poliz.top() << " : " << lex.get_type() << std::endl;
        ++i;
        st_poliz.pop();
    }
}
