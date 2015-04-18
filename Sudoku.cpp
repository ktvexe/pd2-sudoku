#include"Sudoku.h"
#include <iomanip>

Sudoku::Sudoku(void){
	map.clear();
	xlable.clear();
	ylable.clear();
	gridlable.clear();
				
	map.resize(144);
	xlable.resize(12);
	ylable.resize(12);
	gridlable.resize(16);
}

void Sudoku::ReadIn(void){
	for(int i=0;i<144;i++){
		cin>>map[i];
	}
}


void Sudoku::GiveQuestion(void){
	
	srand( time(NULL) );
	for(int i=0;i<12;++i){
		for(int j=0;j<12;j++){
			map[j+12*i]=Map[i][j];
		}
	}
	do{
	int index,orivalue;
	index=rand()%144;
//	cout<<"index="<<index<<endl;
	if(map[index]>0){
		orivalue = map[index];
		map[index]= 0;
		if(!initialCheckUnity()){
//			cout<<"無解"<<endl;
			map[index]=orivalue;
			break;
		}
		getAnswer();
		if(getCountSolution()!=1){
//			cout<<"非唯一"<<endl;
			map[index]=orivalue;
			break;
		}
	}
	multiAnswer.clear();
	}while(1);
	for(int i=0;i<12;++i){
		for(int j=0;j<12;++j){
			cout<<setw(3)<<map[j+i*12];
//			if(map[j+i*12] != -1)
//				cout<<" ";
		}
		cout<<endl;
	}

}

void Sudoku::Solve(void){
	if( ! initialCheckUnity() ){			//若無解
		cout<<"init"<<endl;
		cout<<getCountSolution()<<endl;
		return;
	}
	cout<<"before"<<endl;
	getAnswer();							//解算
	cout<<getCountSolution()<<endl;			//print 多or一解
	if(getCountSolution()==1){				//若唯一解，print答案
		outputans();
	}
	return;
}

void Sudoku::outputans(void){

	for(int k=0;k<multiAnswer.size();++k){
		if(k==1){
			cout<<endl;
		}
		for(int i=0;i<144;++i){
			cout<<multiAnswer.at(k).at(i)<<" ";
			if( i%12 == 11 ){
			cout<<endl;
			}
		}
	}
}


int Sudoku::initialCheckUnity(void){

	for(int i=0;i<12;++i){
		ylable[i].contain=0;
		xlable[i].contain=0;
	}
	for(int i=0;i<16;++i){
		gridlable[i].contain=0;
	}

	vector<int>::iterator it;
	vector<int> t;

	for(int j=0;j<12;++j){
		t.clear();
		for(int i=0;i<12;++i){
			if(map[12*j+i]>0 ){
				ylable[j].contain|=(0x0001<<(map[12*j+i]-1));
				t.push_back(map[12*j+i]);
			}
		}
		t.push_back(0);
		it=adjacent_find(t.begin(),t.end());
		if( it != t.end() ){
			cout<<"ylabel"<<endl;
			return 0;
		}
	}

	for(int i=0;i<12;++i){
		t.clear();
		for(int j=0;j<12;++j){
			if(map[12*j+i]>0){
				xlable[i].contain|=(0x0001<<(map[12*j+i]-1));
				t.push_back(map[12*j+i]);
			}
		}
		t.push_back(0);
		it=adjacent_find(t.begin(),t.end());
		if( it != t.end() ){
			cout<<"xlabel"<<endl;
			return 0;
		}
	}

	for(int k=0,index;k<16;++k){
		t.clear();
		for(int i=0;i<9;++i){
			index=(k/4)*36 + (k%4)*3 + (i/3)*12 + (i%3);
			if( map[index] > 0 ){
				gridlable[k].contain |=(0x0001<<(map[index]-1));
				t.push_back(map[index]);
			}
		}
	
		t.push_back(0);
		it=adjacent_find(t.begin(),t.end());
		if(it !=t.end()){
			cout<<"grid"<<endl;
			return 0;
		}
	}

	for(int i=0;i<12;++i){
		ylable[i].contain= ~ylable[i].contain;
		xlable[i].contain= ~xlable[i].contain;
	}
	for(int i=0;i<16;++i){
	gridlable[i].contain= ~gridlable[i].contain;
	}

	return 1;
}

