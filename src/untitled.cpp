#include <iostream.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
 
enum type_of_lex {
    LEX_NULL, /*0*/
    LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_INT, /*9*/
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE, /*18*/
    LEX_FIN, /*19*/
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, /*27*/
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, /*35*/
    LEX_NUM, /*36*/
    LEX_ID, /*37*/
    POLIZ_LABEL, /*38*/
    POLIZ_ADDRESS, /*39*/
    POLIZ_GO, /*40*/
    POLIZ_FGO
}; 
 
class Lex {
    type_of_lex t_lex;
    int v_lex;
public:
    Lex(type_of_lex t = LEX_NULL, int v = 0): t_lex (t), v_lex (v) {}
    type_of_lex get_type()  { return t_lex; }
    int get_value() { return v_lex; }
    friend ostream& operator<<(ostream &s, Lex l) { s << '(' << l.t_lex << ',' << l.v_lex << ");"; return s; }
};
 
class Ident {
     char *name;
     bool declare;
     type_of_lex type;
     bool assign;
     int value;
public:
    Ident() { declare = false; assign = false; }
    char *get_name() { return name; }
    void put_name(const char *n) { name = new char [strlen(n) + 1]; strcpy(name, n); }
    bool get_declare() { return declare; }
    void put_declare() { declare = true; }
    type_of_lex get_type() { return type; }
    void put_type(type_of_lex t) { type = t; }
    bool get_assign() { return assign; }
    void put_assign() { assign = true; }
    int get_value() { return value; }
    void put_value(int v) { value = v; }
};
 
class Tabl_ident {
    Ident *p;
    int size;
    int top;
public:
    Tabl_ident(int max_size) { p = new Ident [size = max_size]; top = 1; }
    ~Tabl_ident() { delete [] p; }
    Ident &operator[](int k) { return p[k]; }
    int put(const char *buf);
};
 
int Tabl_ident::put(const char *buf) {
    for (int j = 1; j < top; j++)
    if (!strcmp(buf, p[j].get_name())) {
        return j;
    }
    p[top].put_name(buf);
    ++top;
    return top - 1;
} 

class Scanner
{
    enum state {H, IDENT, NUMB, COM, ALE, DELIM, NEQ};
    static char *TW[];
    static type_of_lex words[];
    static char *TD[];
    static type_of_lex dlms[];
    state CS;
    FILE *fp;
    char c;
    char buf[80];
    int buf_top;
    void clear() {
        buf_top = 0;
        for (int j = 0; j < 80; j++)
        buf[j] = '\0';
    }
    void add() { buf [buf_top++] = c; }
    int look(const char *buf, char **list) {
        int i = 0;
        while (list[i]) {
            if (!strcmp(buf, list[i]))
            return i;
            ++i;
        }
        return 0;
    }
    void gc() { c = fgetc(fp); }
public:
    Scanner(const char * program) {
        fp = fopen(program, "r");
        CS = H;
        clear();
        gc();
    }
    Lex get_lex();
};
 
char *Scanner::TW[] = {"", "and", "begin", "bool", "do", "else", "end", "if", "false", "int", "not", "or", "program",
                            "read", "then", "true", "var", "while", "write", NULL};
 
type_of_lex Scanner::words[] = {LEX_NULL, LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_INT,
                     LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE, LEX_NULL};
 
