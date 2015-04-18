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

