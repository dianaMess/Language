#include<iostream>
#include"executor.h"
int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout<<"Two arguments needed"<<endl;
        return 1;
    }
    exec A(argv[1]);
    A.run();
    A.PrintTable();
    return 0;
}
