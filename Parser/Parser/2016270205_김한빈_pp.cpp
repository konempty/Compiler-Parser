#include <iostream>
#include <string>
#define t$ '$'

using namespace std;

int Vt[] = { 'a','b','c','d' };
#define n$ sizeof(Vt)/sizeof(int) //$�� �Ľ����̺����� �ε���
int Vn[] = { 'S','A' };
class Stack;

int is_Vt(int); //Vt�� �ε����� ã�� �Լ� ��ã���� -1����
int is_Vn(int); //Vn�� �ε����� ã�� �Լ� ��ã���� -1����
void add_rule(int); //������Ģ�� �߰�
void error(); //������ �߻�������

class Symbol { //������Ģ��ȣ�� �캯�� ����ϴ� Ŭ����
public:
	Symbol(int num = 0, string right = "") { this->num = num, this->right = right; }
	int num;
	string right;
};

class Stack_Node { //stack���� ����ϴ� ���
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

class Parse{ //������Ģ�� linked list�� �����ϴ� Ŭ����
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

class Stack { //����
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
	Symbol parsing_table[][n$+1] = { {Symbol(1,"aS"),Symbol(2,"bA")},{ Symbol() ,Symbol() ,Symbol(4,"ccA"),Symbol(3,"d") } }; //�Ľ����̺�(����� ���� Vn,Vt�� ���븸 �ٲٸ� ���α׷� ��Ȱ�밡��)
	stack = new Stack();
	cout << "Enter a sentence : ";
	getline(cin, input);
	while((stack_top=stack->Seek())!=t$){
		if (~is_Vt(stack_top)) { //������ ž�� Vt����Ȯ��
			if (stack_top == input[head])//��ǲ�� ž�� ������ ž�� �������
			{
				head++;
				stack->pop();
			}
			else //�ٸ����
				error();
		}
		else {//������ ž�� Vn�ϰ��
			if (~(input_top = is_Vt(input[head]))) { //��ǲ�� ž�� Vn���� Ȯ��
				stack->pop();
				stack->push(parsing_table[stack_top][input_top]);
			}
			else { //��ǲ�� ž�� $�ΰ��
				stack->pop();
				stack->push(parsing_table[stack_top][n$]);
			}
		}
	}
	if (stack->Seek() == input[head]) { //������ ž�� ��ǲ�� ž�� ��� $�ΰ��
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