#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

using namespace std;

double strengthOfBestHand(int cards[7][2],int data[5][2],int n,int r,int s,int d);
bool hasDupes(int cards[5][2]);
double strengthOfHand(int cards[5][2]);
char intToCharValue(int c);
char intToCharSuit(int c);

//
// Heads Up No Limit Texas Hold'em: Doyle's Game wow thats a long name for a game
// 20,000 chips each, 50/100 blinds, stacks reset after each hand and net score is tracked separately
// The Lion's Den acts as a dealer and tracker
//

int main() {

	srand(time(0));

	remove("ctrl1");
	remove("ctrl2");
	remove("ctrl");

	cout<<"Who is playing?\n";
	string engineName1,engineName2;
	cin>>engineName1>>engineName2;
	cout<<"How many hands?\n";
	int numHands;
	cin>>numHands;
	int net1=0,net2=0;
	int data1[numHands];
	int data2[numHands];

	for(int i=0;i<numHands;i++) {

		int deck[52][2] = {
			{2,1},{3,1},{4,1},{5,1},{6,1},{7,1},{8,1},{9,1},{10,1},{11,1},{12,1},{13,1},{14,1},
			{2,2},{3,2},{4,2},{5,2},{6,2},{7,2},{8,2},{9,2},{10,2},{11,2},{12,2},{13,2},{14,2},
			{2,3},{3,3},{4,3},{5,3},{6,3},{7,3},{8,3},{9,3},{10,3},{11,3},{12,3},{13,3},{14,3},
			{2,4},{3,4},{4,4},{5,4},{6,4},{7,4},{8,4},{9,4},{10,4},{11,4},{12,4},{13,4},{14,4},
		};

		//get 9 unique indexes for 9 unique cards from deck
		int indexes[9];
		for(int j=0;j<9;j++) {
			bool unique = true;
			do {
				unique=true;
				indexes[j] = rand()%52;
				for(int k=0;k<j;k++) {
					if (indexes[j]==indexes[k]) {
						unique = false;
					}
				}
			} while(!unique);
		}

		//set hand variables and crap
		int hand1[2][2];
		for(int j=0;j<4;j++) {
			hand1[j/2][j%2] = deck[indexes[j/2]][j%2];
		}
		int hand2[2][2];
		for(int j=0;j<4;j++) {
			hand2[j/2][j%2] = deck[indexes[j/2+2]][j%2];
		}
		int flop[3][2];
		for(int j=0;j<6;j++) {
			flop[j/2][j%2] = deck[indexes[j/2+4]][j%2];
		}
		//already used "turn" as a variable so ill just slap a c on the end
		int turnc[1][2];
		turnc[0][0] = deck[indexes[7]][0];
		turnc[0][1] = deck[indexes[7]][1];
		int river[1][2];
		river[0][0] = deck[indexes[8]][0];
		river[0][1] = deck[indexes[8]][1];

		//7 card hands that ill use to determine hand strength later
		int cards1[7][2];
		int cards2[7][2];
		for(int j=0;j<4;j++) {
			cards1[j/2][j%2] = hand1[j/2][j%2];
			cards2[j/2][j%2] = hand2[j/2][j%2];
		}
		for(int j=0;j<6;j++) {
			cards1[j/2+2][j%2] = flop[j/2][j%2];
			cards2[j/2+2][j%2] = flop[j/2][j%2];
		}
		for(int j=0;j<2;j++) {
			cards1[j/2+5][j%2] = turnc[j/2][j%2];
			cards1[j/2+6][j%2] = river[j/2][j%2];
			cards2[j/2+5][j%2] = turnc[j/2][j%2];
			cards2[j/2+6][j%2] = river[j/2][j%2];
		}

		//output hand info, might want to log this stuff later
		//i think there are good ways of logging hand histories with player actions and stuff, ill look into that later
		// cout<<"|"<<hand1[0][0]<<" "<<hand1[0][1]<<"|"<<hand1[1][0]<<" "<<hand1[1][1]<<"|	";
		// cout<<"|"<<hand2[0][0]<<" "<<hand2[0][1]<<"|"<<hand2[1][0]<<" "<<hand2[1][1]<<"|\n";
		// cout<<"|"<<flop[0][0]<<" "<<flop[0][1]<<"|"<<flop[1][0]<<" "<<flop[1][1]<<"|"<<flop[2][0]<<" "<<flop[2][1]<<"|"<<turnc[0][0]<<" "<<turnc[0][1]<<"|"<<river[0][0]<<" "<<river[0][1]<<"|\n";

		int small=50,big=100;
		int startingStack=20000;
		int potSize=0,stack1=startingStack,stack2=startingStack,bet1=0,bet2=0,stage=0,turn=1,actions=0;

		cout<<"Hand #"<<i+1<<endl;

		if(i%2==0) {
			turn=1;
			stack1-=small;
			stack2-=big;
			bet1=50;
			bet2=100;

			cout<<engineName1<<" is dealt ["<<intToCharValue(hand1[0][0])<<intToCharSuit(hand1[0][1])<<"]["<<intToCharValue(hand1[1][0])<<intToCharSuit(hand1[1][1])<<"] in the small blind";
			cout<<endl;
			cout<<engineName2<<" is dealt ["<<intToCharValue(hand2[0][0])<<intToCharSuit(hand2[0][1])<<"]["<<intToCharValue(hand2[1][0])<<intToCharSuit(hand2[1][1])<<"] in the big blind";
			cout<<endl;
		} else {
			turn=2;
			stack2-=small;
			stack1-=big;
			bet2=50;
			bet1=100;

			cout<<engineName2<<" is dealt ["<<intToCharValue(hand2[0][0])<<intToCharSuit(hand2[0][1])<<"]["<<intToCharValue(hand2[1][0])<<intToCharSuit(hand2[1][1])<<"] in the small blind";
			cout<<endl;
			cout<<engineName1<<" is dealt ["<<intToCharValue(hand1[0][0])<<intToCharSuit(hand1[0][1])<<"]["<<intToCharValue(hand1[1][0])<<intToCharSuit(hand1[1][1])<<"] in the big blind";
			cout<<endl;
		}

		bool handInProgress = true;
		string prevAction = "none";
		while(handInProgress) {

			/*
			info.txt output format:
			
			1						hand number (use to tell when new hand is started)
			call 					previous action (check/call/raise/none)
			14 2 13 2				hand 
			19950 19900				stack sizes (yours then opponents)
			0						pot size (pot not including what the bet sizes are)
			50 100					bet sizes (what you have bet in front of you)
			14 1 12 4 5 2 2 1		board ( value suit value suit etc )

			*/
			//cout<<turn<<endl;
			if(turn==1) {

				//output info to player1
				ofstream info;
				info.open("info.txt");
				info<<i+1<<endl;
				info<<prevAction<<endl;
				info<<hand1[0][0]<<" "<<hand1[0][1]<<" "<<hand1[1][0]<<" "<<hand1[1][1]<<endl;
				info<<stack1<<" "<<stack2<<endl;
				info<<potSize<<endl;
				info<<bet1<<" "<<bet2;
				if(stage>=1) {
					info<<endl<<flop[0][0]<<" "<<flop[0][1]<<" "<<flop[1][0]<<" "<<flop[1][1]<<" "<<flop[2][0]<<" "<<flop[2][1];
				}
				if(stage>=2) {
					info<<" "<<turnc[0][0]<<" "<<turnc[0][1];
				}
				if(stage>=3) {
					info<<" "<<river[0][0]<<" "<<river[0][1];
				}
				info.close();
				// cout<<hand1[0][0]<<" "<<hand1[0][1]<<" "<<hand1[1][0]<<" "<<hand1[1][1]<<endl;
				// cout<<stack1<<" "<<stack2<<endl;
				// cout<<potSize<<endl;
				// cout<<bet1<<" "<<bet2<<endl;

				//create control file 1 to let player1 know its his turn
				ofstream ctrl1;
				ctrl1.open("ctrl1");
				ctrl1.close();

				//wait until file named "ctrl" is created by player1 to know its time to continue
				bool exists = false;
				while(!exists) {
					ifstream ctrl("ctrl");
					if(ctrl.good()) {
						exists=true;
						remove("ctrl");
					}
					time_t start = time(0);
					time_t end = time(0);
					//could definitely adjust this clock, 1 second is pretty slow
					while(difftime(end,start)<1) {
						end = time(0);
					}
				}

				//take info written play player1 to info.txt as input
				//format is just 
				//<command> <raise amt if applicable>
				ifstream action("info.txt");
				string command;
				action>>command;
				if(command=="check") {
					prevAction="check";
					cout<<engineName1<<" checks."<<endl;
				} else if (command=="call") {
					stack1 = stack1 - (bet2-bet1);
					bet1 = bet2;
					prevAction="call";
					cout<<engineName1<<" calls."<<endl;
				} else if (command=="raise") {
					int size;
					action>>size;
					stack1 = stack1 - (size-bet1);
					bet1 = size;
					prevAction="raise";
					cout<<engineName1<<" raises to "<<size<<endl;
				} else if (command=="fold") {
					stack2 += potSize + bet1 + bet2;
					potSize=0;
					handInProgress = false;
					cout<<engineName1<<" folds."<<endl;
					cout<<engineName2<<" wins the pot."<<endl;
				}
				action.close();
				actions++;

				//neat math that toggles turn
				//could have just said turn=2 lol
				turn = -turn+3;

				//if stage of hand is done do stuff
				if(actions>=2 && bet1==bet2) {
					potSize = potSize + bet1 + bet2;
					bet1=0;
					bet2=0;
					stage++;
					actions=0;
					turn=-1*(i%2)+2;
					if(i%2==1 && stage==1) {
						prevAction="none";
					}
					cout<<engineName1<<" vs "<<engineName2<<endl;
					cout<<stack1<<"    "<<stack2<<endl;
					cout<<"["<<intToCharValue(hand1[0][0])<<intToCharSuit(hand1[0][1])<<"]["<<intToCharValue(hand1[1][0])<<intToCharSuit(hand1[1][1])<<"]";
					cout<<" vs ";
					cout<<"["<<intToCharValue(hand2[0][0])<<intToCharSuit(hand2[0][1])<<"]["<<intToCharValue(hand2[1][0])<<intToCharSuit(hand2[1][1])<<"]";
					cout<<endl;
					if(stage>=1) {
						cout<<"["<<intToCharValue(flop[0][0])<<intToCharSuit(flop[0][1])<<"]["<<intToCharValue(flop[1][0])<<intToCharSuit(flop[1][1])<<"]["<<intToCharValue(flop[2][0])<<intToCharSuit(flop[2][1])<<"]";
					}
					if(stage>=2) {
						cout<<"["<<intToCharValue(turnc[0][0])<<intToCharSuit(turnc[0][1])<<"]";
					}
					if(stage>=3) {
						cout<<"["<<intToCharValue(river[0][0])<<intToCharSuit(river[0][1])<<"]";
					}
					cout<<endl;
					cout<<"Pot: "<<potSize<<endl;
					cout<<"Bets: "<<bet1<<" "<<bet2<<endl;
				}
				//check if all in or at showdown then do some stuff
				if((stack1==0 && stack2==0) || stage>=4) {
					int datad[5][2]={0};
					int datad2[5][2]={0};
					double str1 = strengthOfBestHand(cards1,datad,7,5,0,0);
					double str2 = strengthOfBestHand(cards2,datad2,7,5,0,0);
					//cout<<str1<<endl<<str2<<endl;
					if(str1 > str2) {
						stack1+=potSize;
						cout<<engineName1<<" wins the pot."<<endl;
					} else if (str1 < str2) {
						stack2+=potSize;
						cout<<engineName2<<" wins the pot."<<endl;
					} else {
						stack1+=potSize/2;
						stack2+=potSize/2;
						cout<<engineName1<<" and "<<engineName2<<" split the pot."<<endl;
					}
					handInProgress=false;
				}

			} else if (turn==2) {
				//oh wow look all the same stuff as above just copy pasted and changed for player2 pretty much
				ofstream info;
				info.open("info.txt");
				info<<i+1<<endl;
				info<<prevAction<<endl;
				info<<hand2[0][0]<<" "<<hand2[0][1]<<" "<<hand2[1][0]<<" "<<hand2[1][1]<<endl;
				info<<stack2<<" "<<stack1<<endl;
				info<<potSize<<endl;
				info<<bet2<<" "<<bet1;
				if(stage>=1) {
					info<<endl<<flop[0][0]<<" "<<flop[0][1]<<" "<<flop[1][0]<<" "<<flop[1][1]<<" "<<flop[2][0]<<" "<<flop[2][1];
				}
				if(stage>=2) {
					info<<" "<<turnc[0][0]<<" "<<turnc[0][1];
				}
				if(stage>=3) {
					info<<" "<<river[0][0]<<" "<<river[0][1];
				}
				info.close();
				// cout<<hand2[0][0]<<" "<<hand2[0][1]<<" "<<hand2[1][0]<<" "<<hand2[1][1]<<endl;
				// cout<<stack2<<" "<<stack1<<endl;
				// cout<<potSize<<endl;
				// cout<<bet2<<" "<<bet1<<endl;

				ofstream ctrl2;
				ctrl2.open("ctrl2");
				ctrl2.close();
				// cout<<"ctrl2 created\n";

				bool exists = false;
				while(!exists) {
					ifstream ctrl("ctrl");
					if(ctrl.good()) {
						exists=true;
						remove("ctrl");
					}
					time_t start = time(0);
					time_t end = time(0);
					while(difftime(end,start)<1) {
						end = time(0);
					}
				}

				ifstream action("info.txt");
				string command;
				action>>command;
				if(command=="check") {
					prevAction="check";
					cout<<engineName2<<" checks."<<endl;
				} else if (command=="call") {
					stack2 = stack2 - (bet1-bet2);
					bet2 = bet1;
					prevAction="call";
					cout<<engineName2<<" calls."<<endl;
				} else if (command=="raise") {
					int size;
					action>>size;
					stack2 = stack2 - (size-bet2);
					bet2 = size;
					prevAction="raise";
					cout<<engineName2<<" raises to "<<size<<endl;
				} else if (command=="fold") {
					stack1 += potSize + bet1 + bet2;
					handInProgress = false;
					cout<<engineName2<<" folds."<<endl;
					cout<<engineName1<<" wins the pot."<<endl;
				}
				action.close();
				actions++;

				turn = -turn+3;

				if(actions>=2 && bet1==bet2) {
					potSize = potSize + bet1 + bet2;
					bet1=0;
					bet2=0;
					stage++;
					actions=0;
					turn=-1*(i%2)+2;
					if(i%2==0 && stage==1) {
						prevAction="none";
					}
					cout<<engineName1<<" vs "<<engineName2<<endl;
					cout<<stack1<<"    "<<stack2<<endl;
					cout<<"["<<intToCharValue(hand1[0][0])<<intToCharSuit(hand1[0][1])<<"]["<<intToCharValue(hand1[1][0])<<intToCharSuit(hand1[1][1])<<"]";
					cout<<" vs ";
					cout<<"["<<intToCharValue(hand2[0][0])<<intToCharSuit(hand2[0][1])<<"]["<<intToCharValue(hand2[1][0])<<intToCharSuit(hand2[1][1])<<"]";
					cout<<endl;
					if(stage>=1) {
						cout<<"["<<intToCharValue(flop[0][0])<<intToCharSuit(flop[0][1])<<"]["<<intToCharValue(flop[1][0])<<intToCharSuit(flop[1][1])<<"]["<<intToCharValue(flop[2][0])<<intToCharSuit(flop[2][1])<<"]";
					}
					if(stage>=2) {
						cout<<"["<<intToCharValue(turnc[0][0])<<intToCharSuit(turnc[0][1])<<"]";
					}
					if(stage>=3) {
						cout<<"["<<intToCharValue(river[0][0])<<intToCharSuit(river[0][1])<<"]";
					}
					cout<<endl;
					cout<<"Pot: "<<potSize<<endl;
					cout<<"Bets: "<<bet1<<" "<<bet2<<endl;
				}
				if((stack1==0 && stack2==0) || stage>=4) {
					int datad[5][2]={0};
					int datad2[5][2]={0};
					double str1 = strengthOfBestHand(cards1,datad,7,5,0,0);
					double str2 = strengthOfBestHand(cards2,datad2,7,5,0,0);
					//cout<<str1<<endl<<str2<<endl;
					if(str1 > str2) {
						stack1+=potSize;
						cout<<engineName1<<" wins the pot."<<endl;
					} else if (str1 < str2) {
						stack2+=potSize;
						cout<<engineName2<<" wins the pot."<<endl;
					} else {
						stack1+=potSize/2;
						stack2+=potSize/2;
						cout<<engineName1<<" and "<<engineName2<<" split the pot."<<endl;
					}
					handInProgress=false;
				}

			}

		}

		//calculate nets
		net1 = net1 + (stack1-startingStack);
		net2 = net2 + (stack2-startingStack);
		data1[i]=net1;
		data2[i]=net2;
		//log data after each hand in case of crash
		ofstream data;
		data.open("data.txt");
		data<<engineName1<<" "<<engineName2<<endl;
		for(int j=0;j<i+1;j++) {
			data<<data1[j]<<" "<<data2[j]<<endl;
		}
		data.close();
		// cout<<"Hand done."<<endl;

	}

	cout<<"All done."<<endl;
	ofstream data;
	data.open("data.txt");
	data<<engineName1<<" "<<engineName2<<endl;
	for(int i=0;i<numHands;i++) {
		data<<data1[i]<<" "<<data2[i]<<endl;
	}
	data.close();

}

