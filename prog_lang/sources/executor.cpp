#include<iostream>
#include<vector>
#include<string>
#include<stack>
#include<map>
#include"../headers/lexem.h"
#include"../headers/poliz.h"
#include"../headers/executor.h"
void exec::initLabels(std::vector<Lexem *> &infix, int row) {
    for (int i = 1; i < (int)infix.size(); i++) {
        if(infix[i - 1]->ltype() == "Var" && infix[i]->ltype() == "Oper") {
            VARIABLE * lexemvar = (VARIABLE *)infix[i - 1];
            Oper * lexemop = (Oper *)infix[i];
            if (lexemop->getName() == ":") {
                Ltable[lexemvar->getName()] = row;
                delete infix[i - 1];
                delete infix[i];
                infix[i - 1] = nullptr;
                infix[i] = nullptr;
                i++;
            } 
        } else if (infix[i - 1]->ltype() == "Oper" && infix[i]->ltype() == "Var") {
            Oper * lexemop = (Oper *)infix[i - 1];
            VARIABLE * lexemvar = (VARIABLE *)infix[i];
            if (lexemop->getName() == "for") {
                Goto *lexem = (Goto *)lexemop;
                lexem->setForVar(lexemvar->getName());
            }
        }
  
    }
}


void exec::initJumps(std::vector<vector<Lexem *>> &infix) {
    std::stack<Goto *> stackIfElse;
    std::stack<Goto *> stackWhile;
    for (int row = 0; row < (int)infix.size(); row++) {
        for (int i = 0; i < (int)infix[row].size(); i++) {
            if (infix[row][i] == nullptr)
                continue;
            if (infix[row][i]->ltype() == "Oper") {
                Oper  * lexemoper = (Oper *)infix[row][i];
    //            lexemoper->print();
                if (lexemoper->getName() == "if")  
                    stackIfElse.push((Goto *)lexemoper);
                if (lexemoper->getName() == "else") {
   //                 if (stackIfElse.empty())
   //                     cout<<"stack isn't empty"<<endl;
                    stackIfElse.top()->setRow(row + 1);
                    stackIfElse.pop();
                    stackIfElse.push((Goto *)lexemoper);
                }
                if (lexemoper->getName() == "endif") {
     //               if (stackIfElse.empty())
     //                   cout<<"stack isn't empty"<<endl;
 
                    stackIfElse.top()->setRow(row + 1);
                    stackIfElse.pop();
                }
                if (lexemoper->getName() == "while" || lexemoper->getName() == "for") {
                    Goto * lexemgoto = (Goto *)lexemoper;
                    lexemgoto->setRow(row);
                    stackWhile.push(lexemgoto);
                }
                if (lexemoper->getName() == "endwhile" || lexemoper->getName() == "end") {
                    Goto * lexemgoto = (Goto *)lexemoper;
                    if (lexemoper->getName() == "end")
                        lexemgoto->setForVar(stackWhile.top()->getVar());
                    lexemgoto->setRow(stackWhile.top()->getRow());
                    stackWhile.top()->setRow(row + 1);
                    stackWhile.pop();
                }
            }
        }
    }
}
/* 
bool exec::inLabelTable(string name) {
    std::map<string, int>::iterator it;
    it = Ltable.find(name);
    if (it != Ltable.end())
        return true;
    return false;
}
*/
bool exec::inArrTable(string name) {
    std::map<string, vector<int>>::iterator it;
    it = Arr_table.find(name);
    if (it != Arr_table.end())
        return true;
    return false;
} 
bool exec::inVarTable(VARIABLE *var) {
    std::map<string, VARIABLE *>::iterator it;
    it = table.find(var->getName());
    if (it != table.end())
        return true;
    return false;
}
 
