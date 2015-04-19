all: give_question solve

give_question: give_question.o Sudoku.o
	g++ -o give_question give_question.o Sudoku.o
give_question.o: give_question.cpp Sudoku.h
	g++ -c give_question.cpp

solve: solve.o Sudoku.o
	g++ -o solve solve.o Sudoku.o
solve.o: solve.cpp Sudoku.h
	g++ -c solve.cpp

Sudoku.o: Sudoku.cpp Sudoku.h
	g++ -c Sudoku.cpp

clean:
	rm Sudoku.o give_question.o solve.o
