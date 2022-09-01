# 08.14.2022
CFLAGS = -Wall -Werror -pedantic -std=c++17 -g -c
Calculator: main.o evaluate.o scanner.o Operation.o operate.o Operator.o Types.o
	g++ -g main.o evaluate.o scanner.o Operation.o operate.o Operator.o Types.o -o Calculator
main.o: main.cpp interpreter.h
	g++ $(CFLAGS) main.cpp
evaulate.o: evaluate.cpp Operation.cpp operate.cpp Types.cpp interpreter.h
	g++ $(CFLAGS) evaluate.cpp Operation.cpp operate.cpp Types.cpp
scanner.o: scanner.cpp Types.cpp interpreter.h
	g++ $(CFLAGS) scanner.cpp Types.cpp
Operation.o: Operation.cpp operate.cpp Operation.h Token.h
	g++ $(CFLAGS) Operation.cpp operate.cpp
operate.o: operate.cpp Operation.cpp
	g++ $(CFLAGS) operate.cpp
Operator.o: Operator.cpp Operator.h
	g++ $(CFLAGS) Operator.cpp
Types.o: Types.cpp Types.h Token.h
	g++ $(CFLAGS) Types.cpp
clean:
	rm *.o Calculator