int exec::EvaluatePoliz(std::vector<Lexem*> poliz, int row) {
    stack<Lexem *> stack;
    for (int i = 0; i < poliz.size(); i++) {
        if (poliz[i]->ltype() == "Number" || poliz[i]->ltype() == "Var") {
            stack.push(poliz[i]);
        }
        if (poliz[i]->ltype() == "Oper") {
            if (poliz[i]->getName() == "goto" || poliz[i]->getName() == "else" || poliz[i]->getName() == "endwhile") {
                Goto *lexemgoto = (Goto *)poliz[i];
                return lexemgoto->getRow();
            }
            if (poliz[i]->getName() == "end") {
                Goto *lexemgoto = (Goto *)poliz[i];
                lexemgoto->IncFor(table);
                return lexemgoto->getRow();
            }
 
            if (poliz[i]->getName() == "if" || poliz[i]->getName() == "while" || poliz[i]->getName() == "for") {
                Goto * lexemgoto = (Goto *)poliz[i];
                int rvalue = stack.top()->getValue();
                stack.pop();
                if (!rvalue) {
                    return lexemgoto->getRow();
                }
            } else {
                if (poliz[i]->getName() == ":=") {
                    int d;
                    if (stack.top()->ltype() == "Number")
                        d = stack.top()->getValue();
                    else if (stack.top()->ltype() == "Var") { 
                        VARIABLE * ptr = (VARIABLE *)stack.top();
                        if (!(inVarTable(ptr))) {
                            perror("rvalue is needed");
                            exit(0);
                        }
                        VARIABLE *ans = table[ptr->getName()];
                        d = ans->getValue();
                    } else {
                        Pointer * ptr = (Pointer *)stack.top();
                        d = Arr_table[ptr->getName()][ptr->getIndex()];
                    }
                    stack.pop();
                     if (stack.top()->ltype() == "Var") {
                        VARIABLE *ptr = (VARIABLE *)stack.top();
                        ptr->setValue(d);
                        table[ptr->getName()] = ptr;
                    } else {
                        Pointer *ptr = (Pointer *)stack.top();
                        ptr->setValue(d);
                        Arr_table[ptr->getName()][ptr->getIndex()] = d;
                    }
                } else {
                    if (poliz[i]->getName() == "(" || poliz[i]->getName() == ")") {
                        perror("bracket is needed");
                        exit(0);
                    }
                    if (poliz[i]->getName() == "print") {
                        if (stack.top()->ltype() == "Number") {
                            int value = stack.top()->getValue();
                            stack.pop();
                            cout<<value<<' ';
                        } else if (stack.top()->ltype() == "Var") {
                            VARIABLE * ptr = (VARIABLE *)stack.top();
                            VARIABLE * tmp  = table[ptr->getName()];
                            stack.top();
                            cout<<tmp->getValue()<<' ';
                        }
                    } else if (poliz[i]->getName() == "[") {
                        int index = stack.top()->getValue();
                        stack.pop();
                        VARIABLE * array = (VARIABLE *)stack.top();
                        stack.pop();
                        string name = array->getName();
                        if (!(inArrTable(array->getName()))) {
                            vector<int> arr(index + 1);
                            Arr_table[name] = arr;
                        }
                         if (index >= Arr_table[name].size()) 
                            Arr_table[name].resize(index + 1);
                        Pointer * ptr = new Pointer(name, index, Arr_table); 
                        stack.push(ptr);

                    } else if (poliz[i]->getName() == "NEWLINE")
                        cout<<endl;
                    else {
                        Lexem *a, *b;
                        if (stack.top()->ltype() == "Var") {
                            VARIABLE * ptr = (VARIABLE *)stack.top();
                            a = table[ptr->getName()];
                        } else 
                            a = stack.top();
                        stack.pop();
                        if (stack.top()->ltype() == "Var") {
                            VARIABLE * ptr = (VARIABLE *)stack.top();
                            b = table[ptr->getName()];
                        } else 
                            b = stack.top();
                        stack.pop();
                        int ans = poliz[i]->getValue(b->getValue(), a->getValue());
                        stack.push(new Number(ans));
                    }
                } 
            }
        }
    }
    return row + 1;
} 

void exec::run() {
    string expression;
    vector<vector<Lexem *>> poliz, infix;
    int row = 0;
    ifstream fd(filename);
    if (fd.is_open()) {
        while (getline(fd, expression)) 
            infix.push_back(parseLexem(expression));
    } 
    fd.close();

//    while(std::getline(cin, expression, '\n')) {
//        infix.push_back(parseLexem(expression));
//    }
    for (int row = 0; row < (int)infix.size(); row++)
        initLabels(infix[row], row);
    initJumps(infix);
    for (int i = 0; i < (int)infix.size(); i++) {
        poliz.push_back(buildPoliz(infix[i], Ltable));
    }

    cout<<endl;
    while (0 <= row && row < (int)poliz.size()) {
        row = EvaluatePoliz(poliz[row], row);
    }
    return; 
}
void exec::PrintTable() {
    auto it = table.begin();
    for (it = table.begin(); it != table.end(); it++)  
       cout<<it->first<<':'<<it->second->getValue()<<endl;
 
    auto arr_it = Arr_table.begin();
    vector<int>::const_iterator ptr = arr_it->second.begin();
    for (arr_it = Arr_table.begin(); arr_it != Arr_table.end(); arr_it++) {
        cout<<arr_it->first<<':'<<endl;
        for (ptr = arr_it->second.begin(); ptr != arr_it->second.end(); ptr++)  
            cout<<*ptr<<endl;
    }   
}

