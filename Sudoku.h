#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<algorithm>

using namespace std;

typedef struct{
	unsigned int contain: 9;
	unsigned int:0;
}Pixel;

class Sudoku{
	public:
		Sudoku(void);
		void ReadIn(void);
		void Solve(void);
		void GiveQuestion(void);

	private:
		static const int Map[2][12][12];
		int initialCheckUnity(void);
		int getCountSolution(void);
		void getAnswer(void);
		void outOriginMap(void);
		void outputans(void);
		int solveOne(vector<int> Test, vector<Pixel> X, vector<Pixel> Y, vector<Pixel> G);
		int solveMulti(vector<int> Test, vector<Pixel> X, vector<Pixel> Y, vector<Pixel> G);

		vector<Pixel> xlable;
		vector<Pixel> ylable;
		vector<Pixel> gridlable;
		vector<int> map;
		vector< vector<int> > multiAnswer;

};
#endif
