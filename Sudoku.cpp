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
	
	int NO,index,orivalue;
	srand( time(NULL) );
	NO=rand()%10;
	for(int i=0;i<12;++i){
		for(int j=0;j<12;j++){
			map[j+12*i]=Map[NO][i][j];
		}
	}
	do{
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
		getans();
		if(solcount()!=1){
//			cout<<"非唯一"<<endl;
			map[index]=orivalue;
			break;
		}
	}
	multiAns.clear();
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
//		cout<<"init"<<endl;
		cout<<solcount()<<endl;
		return;
	}
//	cout<<"before"<<endl;
	getans();							//解算
	cout<<solcount()<<endl;			//print 多or一解
	if(solcount()==1){				//若唯一解，print答案
		outputans();
	}
	return;
}

void Sudoku::outputans(void){

	for(int k=0;k<multiAns.size();++k){
		if(k==1){
			cout<<endl;
		}
		for(int i=0;i<144;++i){
			cout<<multiAns.at(k).at(i)<<" ";
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

	for(int k=0;k<16;++k){
		t.clear();
		int index;
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

int Sudoku::solcount(void){
	return multiAns.size();
}


void Sudoku::getans(void){
	solveOne(  map, xlable, ylable, gridlable );
}


int Sudoku::solveOne(vector<int> Test, vector<Pixel> X, vector<Pixel> Y, vector<Pixel> G){
	int check1;
	int check2;
	int check3;
	int i;
	unsigned int tmpt;
	vector<int> t;

	do{

		check1=0;
		check2=0;

		for(i=0;i<144;++i){
			if(Test[i]==0){
				check1++;
				tmpt=X[i%12].contain & Y[i/12].contain & G[(i/36)*4+((i%12)/3)].contain;

				if( tmpt == 0 ){
//					cout<<"i="<<i<<"  "<<"tmpt=0"<<endl;
//					cout<<"X"<<X[i%12].contain<<" "<<"Y"<<Y[i/12].contain<<" "<<"G"<<G[(i/36)*4+((i%12)/3)].contain<<endl;
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
			multiAns.push_back(Test);
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
	}while(1);
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
					if( multiAns.size() > 1 ){
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




const int Sudoku::Map[10][12][12]=
{
	{
	{8,7,3,9,5,1,-1,-1,-1,4,2,6},
	{9,5,1,6,2,4,-1,-1,-1,8,7,3},
	{6,2,4,8,7,3,-1,-1,-1,9,5,1},
	{1,3,2,4,8,7,9,5,6,-1,-1,-1},
	{7,8,5,1,9,6,4,2,3,-1,-1,-1},
	{4,9,6,2,3,5,8,7,1,-1,-1,-1},
	{2,4,8,-1,-1,-1,6,9,5,1,3,7},
	{5,6,9,-1,-1,-1,1,3,7,2,8,4},
	{3,1,7,-1,-1,-1,2,8,4,6,9,5},
	{-1,-1,-1,7,4,8,5,6,9,3,1,2},
	{-1,-1,-1,5,6,9,3,1,2,7,4,8},
	{-1,-1,-1,3,1,2,7,4,8,5,6,9},
	},
	{
	{ 9,5,1,8,7,3,-1,-1,-1,4,2,6},
	{ 6,2,4,9,5,1,-1,-1,-1,8,7,3},
	{ 8,7,3,6,2,4,-1,-1,-1,9,5,1},
	{ 4,8,7,1,3,2,9,5,6,-1,-1,-1},
	{ 1,9,6,7,8,5,4,2,3,-1,-1,-1},
	{ 2,3,5,4,9,6,8,7,1,-1,-1,-1},
	{ -1,-1,-1,2,4,8,6,9,5,1,3,7},
	{ -1,-1,-1,5,6,9,1,3,7,2,8,4},
	{ -1,-1,-1,3,1,7,2,8,4,6,9,5},
	{ 7,4,8,-1,-1,-1,5,6,9,3,1,2},
	{ 5,6,9,-1,-1,-1,3,1,2,7,4,8},
	{ 3,1,2,-1,-1,-1,7,4,8,5,6,9},
	},
	{
	{ -1,-1,-1,2,4,8,6,9,5,1,3,7},
	{ -1,-1,-1,5,6,9,1,3,7,2,8,4},
	{ -1,-1,-1,3,1,7,2,8,4,6,9,5},
	{ 4,8,7,1,3,2,9,5,6,-1,-1,-1},
	{ 1,9,6,7,8,5,4,2,3,-1,-1,-1},
	{ 2,3,5,4,9,6,8,7,1,-1,-1,-1},
	{ 7,4,8,-1,-1,-1,5,6,9,3,1,2},
	{ 5,6,9,-1,-1,-1,3,1,2,7,4,8},
	{ 3,1,2,-1,-1,-1,7,4,8,5,6,9},
	{ 9,5,1,8,7,3,-1,-1,-1,4,2,6},
	{ 6,2,4,9,5,1,-1,-1,-1,8,7,3},
	{ 8,7,3,6,2,4,-1,-1,-1,9,5,1},
	},
	{
	{ 6,9,5,1,3,7,2,4,8,-1,-1,-1},
	{ 1,3,7,2,8,4,5,6,9,-1,-1,-1},
	{ 2,8,4,6,9,5,3,1,7,-1,-1,-1},
	{ 9,5,6,-1,-1,-1,1,3,2,4,8,7},
	{ 4,2,3,-1,-1,-1,7,8,5,1,9,6},
	{ 8,7,1,-1,-1,-1,4,9,6,2,3,5},
	{ 5,6,9,3,1,2,-1,-1,-1,7,4,8},
	{ 3,1,2,7,4,8,-1,-1,-1,5,6,9},
	{ 7,4,8,5,6,9,-1,-1,-1,3,1,2},
	{ -1,-1,-1,4,2,6,8,7,3,9,5,1},
	{ -1,-1,-1,8,7,3,9,5,1,6,2,4},
	{ -1,-1,-1,9,5,1,6,2,4,8,7,3},
	},
	{
	{ -1,-1,-1,4,2,6,8,7,3,9,5,1},
	{ -1,-1,-1,8,7,3,9,5,1,6,2,4},
	{ -1,-1,-1,9,5,1,6,2,4,8,7,3},
	{ 9,5,6,-1,-1,-1,1,3,2,4,8,7},
	{ 4,2,3,-1,-1,-1,7,8,5,1,9,6},
	{ 8,7,1,-1,-1,-1,4,9,6,2,3,5},
	{ 5,6,9,3,1,2,-1,-1,-1,7,4,8},
	{ 3,1,2,7,4,8,-1,-1,-1,5,6,9},
	{ 7,4,8,5,6,9,-1,-1,-1,3,1,2},
	{ 6,9,5,1,3,7,2,4,8,-1,-1,-1},
	{ 1,3,7,2,8,4,5,6,9,-1,-1,-1},
	{ 2,8,4,6,9,5,3,1,7,-1,-1,-1},
	},
	{
	{ -1,-1,-1,5,3,7,9,4,8,6,1,2},
	{ -1,-1,-1,6,1,4,5,2,7,9,3,8},
	{ -1,-1,-1,9,2,8,6,3,1,5,7,4},
	{ 4,8,9,3,7,5,-1,-1,-1,1,2,6},
	{ 2,7,5,1,4,6,-1,-1,-1,3,8,9},
	{ 6,3,1,2,8,9,-1,-1,-1,7,4,5},
	{ 8,9,6,-1,-1,-1,1,7,4,2,5,3},
	{ 7,5,2,-1,-1,-1,3,8,9,4,6,1},
	{ 3,1,4,-1,-1,-1,2,5,6,8,9,7},
	{ 9,6,8,7,5,3,4,1,2,-1,-1,-1},
	{ 5,2,7,4,6,1,8,9,3,-1,-1,-1},
	{ 1,4,3,8,9,2,7,6,5,-1,-1,-1},
	},
	{
	{ -1,-1,-1,6,3,1,5,7,4,9,2,8},
	{ -1,-1,-1,5,2,7,9,3,8,6,1,4},
	{ -1,-1,-1,9,4,8,6,1,2,5,3,7},
	{ 1,4,3,7,6,5,-1,-1,-1,8,9,2},
	{ 5,2,7,8,9,3,-1,-1,-1,4,6,1},
	{ 9,6,8,4,1,2,-1,-1,-1,7,5,3},
	{ 3,1,4,2,5,6,8,9,7,-1,-1,-1},
	{ 7,5,2,3,8,9,4,6,1,-1,-1,-1},
	{ 8,9,6,1,7,4,2,5,3,-1,-1,-1},
	{ 6,3,1,-1,-1,-1,7,4,5,2,8,9},
	{ 2,7,5,-1,-1,-1,3,8,9,1,4,6},
	{ 4,8,9,-1,-1,-1,1,2,6,3,7,5},
	},
	{
	{ 8,4,7,2,1,3,-1,-1,-1,9,6,5},
	{ 2,1,3,9,6,5,-1,-1,-1,8,4,7},
	{ 9,6,5,8,4,7,-1,-1,-1,2,1,3},
	{ 4,8,2,-1,-1,-1,7,1,3,5,9,6},
	{ 7,1,3,-1,-1,-1,9,6,5,4,8,2},
	{ 5,9,6,-1,-1,-1,8,4,2,7,3,1},
	{ 1,7,8,5,3,2,6,9,4,-1,-1,-1},
	{ 3,2,4,6,9,1,5,8,7,-1,-1,-1},
	{ 6,5,9,7,8,4,2,3,1,-1,-1,-1},
	{ -1,-1,-1,3,7,8,4,2,6,1,5,9},
	{ -1,-1,-1,4,2,6,1,5,9,3,7,8},
	{ -1,-1,-1,1,5,9,3,7,8,6,2,4},
	},
	{
	{ 5,7,3,6,2,1,-1,-1,-1,9,8,4},
	{ 6,4,1,9,8,3,-1,-1,-1,5,7,2},
	{ 9,8,2,5,4,7,-1,-1,-1,1,3,6},
	{ -1,-1,-1,3,5,2,4,7,1,6,9,8},
	{ -1,-1,-1,1,6,4,9,8,3,2,5,7},
	{ -1,-1,-1,7,9,8,6,5,2,4,1,3},
	{ 3,5,7,-1,-1,-1,2,1,4,8,6,9},
	{ 1,6,4,-1,-1,-1,3,9,8,7,2,5},
	{ 2,9,8,-1,-1,-1,5,6,7,3,4,1},
	{ 7,3,5,2,1,6,8,4,9,-1,-1,-1},
	{ 4,1,6,8,3,9,7,2,5,-1,-1,-1},
	{ 8,2,9,4,7,5,1,3,6,-1,-1,-1},
	},
	{
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
	}

};
