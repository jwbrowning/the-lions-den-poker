#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

using namespace std;

void waitForTurn(string fileName,float t);
void readAndDisplayInfo();
void exportAction(string fileName);
char intToCharValue(int i);
char intToCharSuit(int i);

int main() {

	cout<<"Which player are you? (1|2)\n";
	int p;
	cin>>p;
	string inFileName;
	if(p==1) {
		inFileName="ctrl1";
	} else {
		inFileName="ctrl2";
	}
	string outFileName = "ctrl";

	while(true) {

		waitForTurn(inFileName,.1);
		readAndDisplayInfo();
		exportAction(outFileName);

	}

}

void waitForTurn(string fileName,float t) {
	bool exists = false;
	while(!exists) {
		ifstream ctrl(fileName);
		if(ctrl.good()) {
			exists=true;
			remove("ctrl1");
		}
		time_t start = time(0);
		time_t end = time(0);

		while(difftime(end,start)<t) {
			end = time(0);
		}
	}
}

void readAndDisplayInfo() {
	ifstream info("info.txt");
	int count;
	info>>count;
	string prevAction;
	info>>prevAction;
	int hand[2][2]={0};
	info>>hand[0][0]>>hand[0][1]>>hand[1][0]>>hand[1][1];

	int myStack,opStack;
	info>>myStack>>opStack;
	int pot,bet1,bet2;
	info>>pot>>bet1>>bet2;
	int board[5][2]={0};
	int c=0;
	if(!info.eof()) {
		for(int i=0;i<6;i++) {
			info>>board[i/2][i%2];
		}
		c=3;
	}
	if(!info.eof()) {
		info>>board[3][0]>>board[3][1];
		c=4;
	}
	if(!info.eof()) {
		info>>board[4][0]>>board[3][1];
		c=5;
	}

	cout<<"\n\n\n\n\n";
	for(int i=0;i<c;i++) {
		cout<<"["<<intToCharValue(board[i][0])<<intToCharSuit(board[i][1])<<"]";
	}
	cout<<"\n";
	cout<<"["<<intToCharValue(hand[0][0])<<intToCharSuit(hand[0][1])<<"]["<<intToCharValue(hand[1][0])<<intToCharSuit(hand[1][1])<<"]\n";
	cout<<myStack<<" "<<opStack<<"\n";
	cout<<pot<<endl;
	cout<<bet1<<" "<<bet2<<endl;
	cout<<"\nYour turn.\n";
}

void exportAction(string fileName) {
	string action;
	cin>>action;
	ofstream info;
	info.open("info.txt");
	if(action=="raise") {
		int amt;
		cin>>amt;
		info<<action<<" "<<amt;
	} else {
		info<<action;
	}
	info.close();

	ofstream ctrl;
	ctrl.open(fileName);
	ctrl.close();
}

char intToCharValue(int i) {
	switch(i) {
		case 10:
			return 'T';
			break;
		case 11:
			return 'J';
			break;
		case 12:
			return 'Q';
			break;
		case 13:
			return 'K';
			break;
		case 14:
			return 'A';
			break;
		default:
			return '0'+i;
			break;
	}
}

char intToCharSuit(int i) {
	switch(i) {
		case 1:
			return 'c';
			break;
		case 2:
			return 'd';
			break;
		case 3:
			return 'h';
			break;
		case 4:
			return 's';
			break;
		default:
			return 'x';
	}
}








