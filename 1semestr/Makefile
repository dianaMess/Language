install: lexem poliz executor main all
lexem:
	g++ -std=c++11 lexem.cpp -o lexem.o -c
poliz:
	g++ -std=c++11 poliz.cpp -o poliz.o -c
executor:
	g++ -std=c++11 executor.cpp -o executor.o -c
main:
	g++ -std=c++11 main.cpp -o main.o -c
all:
	g++ -std=c++11 lexem.o poliz.o executor.o main.o -o main
