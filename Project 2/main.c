#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "ParseTree.h"
#include <math.h>
#pragma warning(disable:4996)
Tree E();
Tree N();
Tree D();
Tree T();
Tree F();
Tree C();
Tree M();
char *input;
int answer;
int digitNum;
int spacing;

int groupDigit(Tree head) {
	if (head.a == 'D') {
		return groupDigit(head.children[0]);
	}
	if (head.a == 'N') {
		if (head.children[0].a == '$') {
			return 0;
		}
		if (head.childSize == 1) {
			return groupDigit(head.children[0]);
		}
		int d1 = groupDigit(head.children[1]);
		int d2 = groupDigit(head.children[0]);
		int sum = d1 + d2;
		return sum;
	}
	else{
		int send = head.a - '0';
		if (digitNum == 0) {
			digitNum++;
			return send;
		}
		double t = pow(10, digitNum);
		send = send*t;
		digitNum++;
		return send;
	}
}


int eval(Tree head) {
	if (head.a == '$') {
		return 0;
	}
	else if (head.childSize == 3 && head.a == 'C') {
		int temp = answer;
		if (head.children[0].a == '+') {
			int n = eval(head.children[1]);
			answer = temp + n;
		}
		if (head.children[0].a == '-') {
			answer = temp - eval(head.children[1]);
		}
		eval(head.children[2]);
		return answer;
	}
	else if (head.childSize == 3 && head.a == 'M') {
		int temp = answer;
		if (head.children[0].a == '*') {
			int n = eval(head.children[1]);
			answer = temp * n;
		}
		if (head.children[0].a == '/') {
			answer = temp / eval(head.children[1]);
		}
		eval(head.children[2]);
		return answer;
	}
	else if (head.childSize == 3 && head.a == 'F') {
		int temp = answer;
		answer = 0;
		int n = eval(head.children[1]);
		answer = temp;
		return n;
	}
	else if (head.a == 'N' && head.childSize == 2) {
		digitNum = 0;
		int num = groupDigit(head, 0);
		if (answer == 0) {
			answer = num;
		}
		return num;
	}
	
	else {
		int num = 0;
		if (head.a == 'E') {
			num = eval(head.children[0]);
			Tree temp = head.children[1];
			if (temp.children[0].a != '$') {
				return eval(head.children[1]);
			}
		}
		else {
			num = eval(head.children[0]);
			if (head.childSize == 2) {
				eval(head.children[1]);
			}
			return num;
		}
	}
	//return answer;
}

int isNum() {
	if (*input >= '0' && *input <= '9') {
		return 1;
	}
	else {
		return 0;
	}
}

void printTree(Tree head) {

	printf("\n", "");
	for (int i = 0; i <= spacing; i++) {
		printf("%c", ' ');
	}
	printf("%c", head.a);
	if (head.children == NULL) {

	}
	else {
		for (int i = 0; i < head.childSize; i++) {
			spacing++;
			printTree(head.children[i]);
			spacing--;

		}
	}
}


Tree addChild(Tree head, Tree child) {
	if (head.children == NULL) {
		head.children = malloc(3 * sizeof(Tree));
		head.children[head.childSize] = child;
		head.childSize++;
		return head;
	}
	else {
		head.children[head.childSize] = child;
		head.childSize++;
		return head;
	}

}

Tree E() {
	Tree head = { 'E', 0, NULL };
	Tree t = T();
	Tree c = C();
	head = addChild(head, t);
	head = addChild(head, c);
	return head;
}

Tree C() {
	Tree head = { 'C', 0, NULL };
	if ((*input == '+') || (*input == '-')) {
		Tree op = { *input, 0, NULL };
		head = addChild(head, op);
		input++;
		Tree t = T();
		head = addChild(head, t);
		Tree c =  C();
		head = addChild(head, c);
	}
	else {
		Tree ep = { '$', 0, NULL };
		head = addChild(head, ep);
	}
	return head;
}

Tree T() {
	Tree head = { 'T', 0, NULL };
	Tree f = F();
	head = addChild(head, f);
	Tree m = M();
	head = addChild(head, m);
	return head;
}

