install: lexem poliz executor main all
lexem:
	mkdir bin
	g++ -std=c++11  sources/lexem.cpp -o bin/lexem.o -c
poliz:
	g++ -std=c++11  sources/poliz.cpp -o bin/poliz.o -c
executor:
	g++ -std=c++11  sources/executor.cpp -o bin/executor.o -c
main:
	g++ -std=c++11 main.cpp -o bin/main.o -c
all:
	g++ -std=c++11 bin/lexem.o bin/poliz.o bin/executor.o bin/main.o -o main
