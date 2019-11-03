#include <iostream>
using namespace std;

enum token {t$='$', ta = 'a', tb, tc, td };

class Parse {//생성규칙을 linked list로 관리하는 클래스
	Parse* next;
	int rule;
public:
	Parse(int rule) {
		this->rule = rule;
		next = NULL;
	}
	void print() {
		cout << rule;
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

void PS();
void PA();
void get_nextSymbol(); //입력버퍼에서 다음 심볼 얻어오는 함수
void add_rule(int); //생성규칙을 추가
void error(); //에러가 발생했을때
char nextSymbol; //다음심볼을 기억하는 변수

int main()
{
	cout << "Enter a sentence : ";
	get_nextSymbol();
	PS();
	if (nextSymbol == t$) {
		cout << "Accept!!"<<endl<<"LeftParse : ";
		
		if (parse)
			parse->print();
	}
	else
		cout << "error!!\n";

	delete parse;
    return 0;
}
void PS() {
	switch (nextSymbol) {

	case ta:
		add_rule(1);
		get_nextSymbol();
		PS();
		break;

	case tb:
		add_rule(2);
		get_nextSymbol();
		PA();
		break;

	default:
		error();
		break;
	}
}
void PA() {
	switch (nextSymbol) {

	case tc:
		add_rule(4);
		get_nextSymbol();
		if (nextSymbol == tc) {
			get_nextSymbol();
			PA();
		}
		else {
			error();
		}
		break;

	case td:
		add_rule(3);
		get_nextSymbol();
		break;

	default:
		error();
		break;
	}
}
void get_nextSymbol() {
	nextSymbol = getchar();
}
void add_rule(int rule) {
	if (parse)
		parse->next_rule(rule);
	else
		parse = new Parse(rule);
}
void error() {
	delete parse;
	cout << "error!!\n";
	exit(1);
}