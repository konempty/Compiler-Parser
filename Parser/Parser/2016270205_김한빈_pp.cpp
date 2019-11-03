#include <iostream>
#include <string>
#define t$ '$'

using namespace std;

int Vt[] = { 'a','b','c','d' };
#define n$ sizeof(Vt)/sizeof(int) //$의 파싱테이블에서의 인덱스
int Vn[] = { 'S','A' };
class Stack;

int is_Vt(int); //Vt의 인덱스를 찾는 함수 못찾으면 -1리턴
int is_Vn(int); //Vn의 인덱스를 찾는 함수 못찾으면 -1리턴
void add_rule(int); //생성규칙을 추가
void error(); //에러가 발생했을때

class Symbol { //생성규칙번호와 우변을 기억하는 클래스
public:
	Symbol(int num = 0, string right = "") { this->num = num, this->right = right; }
	int num;
	string right;
};

class Stack_Node { //stack에서 사용하는 노드
private:
	Stack_Node * prev;
	int data;
public:
	Stack_Node(Stack_Node *prev, int data)
	{
		int index;
		this->prev = prev;
		if (~(index = is_Vn(data))) {
			this->data = index;
		}
		else {
			this->data = data;
		}
	}
	friend Stack;
};

class Parse{ //생성규칙을 linked list로 관리하는 클래스
private:
	Parse* next;
	int data;
public:
	Parse(int rule){
		this->data = rule;
		next = NULL;
	}
	void print() {
		cout << data;
		if (next)
			next->print();
		else
			cout << endl;
	}
	void next_rule(int rule) {
		if (next)
			next->next_rule(rule);
		else
			next = new Parse(rule);
	}
	~Parse() {
		if (next)
			delete next;
	}
}*parse;

class Stack { //스택
	Stack_Node *node;
public:
	Stack() {
		node = new Stack_Node(new Stack_Node(NULL,t$),'S');
	}
	int Seek() {
		return node->data;
	}
	void push(Symbol s) {
		int i;
		if (!s.num)
			error();
		
		add_rule(s.num);
		for (i = s.right.length() - 1; i >= 0; i--)
			node = new Stack_Node(node, s.right[i]);
	}
	void pop() {
		Stack_Node *tmp = node->prev;
		delete node;
		node = tmp;
	}
	~Stack(){
		Stack_Node *tmp;
		while (node) {
			tmp = node->prev;
			delete node;
			node = tmp;
		}
	}
}*stack;

int main() {
	string input;
	int head = 0,stack_top,input_top;
	Symbol parsing_table[][n$+1] = { {Symbol(1,"aS"),Symbol(2,"bA")},{ Symbol() ,Symbol() ,Symbol(4,"ccA"),Symbol(3,"d") } }; //파싱테이블(여기와 위에 Vn,Vt의 내용만 바꾸면 프로그램 재활용가능)
	stack = new Stack();
	cout << "Enter a sentence : ";
	getline(cin, input);
	while((stack_top=stack->Seek())!=t$){
		if (~is_Vt(stack_top)) { //스택의 탑이 Vt인지확인
			if (stack_top == input[head])//인풋의 탑과 스택이 탑이 같은경우
			{
				head++;
				stack->pop();
			}
			else //다른경우
				error();
		}
		else {//스택의 탑이 Vn일경우
			if (~(input_top = is_Vt(input[head]))) { //인풋의 탑이 Vn인지 확인
				stack->pop();
				stack->push(parsing_table[stack_top][input_top]);
			}
			else { //인풋의 탑이 $인경우
				stack->pop();
				stack->push(parsing_table[stack_top][n$]);
			}
		}
	}
	if (stack->Seek() == input[head]) { //스택의 탑과 인풋의 탑이 모두 $인경우
		cout << "Accept!!" << endl << "LeftParse : ";
		if (parse)
			parse->print();
	}
	else
		error();
	delete stack;
	delete parse;
	return 0;
}


int is_Vt(int s){
	int size = sizeof(Vt)/sizeof(int),i;
	for (i = 0; i < size; i++)
		if (s == Vt[i])
			return i;
	return -1;

}
int is_Vn(int s) {
	int size = sizeof(Vn) / sizeof(int), i;
	for (i = 0; i < size; i++)
		if (s == Vn[i])
			return i;
	return -1;

}


void add_rule(int rule) {
	if (parse)
		parse->next_rule(rule);
	else
		parse = new Parse(rule);
}
void error() {
	delete parse;
	delete stack;
	printf("error!!!\n");
	exit(1);
}