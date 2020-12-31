#include <stdio.h> 
#include <string.h> 
#include <math.h>

#define MAX_SIZE 100 

// STACK이라는 새로운 형 선언
typedef struct STACK {
	int data[MAX_SIZE];
	int top;
} STACK;


// STACK 초기화
void Initialize(STACK* s) {
	s->top = -1;
}

// STACK이 가득 차 있는 경우
int Is_full(STACK* s) {
	return (s->top == (MAX_SIZE - 1));
}

// STACK이 비어있는 경우
int Is_empty(STACK* s) {
	return (s->top == -1);
}

// STACK에 새로운 값 삽입
void Push(STACK* s, int data) {
	if (Is_full(s)) {
		printf("스택이 가득 차 있습니다.\n");
		return;
	}
	else {
		s->data[++(s->top)] = data;
	}
}

// STACK에 기존 데이터 삭제
int Pop(STACK* s) {
	if (Is_empty(s)) {
		printf("스택이 비어있습니다.\n");
		return -1;
	}

	return s->data[(s->top)--];
}

// STACK에서 top이 가리키는 값 반환
int Peek(STACK* s) {
	if (Is_empty(s)) {
		printf("스택이 비어있습니다.\n");
		return -1;
	}

	return s->data[s->top];
}

// 연산자 우선순위 반환
int Priority(char operation) {
	switch (operation) {
	case '^': return 0;
	case '(': case ')': return 1;
	case '+': case '-': return 2;
	case '*': case '/': case '%': return 3;
	}

	return -1;
}

// 숫자인지 판별
int IsNum(char c) {
	if (c >= 48 && c <= 57)
		return 1;
	else
		return 0;
}

// Infix expression에서 Postfix expression으로 변환
int InfixToPostfix(char* expression) {
	STACK stack;
	char operation, top_operation;
	int i, length = strlen(expression);

	Initialize(&stack); // 스택 초기화

	for (i = 0; i < length; i++) {
		operation = expression[i];

		if (operation == ' ')
			continue;

		switch (operation) {
		case '-':
			if (IsNum(expression[i + 1])) { // 음수
				printf("-");
				while (IsNum(expression[i + 1])) {
					i++;
					printf(expression[i]);
				}
			}

			else {
				while (!Is_empty(&stack) && Priority(operation) <= Priority(Peek(&stack)))
					printf("%c", Pop(&stack));;

				Push(&stack, operation);
				break;
			}

		case '+':	case '*':    case '/': case '%':
			while (!Is_empty(&stack) && Priority(operation) <= Priority(Peek(&stack)))
				printf("%c", Pop(&stack));;

			Push(&stack, operation);
			break;

		case '^':
			Push(&stack, operation);
			break;

		case '(':
			Push(&stack, operation);
			break;

		case ')':
			top_operation = Pop(&stack);

			while (top_operation != '(') {
				printf("%c", top_operation);
				top_operation = Pop(&stack);
			}
			break;

		default: // 숫자인 경우
			while (1) {
				printf("%c", operation);

				if (!(i + 1 < length && IsNum(expression[i + 1])))
					break;

				i++;
				operation = expression[i];
			}
			printf(" ", operation);
			break;
		}
	}

	while (!Is_empty(&stack)) {
		printf("%c ", Pop(&stack));
	}

	return 0;
}

// Postfix expression 계산
int Calculate(char* expression) {
	STACK stack;
	int i, length = strlen(expression);
	char operation;
	int a, b, value;
	int totalnum = 0;

	Initialize(&stack); // 스택 초기화

	for (i = 0; i < length; i++) {
		operation = expression[i];
		if (operation == ' ')
			continue;

		if (IsNum(operation)) {
			totalnum = operation - '0';
			while (IsNum(expression[i + 1])) {
				i++;
				totalnum *= 10;
				totalnum += (expression[i] - '0');
			}

			Push(&stack, totalnum); // 입력 피연산자이면  숫자로 변경
		}

		else {
			b = Pop(&stack);
			a = Pop(&stack);

			switch (operation) {
			case '+':    Push(&stack, a + b);	break;

			case '-':
				if (IsNum(expression[i + 1])) {
					i++;
					totalnum *= 10;
					totalnum += (expression[i] - '0');
					totalnum *= (-1);
				}

				Push(&stack, a - b);	break;

			case '*':    Push(&stack, a * b);   break;
			case '/':    Push(&stack, a / b);   break;
			case '%':	 Push(&stack, a % b);	break;
			case '^':	 Push(&stack, (int)pow(a, b));	break;
			}
		}
	}
	return Pop(&stack);
}

int main() {
	char string[50];
	int a = 0;
	printf("입력 내용\n");

	while (1) {
		gets(string);

		// 잘못된 입력
		if (string[0] == ')') { // 짝이 맞지 않는 괄호 
			printf("ERROR\n");
			continue;
		}

		for (int i = 0; string[i] != 0; i++) {
			if (!IsNum(string[i]) && (string[i] == string[i + 1])) { // 잘못된 연산자
				printf("ERROR\n");
				a = 2;
				break;
			}

			else if (string[i] != '-' && string[i] != '^' && string[i] != '+' && string[i] != '/' && string[i] != '*' && string[i] != '%' && string[i] != '(' && string[i] != ')' && !ISNum(string[i])) { // 정의되지 않은 기호 입력
				printf("ERROR\n");
				a = 2;
				break;
			}
		}

		if (a == 2)
			continue;

		if (strcmp(string, "q") == 0) {
			printf("(프로그램 종료)");
			break;
		}

		InfixToPostfix(string);
		printf("\n");
		printf("%d\n", Calculate(gets(string)));
	}

	return 0;
}