int Sudoku::getCountSolution(void){
	return multiAnswer.size();
}


void Sudoku::getAnswer(void){
	solveOne(  map, xlable, ylable, gridlable );
}


int Sudoku::solveOne(vector<int> Test, vector<Pixel> X, vector<Pixel> Y, vector<Pixel> G){
	int check1;
	int check2;
	int check3;
	int i;
	unsigned int tmpt;
	vector<int> t;

	while(1){

		check1=0;
		check2=0;

		for(i=0;i<144;++i){
			if(Test[i]==0){
				check1++;
				tmpt=X[i%12].contain & Y[i/12].contain & G[(i/36)*4+((i%12)/3)].contain;

				if( tmpt == 0 ){
					cout<<"i="<<i<<"  "<<"tmpt=0"<<endl;
					cout<<"X"<<X[i%12].contain<<" "<<"Y"<<Y[i/12].contain<<" "<<"G"<<G[(i/36)*4+((i%12)/3)].contain<<endl;
					return 0;
				}

				t.clear();
				for(int n=1;n<10;++n){
					if( ( tmpt>>(n-1) ) & 0x0001 ){
						t.push_back(n);
					}
				}

				if( t.size()==1 ){
					check2++;
					Test[i]=t.at(0);
					X[i%12].contain &= ~( 0x0001 << (t.at(0)-1) );
					Y[i/12].contain &= ~( 0x0001 << (t.at(0)-1) );
					G[(i/36)*4+((i%12)/3)].contain &= ~( 0x0001 << (t.at(0)-1) );
				}

			}//if(test[i]==0)
		}//for(i<81)

		if(check1==0){
			multiAnswer.push_back(Test);
			return 1;//good answer output
		}

		if(check2==0){
			if( solveMulti(Test,X,Y,G) ){
				return 1;
			}
			else{
				return 0;
			}
		}
	}
}



int Sudoku::solveMulti(vector<int> Test, vector<Pixel> X, vector<Pixel> Y, vector<Pixel> G){

	vector<int> t;
	unsigned int tmpt;

	for(int i=0;i<144;++i){
		if(Test[i]==0){

			tmpt=X[i%12].contain & Y[i/12].contain & G[(i/36)*4+((i%12)/3)].contain;

			t.clear();
			for(int n=1;n<10;++n){
				if( ( tmpt>>(n-1) ) & 0x0001 ){
					t.push_back(n);
				}
			}

			for(int k=0;k<t.size();++k){

				Test[i]=t.at(k);
				X[i%12].contain &= ~( 0x0001 << (t.at(k)-1) );
				Y[i/12].contain &= ~( 0x0001 << (t.at(k)-1) );
				G[(i/36)*4+((i%12)/3)].contain &= ~( 0x0001 << (t.at(k)-1) );

				if( solveOne( Test, X, Y, G) == 1 ){
					if( multiAnswer.size() > 1 ){
						return 1;
					}
				}

				X[i%12].contain |= ( 0x0001 << (t.at(k)-1) );
				Y[i/12].contain |= ( 0x0001 << (t.at(k)-1) );
				G[(i/36)*4+((i%12)/3)].contain |= ( 0x0001 << (t.at(k)-1) );
			}

			return 0;//guess all and failure

		}//if(test[i]==0)

	}//for(i<81)

}//function end



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

const int Sudoku::Map[12][12]=
{
/*	{
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
*/	
	{ 4,2,6,8,7,3,9,5,1,-1,-1,-1},
	{ 8,7,3,9,5,1,6,2,4,-1,-1,-1},
	{ 9,5,1,6,2,4,8,7,3,-1,-1,-1},
	{ -1,-1,-1,1,3,2,4,8,7,9,5,6},
	{ -1,-1,-1,7,8,5,1,9,6,4,2,3},
	{ -1,-1,-1,4,9,6,2,3,5,8,7,1},
	{ 1,3,7,2,4,8,-1,-1,-1,6,9,5},
	{ 2,8,4,5,6,9,-1,-1,-1,1,3,7},
	{ 6,9,5,3,1,7,-1,-1,-1,2,8,4},
	{ 3,1,2,-1,-1,-1,7,4,8,5,6,9},
	{ 7,4,8,-1,-1,-1,5,6,9,3,1,2},
	{ 5,6,9,-1,-1,-1,3,1,2,7,4,8},
	
};
