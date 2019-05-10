#ifndef __LEXEM_H__
#define __LEXEM_H__
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <stack>
#include<typeinfo>
#include<map>
#include<cctype>
#include<algorithm>
#include<cstdio>
using namespace std;

class Lexem {
private:
    int k;
public:
    Lexem();
    virtual void print();
    enum LEXEMTYPE {
        OPER, NUMBER, VAR, POINTER, FUNC
    };
//    virtual LEXEMTYPE ltype() {};
    virtual int getValue();
    virtual string ltype();
    virtual string getName();
//    virtual OPERATOR getType() {};
    virtual int getValue(int left, int right);
};

class Number: public Lexem {
    int value;
public:
    Number(int num) {value = num;}
    int getValue();
    void print();
    string ltype();
//    LEXEMTYPE ltype() {
//        return NUMBER;
//    } 
};

//map <string, int> Ltable;
//std::map <std::string, vector <int>> Arr_table;

class VARIABLE: public Lexem {
    int value;
    bool var;
    string name;
public:
    VARIABLE(string var) {
        name = var;
    }
    int getValue();
    string getName();
//    LEXEMTYPE ltype() {
//        return VAR;
//    }
    string ltype();
    void setValue(int value);
//    bool inLabelTable();
//    bool inArrTable();
    void print();
};
//std::map <std::string, VARIABLE *> table;
class Pointer: public Lexem {
    int * ptr;
    int index;
    string name;
public:
    Pointer(string array, int index, map<std::string, vector<int>> &Arr_table) {
        Pointer::index = index;
        name = array;
        ptr = &(Arr_table[array][index]);
    }
    void setValue(int value);
    string ltype();
    int getValue();
    string getName();
    int getIndex();
};
class Operator {
protected:
    enum OPERATOR {
        PRINT, NEWLINE,
        COMMA,
        IF, THEN,
        ELSE, ENDIF,
        WHILE, ENDWHILE,
        FOR, TO, END, DO,
        GOTO, FUNCTION, ASSIGN, COLON,
        LBR, RBR,
        LBRACKET, RBRACKET,
        OR,
        AND,
        BITOR,
        XOR,
        BITAND,
        EQ, NEQ,
        SHL, SHR, 
        LEQ, LT, GEQ, GT,
        PLUS, MINUS,
        MULTIPLY, DIV, MOD
    };
    string OPERCHAR[40] = {
        "print", "NEWLINE",
        ",",
        "if", "then",
        "else", "endif",
        "while", "endwhile",
        "for", "to", "end", "do",
        "goto", "function",":=", ":",
        "[", "]",
        "(", ")",
        "or",
        "and",
        "|", 
        "^", 
        "&",
        "==", "!=",
        "<<", ">>",
        "<=", "<", ">=", ">",
        "+", "-",
        "*", "/", "%"
    };

    int PRIORITY[40] = {
        -8, -8,
        -7,
        -6, -6,
        -5, -5,
        -4, -4,
        -3, -2, -2, -2,
        -1, -1, -1, -1,
        0, 0,
        1, 1,
        2,
        3,
        4,
        5,
        6,
        7, 7,
        8, 8,
        9, 9, 9, 9,
        10, 10,
        11, 11, 11
    };
public:
    OPERATOR getOp(int i);
    string getOper(int i);
    int getSize();
};

class Oper: protected Operator, public Lexem {
    OPERATOR opertype;
public:
    Oper(OPERATOR oper) {opertype = oper;}
    OPERATOR getType();
    string ltype();
    string getName();
    void print();
    int getPriority();
    int getValue(int left, int right);
};

class Goto: public Oper {
    int row;
    Operator::OPERATOR optype;
    string var = "undefined";
public:
    Goto(Operator::OPERATOR optype): Oper(optype) {
        Goto::optype = optype;
    }
    void setRow(int row);
    void setForVar(string name);
    string getVar();
    void IncFor(map<string, VARIABLE *> &table);
    void setRow(const std::string &labelname,  map<string, int> &Ltable);
    int getRow();
    void print();
};

Oper *getOperator(const std::string &codeline, int &i);
Number * getNumber(string &code, int &i);
VARIABLE * getWord(string &code, int &i);
std::vector<Lexem*> parseLexem(std::string& input);
 
#endif
