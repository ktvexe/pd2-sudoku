#include"Sudoku.h"

Sudoku::Sudoku(void){
	map.clear();
	xlable.clear();
	ylable.clear();
	gridlable.clear();
				
	map.resize(144);
	xlable.resize(12);
	ylable.resize(12);
	gridlable.resize(9);
}

void Sudoku::ReadIn(void){
	for(int i=0;i<144;i++){
		cin>>map[i];
	}
}


void Sudoku::GiveQuestion(void){
	
	srand( time(NULL) );
	int index;
	index=rand()%1;
	for(int j=0;j<12;++j){
		for(int i=0;i<12;++i){
			cout<<Map[index][j][i]<<" ";
		}
		cout<<endl;
	}
}

void Sudoku::Solve(void){
	if( ! initialCheckUnity() ){			//若非唯一
		cout<<getCountSolution()<<endl;
		return;
	}
	getAnswer();							//解算
	cout<<getCountSolution()<<endl;			//print 多or一解
	if(getCountSolution()==1){				//若唯一解，print答案
		outputans();
	}
	return;
}


/*
void Sudoku::outOriginMap(void){
	for(int j=0;j<144;++j){
		cout<<map[j]<<" ";
		if( j%12 == 11 ){
			cout<<endl;
		}
	}
}
*/

const int Sudoku::Map[1][12][12]=
{
	{
	{ 4,2,6,8,7,3,9,5,1,-1,-1,-1},
	{ 0,0,3,9,5,0,6,0,4,-1,-1,-1},
	{ 9,0,1,6,2,4,8,0,0,-1,-1,-1},
	{ -1,-1,-1,1,3,2,0,8,7,9,5,6},
	{ -1,-1,-1,0,8,0,1,9,0,4,2,0},
	{ -1,-1,-1,4,9,6,2,3,0,8,7,1},
	{ 1,0,0,0,4,0,-1,-1,-1,6,9,5},
	{ 0,0,4,0,6,0,-1,-1,-1,1,3,7},
	{ 6,9,5,0,1,7,-1,-1,-1,2,8,4},
	{ 3,1,2,-1,-1,-1,7,4,0,5,0,9},
	{ 7,4,8,-1,-1,-1,0,6,9,3,0,2},
	{ 0,6,0,-1,-1,-1,3,1,0,7,0,8},
	}																
};
