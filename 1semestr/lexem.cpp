#include<iostream>
#include<string>
#include<vector>
#include <stack>
#include<typeinfo>
#include<map>
#include<cctype>
#include<algorithm>
#include<cstdio>
#include"lexem.h"

using namespace std;
    Lexem::Lexem() {}
    void Lexem::print() {};
//    virtual LEXEMTYPE ltype() {};
    int Lexem::getValue() {};
    string Lexem::ltype() {};
    string Lexem::getName() {};
    int Lexem::getValue(int left, int right) {};
    int Number::getValue() {
        return value;
    }
    void Number::print() {
        cout<<value;
    }
    string Number::ltype() {
        return "Number";
    }
    int VARIABLE::getValue() { 
        return value;
    }
    string VARIABLE::getName() {
        return name;
    }

    string VARIABLE::ltype() {
        return "Var";
    }
    void VARIABLE::setValue(int value) {
        VARIABLE::value = value;
        return;
    }
/*    bool VARIABLE::inLabelTable() {
        std::map<string, int>::iterator it;
        it = Ltable.find(name);
        if (it != Ltable.end())
            return true;
        return false;
    }
    bool VARIABLE::inArrTable() {
        std::map<string, vector<int>>::iterator it;
        it = Arr_table.find(name);
        if (it != Arr_table.end())
            return true;
        return false;
    } 
*/
    void VARIABLE::print() {
        cout<<name;
    }
    void Pointer::setValue(int value) {
        *ptr = value;
    }
//    LEXEMTYPE ltype() {
//        return POINTER;
//    }
    string Pointer::ltype() {
        return "Pointer";
    }
    int Pointer::getValue() {
        return *ptr;
    }
    string Pointer::getName() {
        return name;
    }
    int Pointer::getIndex() {
        return index;
    }
    Operator::OPERATOR Operator::getOp(int i) {
        return (OPERATOR)i;
    }
    string Operator::getOper(int i) {
        return OPERCHAR[i];
    }
    int Operator::getSize() {
        return sizeof(OPERCHAR);
    }
    Operator::OPERATOR Oper::getType() {
        return opertype;
    }
    string Oper::ltype() {
        return "Oper";
    }
    string Oper::getName() {
        return OPERCHAR[opertype];
    }
    void Oper::print() {
        cout<<OPERCHAR[opertype];
    }
    int Oper::getPriority() {
        return PRIORITY[opertype];
    }
    int Oper::getValue(int left, int right) {
        if (opertype == PLUS) 
            return left + right;
        if (opertype == MINUS) 
            return left - right;
        if (opertype == MULTIPLY) 
            return left * right;
        if (opertype == OR) 
            return left or right;
        if (opertype == AND) 
            return left and right;
        if (opertype == BITOR) 
            return left | right;
        if (opertype == XOR) 
            return left ^ right;
        if (opertype == BITAND)
            return left & right; 
        if (opertype == EQ)
            return left == right; 
        if (opertype == NEQ) 
            return left != right;
        if (opertype == LEQ)
            return left <= right;
        if (opertype == LT || opertype == TO)
            return left < right; 
        if (opertype == GEQ) 
            return left >= right;
        if (opertype == GT) 
            return left > right;
        if (opertype == DIV)
            return left / right; 
        if (opertype == MOD) 
            return left % right;

        return 0;
    }

    void Goto::setRow(int row) {
        Goto::row = row;
    }
    void Goto::setRow(const std::string &labelname, map<string, int> &Ltable) {
        row = Ltable[labelname];
    }
    void Goto::setForVar(string name) {
        var = name;
    }
    string Goto::getVar() {
        return var;
    }
    void Goto::IncFor(map<string, VARIABLE *> &table) {
        VARIABLE * lexemvar = table[var];
        int val = lexemvar->getValue();
        val += 1;
        lexemvar->setValue(val);
        table[var] = lexemvar;
    }

    int Goto::getRow() {
        return row;
    }
    void Goto::print() {
        cout<<"ROW "<<row<<' '<<OPERCHAR[optype];
    }
Oper *getOperator(const std::string &codeline, int &i) {
    Operator Op;
    string str = "\0";
    int n = Op.getSize() / sizeof(std::string);
    for (int j = 0; j < n; j++) {
        int len = Op.getOper(j).size();
        if (Op.getOper(j) == codeline.substr(i, len) && (codeline.substr(i, len) != str)) {
//           i += len - 1;
           if (codeline.substr(i, len) == "goto" || codeline.substr(i, len) == "if" || codeline.substr(i, len) == "else" || codeline.substr(i, len) == "while" || codeline.substr(i, len) == "endwhile" || codeline.substr(i, len) == "for" || codeline.substr(i, len) == "end") {
               return new Goto(Op.getOp(j));
           }
           i += len - 1;
 
           return new Oper(Op.getOp(j));
        }
    }
    return nullptr;
}


Number * getNumber(string &code, int &i) {
    string word;
    int pos = i;
    int flag = 0;
    for (int j = i; j < code.size(); j++) {
        if (isdigit(code[j])) {
            i = pos;
            flag = 1;
            word = code.substr(pos, j - pos + 1);
            i += word.size() - 1;
        } else {
            return new Number(atoi(word.c_str()));
        }
    }
    if (flag == 1) {
        return new Number(atoi(word.c_str()));
    }
    return nullptr;
}
 
VARIABLE * getWord(string &code, int &i) {
    string word;
    int pos = i;
    int flag = 0;
    for (int j = i; j < code.size(); j++) {
        if (isalpha(code[j])) {
            i = pos;
            flag = 1;
            word = code.substr(pos, j - pos + 1);
            i += word.size() - 1;
        } else {
            return new VARIABLE(word);
        }
    }
    if (flag == 1)
        return new VARIABLE(word);
    return nullptr;
}

std::vector<Lexem*> parseLexem(std::string& input) {
    std::vector<Lexem *> lexems;
    string word;
    int i = 0;
    for (i = 0; i < input.size();) {
        if ((!isalnum(input[i])) && (!isalpha(input[i])) && (input[i] != ' ')) {
            Oper * op = getOperator(input , i);
            lexems.push_back(op);
        }
        if (isdigit(input[i])) {
            Number * num = getNumber(input, i);
            if (num)
                lexems.push_back(num);
        } 
        if (isalpha(input[i])) {
            VARIABLE * P = getWord(input, i);
            VARIABLE * p = P;
            int k = 0;
            Oper * op = getOperator(p->getName(), k);
	    if (op != nullptr) {
                lexems.push_back(op);
            } else
                lexems.push_back(P);
        }
        i++;
       
    }
    return lexems;
}