char *Scanner::TD[] = {"", "@", ";", ",", ":", ":=", "(", ")", "=", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", NULL};
 
type_of_lex Scanner::dlms[] = {LEX_NULL, LEX_FIN, LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ,
                     LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_NULL};
 
Lex Scanner::get_lex () {
    int d, j;
    CS = H;
    do {
        switch(CS) {
            case H:
                if (c == ' ' || c == '\n' || c == '\r' || c == '\t') { 
                    gc();
                }
                else if (isalpha(c)) {
                    clear();
                    add();
                    gc();
                    CS = IDENT;
                }
                else if (isdigit(c)) {
                    d = c - '0';
                    gc();
                    CS = NUMB;
                }
                else if (c == '{') {
                    gc();
                    CS = COM;
                }
                else if (c== ':' || c== '<' || c== '>') { 
                    clear(); 
                    add(); 
                    gc(); 
                    CS = ALE; 
                }
                else if (c == '@') {
                    return Lex(LEX_FIN);
                }
                else if (c == '!') {
                    clear();
                    add();
                    gc();
                    CS = NEQ;
                }
                else {
                    CS = DELIM;
                }
            break;

            case IDENT:
                if (isalpha(c) || isdigit(c)) {
                    add(); 
                    gc();
                }
                else if (j = look(buf, TW)) {
                    return Lex (words[j], j);
                }
                else {
                    j = TID.put(buf);
                    return Lex (LEX_ID, j);
                }
            break;

            case NUMB:
                if (isdigit(c)) {
                    d = d * 10 + (c - '0'); gc();
                }
                else {
                    return Lex ( LEX_NUM, d);
                }
            break;

            case COM:
            if (c == '}') {
                gc();
                CS = H;
            }
            else if (c == '@' || c == '{') {
                throw c;
            }
            else {
                gc();
            }
            break;

            case ALE:
                if (c== '=') {
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }
                else {
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }
            break;

            case NEQ:
                if (c == '=') {
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(LEX_NEQ, j);
                }
                else {
                    throw '!';
                }
            break;

            case DELIM:
                clear();
                add();
                if (j = look(buf, TD)) {
                    gc();
                    return Lex(dlms[j], j);
                }
                else {
                    throw c;
                }
            break;
        }
  } while (true);
}

class Poliz
{
    Lex *p;
    int size;
    int free;
public:
    Poliz(int max_size) {
        p = new Lex [size = max_size];
        free = 0;
    }
    ~Poliz() { delete [] p; }
    void put_lex(Lex l) {
        p [free] = l;
        free++;
    }
    void put_lex(Lex l, int place) { p [place] = l; }
    void blank() { free++; }
    int get_free() { return free; }
    Lex &operator[](int index) {
        if (index > size) {
            throw "POLIZ:out of array";
        }
        else {
            if (index > free) {
                throw "POLIZ:indefinite element of array";
            }
        }
        else {
            return p[index];
        }
    }
    void print() {
        for (int i = 0; i < free; ++i) {
            cout << p[i];
        }
    }
};

class Parser {
    Lex curr_lex;
    type_of_lex c_type;
    int c_val;
    Scanner scan;
    Stack<int, 100> st_int;
    Stack<type_of_lex, 100> st_lex;

    void E();
    void E_0();
    void E_1();
    void E_2();
    void T(); 
    void T_1();
    void F();

    void dec(type_of_lex type);
    void check_id();
    void check_op();
    void check_not();
    void eq_type();
    void eq_bool();
    void check_id_in_read();
    void gl() {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }
public:
    Poliz prog;
    Parser(const char *program) : scan(program),prog(1000) {}
    void analyze();
};

void Parser::E() {
    E_1();
    E_2();
}

void Parser::E_1() {
    T();
    T_1();
}

void Parser::E_2() {
    if (c_type == LEX_ASSIGN) {
        gl();
        E_1();
    } else if (c_type == LEX_NULL) {
        gl();
    } 
    else {
        throw curr_lex;
    }
}

void Parser::T_1() {
    if (c_type == LEX_PLUS) {
        gl();
        T();
    } else if (c_type == LEX_NULL) {
        gl();
    }
    else {
        throw curr_lex;
    }
}

void Parser::E_0() {
    if (c_type == LEX_COMMA) {
        gl();
        E();
    } else if (c_type == LEX_NULL) {
        gl();
    }
    else {
        throw curr_lex;
    }
}

void Parser::T() {
    if (c_type == LEX_ID) {
        gl();
    } else {
        gl();
        F();
    }
}

void Parser::F() {
    if (c_type == LEX_ID) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            E();
            E_0();
            if (c_type == LEX_RPAREN) {
                gl();
            } else {
                throw curr_lex;
            }
        } else {
            throw curr_lex;
        }
    } else {
        throw curr_lex;
    }
}
 
void Parser::analyze() {
    gl();
    E();
    prog.print();
    cout << endl << "Yes!!!" << endl;
}

void Parser::dec(type_of_lex type) {
    int i;
    while (!st_int.is_empty()) {
        i = st_int.pop();
        if (TID[i].get_declare()) {
            throw "twice";
        }
        else {
            TID[i].put_declare();
            TID[i].put_type(type);
        }
    }
}
 
void Parser::check_id() {
    if ( TID[c_val].get_declare() )
    st_lex.push ( TID[c_val].get_type() );
  else 
    throw "not declared";
}
 
