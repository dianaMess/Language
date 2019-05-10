#ifndef __POLIZ_H__
#define __POLIZ_H__
#include<iostream>
#include<stack>
#include<vector>
#include<string>
#include"lexem.h"
using namespace std;
void initLabels(std::vector<Lexem *> &infix, int row);
bool inLabelTable(string name, map<string, int> Ltable);
void joinGotoAndLabel(VARIABLE *lexemvar, stack<Oper *> &stack);
std::vector<Lexem*> buildPoliz(std::vector<Lexem*> &infix, map<string, int> &Ltable);
#endif
