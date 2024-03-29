#include <iostream>
#include <cstdlib> 
using namespace std;

#define NO_RULES 6
#define GOAL_RULE (NO_RULES+1)
#define NO_SYMBOLS 9
#define Vt 6
#define NO_STATES 12
#define PS_SIZE 200

class Parse {
	Parse *next;
	int data;
public:
	Parse(int data) { this->data = data; next = NULL; }
	Parse* setnext(int data) { return next = new Parse(data); }
	Parse* getNext() { return next; }
	int getData() { return data; }
};

char Symbol[] = { 'a','+','*','(',')','$','E','T','F' };
int parsingTable[NO_STATES][NO_SYMBOLS + 1] = {
//	 a  +  *  (  )  $  E  T  F
	{5, 0, 0, 4, 0, 0, 1, 2, 3},
	{0, 6, 0, 0, 0,-7, 0, 0, 0},
	{0,-2, 7, 0,-2,-2, 0, 0, 0},
	{0,-4,-4, 0,-4,-4, 0, 0, 0},
	{5, 0, 0, 4, 0, 0, 8, 2, 3},
	{0,-6,-6, 0,-6,-6, 0, 0, 0},
	{5, 0, 0, 4, 0, 0, 0, 9, 3},
	{5, 0, 0, 4, 0, 0, 0, 0,10},
	{0, 6, 0, 0,11, 0, 0, 0, 0},
	{0,-1, 7, 0,-1,-1, 0, 0, 0},
	{0,-3,-3, 0,-3,-3, 0, 0, 0},
	{0,-5,-5, 0,-5,-5, 0, 0, 0}
};

int sp;
int stateStack[PS_SIZE];
int symbolStack[PS_SIZE];
int leftSymbol[NO_RULES + 1] = { 0,'E','E','T','T','F','F' }, rightLength[NO_RULES + 1] = {0,3,1,3,1,3,1};

Parse* parser() {
	int i;
	int entry, ruleNumber, lhs,tokenNum;
	int currentState;
	char token;
	Parse *head=NULL,*tmp;
	sp = 0; stateStack[sp] = 0;
	token = getchar();
	while (true) {
		currentState = stateStack[sp];
		tokenNum = -1;
		for(i=0;i<Vt;i++)
			if (token==Symbol[i]) {
				tokenNum = i; 
				break;
			}
		if (tokenNum==-1)
		{
			cout << "Error!!! : Unknown symbol" << endl;
			exit(1);
		}
		entry = parsingTable[currentState][tokenNum];
		if (entry > 0) {
			sp++;
			if (sp > PS_SIZE) {
				cout<<"Error!!! : stack overflow"<<endl;
				exit(1);
			}
			symbolStack[sp] = tokenNum;
			stateStack[sp] = entry;
			token = getchar();
		}
		else if(entry<0){
			ruleNumber = -entry;
			if (ruleNumber == GOAL_RULE) {
					cout<<"Accept!!!"<<endl;
					return head;
			}
			if (head)
				tmp = tmp->setnext(ruleNumber);
			else
				tmp = head = new Parse(ruleNumber);
			sp = sp - rightLength[ruleNumber];
			lhs = leftSymbol[ruleNumber];
			for(i=Vt;i<NO_SYMBOLS;i++)
				if (lhs == Symbol[i]) {
					lhs = i;
					break;
				}
			currentState = parsingTable[stateStack[sp]][lhs];
			sp++;
			symbolStack[sp] = lhs;
			stateStack[sp] = currentState;
		}
		else {
			cout<<"Error!!! : Wrong sentence"<<endl;
			exit(1);
		}
	}
}

int main()
{
	Parse *head,*tmp;
	cout << "Input string : ";
	head = parser();
	cout << "Right Parse : ";
	while(head)
	{
		cout << head->getData()<<" ";
		tmp = head;
		head = head->getNext();
		delete tmp;
	}
	cout << endl;
    return 0;
}

