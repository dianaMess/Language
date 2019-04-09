#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__
#include<iostream>
#include<vector>
#include<string>
#include<stack>
#include<map>
#include"lexem.h"
#include"poliz.h"
#include"executor.h"
class exec {
    string filename;
    map<string, int> Ltable;
    map<string, VARIABLE *> table;
    map<string, vector<int>> Arr_table;
public:
    exec(string filename) {
        exec::filename = filename;
    }
    void initLabels(std::vector<Lexem *> &infix, int row);
    void initJumps(std::vector<vector<Lexem *>> &infix);
//    bool inLabelTable();
    bool inArrTable(string name);
    bool inVarTable(VARIABLE *var);
    int EvaluatePoliz(std::vector<Lexem*> poliz, int row);
    void run();
    void PrintTable();
};

#endif