Tree M() {
	Tree head = { 'M', 0, NULL };
	if ((*input == '*') || (*input == '/')) {
		Tree op = { *input, 0, NULL };
		head = addChild(head, op);
		input++;
		Tree f = F();
		head = addChild(head, f);
		Tree m = M();
		head = addChild(head, m);
	}
	else {
		Tree ep = { '$', 0, NULL };
		head = addChild(head, ep);
	}
	return head;
}

Tree F() {
	Tree head = { 'F', 0, NULL };
	if (*input >= '0' && *input <= '9') {
		Tree n = N();
		head = addChild(head, n);
		return head;
	}
	else if (*input == '(') {
		Tree t1 = { '(', 0, NULL };
		input++;
		Tree t2 = E();
		if (*input == ')') {
			Tree t3 = { ')', 0, NULL };
			head = addChild(head, t1);
			head = addChild(head, t2);
			head = addChild(head, t3);
			input++;
			return head;
		}
		else {
			printf("\n Your input is invalid.");
		}
	}
	else {
		printf("\n Your input has failed");
	}
}

Tree N() {
	Tree n = { 'N', 0, NULL };
	if (isNum() == 0) {
		Tree ep = { '$', 0, NULL };
		n = addChild(n, ep);
		return n;
	}
	Tree d = D();
	Tree n1 = N();
	n = addChild(n, d);
	n = addChild(n, n1);
	return n;
}

void Fail() {

}

Tree D() {
	if (*input >= '0' && *input <= '9') {
		Tree d = { 'D', 0, NULL };
		Tree t1 = { *input, 0, NULL };
		d = addChild(d, t1);
		input++;
		return d;
	}
	else {
		printf("\n Your input has failed");
		Fail();
		return ;
	}
}

int findP(char *str) {
	int i = 0;
	for(i = 0; i < 23; i++) {
		if (str[i] == ')') {
			return 1;
		}
	}
	return 0;
}

int containsError(char *str) {
	int i = 0;
	
	while (*str != '\0') {
		printf("\n w: %c", *str);
		printf("\n next: %c", *(str + 1));
		if (i == 0) {
			if (*str >= '0' && *str <= '9') {
				
			}
			else if (*str == '(') {

			}
			else {
				return 0;
			}
		}
		else if(*str == '+' || *str == '-') {
			if ((*(str + 1) >= '0') && (*(str + 1) <= '9')) {
				printf("\n num we good.");
			}
			else if (*(str+1) == '(') {

			}
			else {
				printf("\n sweat");
				return 0;
			}
		}
		else if (*str == '*' || *str == '/') {
			if (*(str + 1) >= '0' && *(str + 1) <= '9') {

			}
			else if (*(str + 1) == '(') {

			}
			else {
				return 0;
			}
		}
		else if (*str == '(') {
			if (*(str + 1) >= '0' && *(str + 1) <= '9') {

			}
			else if (findP(*str) == 0) {
				return 0;
			}
			else {
				return 0;
			}
		}
		else if (*str == ')') {
			if (*(str + 1) == '+' && *(str + 1) == '-') {
				
			}
			if (*(str+1) == '+' && *(str + 1) == '-') {

			}
			else if (*(str+1) == '\0') {

			}
		}
		else if (*str >= '0' && *str <= '9') {

		}
		else if (*str == '\0') {
			break;
		}
		str++;
		i++;
	}
	return 1;
}

int main()
{
	FILE *fp;
	printf("What is the file name:\t");
	char filename[100];
	scanf("%s", filename);
	printf("\n scans?");
	char tempinput[100];
	fp = fopen(filename, "r");
	printf("\n file opens");
	
	while (fgets(tempinput, 100, fp) != NULL) {
		input = tempinput;
		char *other = tempinput;
		if (containsError(other) == 0) {
			printf("\n This equation contains an error.");
		}
		else {
			printf("\n equation: %s", tempinput);
			Tree result = E();
			answer = 0;
			spacing = 0;
			printTree(result);
			eval(result);
			printf("\n total= %d", answer);
			printf("\n");
		}
	}
	fclose(fp);
	return 0;
}