void Parser::check_op () {
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;
    t2 = st_lex.pop();
    op = st_lex.pop();
    t1 = st_lex.pop();
    if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH) {
        r = LEX_INT;
    }
    if (op == LEX_OR || op == LEX_AND) {
        t = LEX_BOOL;
    }
    if (t1 == t2  &&  t1 == t) {
        st_lex.push(r);
    }
    else {
        throw "wrong types are in operation";
    }
    prog.put_lex (Lex (op) );
}
 
void Parser::check_not() {
    if (st_lex.pop() != LEX_BOOL) {
        throw "wrong type is in not";
    }
    else {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_NOT));
    }
}
 
void Parser::eq_type() {
    if (st_lex.pop() != st_lex.pop()) {
        throw "wrong types are in :=";
    }
}
 
void Parser::eq_bool () {
    if (st_lex.pop() != LEX_BOOL) {
        throw "expression is not boolean";
    }
}
 
void Parser::check_id_in_read() {
    if (!TID[c_val].get_declare()) {
        throw "not declared";
    }
}
 
class Executer {
    Lex pc_el;
public:
    void execute(Poliz& prog);
};
 
void Executer::execute (Poliz& prog) {
    Stack < int, 100 > args;
    int i, j, index = 0, size = prog.get_free();
    while (index < size) {
    pc_el = prog[index];
    switch (pc_el.get_type()) {
        case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case POLIZ_ADDRESS: case POLIZ_LABEL:
            args.push(pc_el.get_value());
            break;
        case LEX_ID:
            i = pc_el.get_value();
        if (TID[i].get_assign()) {
            args.push ( TID[i].get_value () );
            break;
        }
        else {
            throw "POLIZ: indefinite identifier";
        }
        case LEX_NOT:
            args.push( !args.pop() );
            break;
        case LEX_OR:
            i = args.pop();
            args.push(args.pop() || i);
            break;
        case LEX_AND:
            i = args.pop();
            args.push(args.pop() && i);
            break;
        case POLIZ_GO:
            index = args.pop() - 1;
            break;
        case POLIZ_FGO:
            i = args.pop();
            if (!args.pop()) {
                index = i-1;
            }
            break;
        case LEX_WRITE:
            cout << args.pop () << endl;
            break;
        case LEX_READ: {
            int k;
            i = args.pop ();
            if ( TID[i].get_type () == LEX_INT ) {
                cout << "Input int value for" << TID[i].get_name () << endl;
                cin >> k;
            }
            else {
                char j[20];
                rep:
                cout << "Input boolean value (true or false) for" << TID[i].get_name() << endl;
                cin >> j;
                if (!strcmp(j, "true")) {
                    k = 1;
                }
                else if (!strcmp(j, "false")) {
                    k = 0;
                }
                else {
                    cout << "Error in input:true/false" << endl;
                    goto rep;
                }
            }
            TID[i].put_value(k);
            TID[i].put_assign();
            break;
        }
        case LEX_PLUS:
            args.push(args.pop() + args.pop());
            break;
        case LEX_TIMES:
            args.push(args.pop() * args.pop());
            break;
        case LEX_MINUS:
            i = args.pop();
            args.push(args.pop() - i);
            break;
        case LEX_SLASH:
            i = args.pop();
            if (!i) {
                args.push(args.pop() / i);
                break;
            }
            else {
                throw "POLIZ:divide by zero";
            }
        case LEX_EQ:
            args.push(args.pop() == args.pop());
            break;
        case LEX_LSS:
            i = args.pop();
            args.push(args.pop() < i);
            break;
        case LEX_GTR:
            i = args.pop();
            args.push(args.pop() > i);
            break;
        case LEX_LEQ:
            i = args.pop();
            args.push(args.pop() <= i);
            break;
        case LEX_GEQ:
            i = args.pop();
            args.push(args.pop() >= i);
            break;
        case LEX_NEQ:
            i = args.pop();
            args.push(args.pop() != i);
            break;
        case LEX_ASSIGN:
            i = args.pop();
            j = args.pop();
            TID[j].put_value(i);
            TID[j].put_assign(); 
            break;
        default:
            throw "POLIZ: unexpected elem";
    }
    ++index;
    };
    cout << "Finish of executing!!!" << endl;
}
 
class Interpretator {
    Parser pars;
    Executer E;
public:
    Interpretator(char* program): pars (program) {}
    void interpretation();
};
 
void Interpretator::interpretation() {
    pars.analyze ();
    E.execute(pars.prog);
}