char intToCharValue(int value) {
	char result;
	switch(value) {
		case 10:
			result = 'T';
			break;
		case 11:
			result = 'J';
			break;
		case 12:
			result = 'Q';
			break;
		case 13:
			result = 'K';
			break;
		case 14:
			result = 'A';
			break;
		default:
			result = value + '0';
			break;
	}
	return result;
}

char intToCharSuit(int suit) {
	char result;
	switch(suit) {
		case 1:
			result = 'c';
			break;
		case 2:
			result = 'd';
			break;
		case 3:
			result = 'h';
			break;
		case 4:
			result = 's';
			break;
	}
	return result;
}

// recursively find strength of all combinations of 5 card hands from given 7 cards and return strength of best hand
double strengthOfBestHand(int cards[7][2],int data[5][2],int n,int r,int s,int d) {
	double max = 0;

	if(r==0) {
		max = strengthOfHand(data);
	} else {
		for(int i=s;i<s+n-r+1;i++) {
			data[d][0] = cards[i][0];
			data[d][1] = cards[i][1];
			double str = strengthOfBestHand(cards,data,n-(i-s)-1,r-1,i+1,d+1);
			if(str>max) {
				max=str;
			}
		}
	}

	return max;

}

// does what it says - no long longs anymore :(
double strengthOfHand(int cards1[5][2]) {

	int cards[5][2];
	for(int i=0;i<5;i++) {
		cards[i][0]=cards1[i][0];
		cards[i][1]=cards1[i][1];
	}

	//sort
	int key1,key2,j;
	for(int i=1;i<5;i++) {
		key1=cards[i][0];
		key2=cards[i][1];
		j=i-1;
		while(j>=0 && cards[j][0]<key1) {
			cards[j+1][0]=cards[j][0];
			cards[j+1][1]=cards[j][1];
			j--;
		}
		cards[j+1][0]=key1;
		cards[j+1][1]=key2;
	}

	//Four of a Kind
	if (cards[0][0]==cards[1][0] && cards[0][0]==cards[2][0] && cards[0][0]==cards[3][0]) {
		return 8000 + cards[4][0];
	} else if (cards[4][0]==cards[1][0] && cards[4][0]==cards[2][0] && cards[4][0]==cards[3][0]) {
		return 8000 + cards[0][0];
	}

	//Full House
	if (cards[0][0]==cards[1][0] && cards[0][0]==cards[2][0] && cards[3][0]==cards[4][0]) {
		return 7000 + cards[0][0] * 10 + cards[4][0] * .1;
	} else if (cards[0][0]==cards[1][0] && cards[2][0]==cards[3][0] && cards[2][0]==cards[4][0]) {
		return 7000 + cards[4][0] * 10 + cards[0][0] * .1;
	}

	bool flush=false;
	//Flush
	if (cards[0][1]==cards[1][1] && cards[0][1]==cards[2][1] && cards[0][1]==cards[3][1] && cards[0][1]==cards[4][1]) {
		flush=true;
	}

	//Straight and Straight Flush
	if (cards[0][0]-cards[4][0]==4 && cards[0][0]!=cards[1][0] && cards[1][0]!=cards[2][0] && cards[2][0]!=cards[3][0] && cards[3][0]!=cards[4][0]) {
		if(flush) {
			return 9000 + cards[0][0];
		} else {
			return 5000 + cards[0][0];
		}
	} else if (cards[0][0]==14 && cards[1][0]==5 && cards[2][0]==3 && cards[3][0]==3 && cards[4][0]==2) {
		if(flush) {
			return 9000 + cards[1][0];
		} else {
			return 5000 + cards[1][0];
		}
	} else if (flush) {
		return 6000 + cards[0][0] * 10 + cards[1][0] * .1 + cards[2][0] * .001 + cards[3][0] * .00001 + cards[4][0] * .0000001;
	}

	//Three of a Kind
	if (cards[0][0]==cards[1][0] && cards[0][0]==cards[2][0]) {
		return 4000 + cards[0][0]*10 + cards[3][0]*.1 + cards[4][0]*.001;
	} else if (cards[1][0]==cards[2][0] && cards[1][0]==cards[3][0]) {
		return 4000 + cards[1][0]*10 + cards[0][0]*.1 + cards[4][0]*.001;
	} else if (cards[2][0]==cards[3][0] && cards[2][0]==cards[4][0]) {
		return 4000 + cards[2][0]*10 + cards[0][0]*.1 + cards[1][0]*.001;
	}

	//Two Pair
	if (cards[0][0]==cards[1][0] && cards[2][0]==cards[3][0]) {
		return 3000 + cards[0][0]*10 + cards[2][0]*.1 + cards[4][0]*.001;
	} else if (cards[0][0]==cards[1][0] && cards[3][0]==cards[4][0]) {
		return 3000 + cards[0][0]*10 + cards[3][0]*.1 + cards[2][0]*.001;
	} else if (cards[1][0]==cards[2][0] && cards[3][0]==cards[4][0]) {
		return 3000 + cards[1][0]*10 + cards[3][0]*.1 + cards[0][0]*.001;
	}

	//Pair
	if (cards[0][0]==cards[1][0]) {
		return 2000 + cards[0][0]*10 + cards[2][0]*.1 + cards[3][0]*.001 + cards[4][0]*.00001;
	} else if (cards[1][0]==cards[2][0]) {
		return 2000 + cards[1][0]*10 + cards[0][0]*.1 + cards[3][0]*.001 + cards[4][0]*.00001;
	} else if (cards[2][0]==cards[3][0]) {
		return 2000 + cards[2][0]*10 + cards[0][0]*.1 + cards[1][0]*.001 + cards[4][0]*.00001;
	} else if (cards[3][0]==cards[4][0]) {
		return 2000 + cards[3][0]*10 + cards[0][0]*.1 + cards[1][0]*.001 + cards[2][0]*.00001;
	}

	//High Card
	return 1000 + cards[0][0]*10 + cards[1][0]*.1 + cards[2][0]*.001 + cards[3][0]*.00001 + cards[4][0]*.0000001;
}










