#include<iostream>
#include<stack>
#include<vector>
#include<string>
#include<cstdio>
#include"lexem.h"
#include"poliz.h"
using namespace std;
void joinGotoAndLabel(VARIABLE *lexemvar, stack<Oper *> &stack, map<string, int> &Ltable) {
    if (stack.top()->getName() == "goto") {
        Goto * lexemgoto = (Goto *)stack.top();
        lexemgoto->setRow(lexemvar->getName(), Ltable);
    }
    return;
}
bool inLabelTable(string name, map<string, int> Ltable) {
    std::map<string, int>::iterator it;
    it = Ltable.find(name);
    if (it != Ltable.end())
        return true;
    return false;
}
std::vector<Lexem*> buildPoliz(std::vector<Lexem*> &infix, map<string, int> &Ltable) {
    vector<Lexem*> poliz;
    stack<Oper *> stack;
    for (auto &lexem: infix) {
        if (lexem == nullptr)
            continue;
        if (lexem->ltype() == "Number") {
            poliz.push_back(lexem);
        }
        if (lexem->ltype() == "Var") {
            VARIABLE *lexemvar = (VARIABLE *) lexem;
            if (inLabelTable(lexemvar->getName(), Ltable))
                joinGotoAndLabel(lexemvar, stack, Ltable);
            else
                poliz.push_back(lexem);
        }
        if (lexem->ltype() == "Oper") {
            Oper *lexemoper = (Oper *)lexem;
            if (lexemoper->getName() == "endif" || lexemoper->getName() == "then" || lexemoper->getName() == "do")
                continue;
            if (lexemoper->getName() == ",") {
                poliz.push_back(lexemoper);
                continue;
            }
            if (lexemoper->getName() == ")" || lexemoper->getName() == "]") {
                if (lexemoper->getName() == ")") {
                    while (stack.top()->getName() != "(") {
                        poliz.push_back(stack.top());
                        stack.pop();
                        if (stack.empty()) {
                            perror("incorrect expression");
                            exit(0);
                        }
                    }
                    stack.pop();
//                    while(!FunStack.empty()) {
//                        poliz.push_back(FunStack.top());
//                        FunStack.pop();
//                    }
                } else {
                    while (stack.top()->getName() != "[") {
                        poliz.push_back(stack.top());
                        stack.pop();
                    } 
                    poliz.push_back(stack.top());
                    stack.pop();
                }
            } else {
//                if (lexemoper->getType() == LBR && stack.top()->getType() == LBR) {
//                    stack.push(lexemoper);
//                    continue;
//                }
                if (lexemoper->getName() != "(" && lexemoper->getName() != "[")
                     while (!stack.empty() && (stack.top()->getPriority() >= lexemoper->getPriority())) {
                        poliz.push_back(stack.top());
                        stack.pop();
                    }
                stack.push(lexemoper);
            }
        }
    } 
    while(!stack.empty()) {
        poliz.push_back(stack.top());
        stack.pop();
    }
    return poliz;          
}

