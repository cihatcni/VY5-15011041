/**
@file
Data Structures and Algorithms
Arithmetic Calculator
@author
Name : Cihat Bozkurt
Surname : Cüni
Student Number : 15011041
Date : 20.05.2018
E-Mail: cihatc52@gmail.com
Compiler : GCC
IDE: Geany 
Operating System : Windows 10
CPU : 2.60 Ghz
Ram : 8 GB
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
	
typedef struct {
	int top;
	char st[100]; //Stack for characters.
	int sti[100]; //Stack for integers.
	} STACK;
	
typedef struct {
	char variable;	  // Variable name.
	int value;		  // Variable value.
	char string[255]; // String field for postfix version.
	} LIST;
		
LIST list[20];	
STACK stack;

int N;				// Number of equations.
int isEmpty();		//Is stack empty control.
int isFull();		//Is stack full control.
void push(char);	//Adds a new char on top of the stack.
void pushInt(int);	//Adds a new integer on top of the stack.
char pop();			//Returns the char at the top of the stack.
int popInt();		//Returns the integer at the top of the stack.
void printList();	//It prints the list.
void infixToPostfix(char[],int); //Converts the infix string to the postfix string.
void postFixtoInt(int);	//It does mathematical operations and generates final int value.
void printCharStack();	//Prints the char stack.
void printIntStack();	//Prints the integer stack.

int main() {
	int flag=0,j,i=0;
	char string[255];
	FILE *file;
	
	file = fopen("input.txt","r+");
	if(!file) {
		printf("ERROR. There is not input file.\n");
		getch();
		exit(0);
	}
	
	printf("ARITHMETIC CALCULATOR\n");
	while(!feof(file)) {
		fgets(string, 255, file);
		strcpy(string,strtok(string,";"));	//Deleting ';' from string.
		printf("STRING : %s\n",string);
		flag=0;	
		j=0;
		
		while(j<N && flag==0) { // The variable is checked to see if it is already added to the list.
			if(list[j].variable==string[0]) { 
				for(int k = 0 ; k<50 ; k++) 
					list[j].string[k]='\0';
				infixToPostfix(string,j);
				postFixtoInt(j);
				flag=1;
			}
			j++;
		}
		
		if(flag==0) { //If the variable is not added to the list.
			infixToPostfix(string,i);
			postFixtoInt(i);
			i++;
		}
		
		N++;
		printf("Press a key.\n\n");
		getch();
	}
	
	printList();
	fclose(file);
	getch();

	return 0;
}

void push (char ch) { //Adds a new char on top of the stack.
	if(!isFull()) {
		stack.st[stack.top]=ch;
		stack.top++;
	}
	else
		printf("STACK FULL!\n");
}

void pushInt (int n) { //Adds a new integer on top of the stack.
	if(!isFull()) {
		stack.sti[stack.top]=n;
		stack.top++;
	}
	else
		printf("STACK FULL!\n");	
}
	
int isEmpty() {	//Is stack empty control.
	if(stack.top==0)
		return 1;
	return 0;
}
	
int isFull() {	//Is stack full control.
	if(stack.top==100)
		return 1;
	return 0;
}

char pop () { //Returns the char at the top of the stack.
	if(!isEmpty()) {
		stack.top--;
		return stack.st[stack.top];	
	}	
	return '\0';
}

int popInt () { //Returns the integer at the top of the stack.	
	if(!isEmpty()) {
		stack.top--;
		return stack.sti[stack.top];
	}	
	return 0;
}
	
void printList() { //It prints the list.
	int i=0;
	printf("RESULTS\n");
	printf("------------------------------\n");
	while(list[i].value) {
		printf("%c = %d\n",list[i].variable,list[i].value);
		i++;
	}
}	

//Converts the infix string to the postfix string.		
void infixToPostfix(char string[],int i) {
	char *pch;
	char tmp;
	int j=0;
	stack.top=0;
	
	pch=strtok(string," "); //Variable name
	list[i].variable=pch[0];
	pch=strtok(NULL," "); // Equal mark
	printf("Postfix : %s ||\t",list[i].string);
	printCharStack();
	
	pch=strtok(NULL," "); 
	while(pch!=NULL) {
		if(pch[0]!='(' && pch[0]!=')' && pch[0]!='+' && pch[0]!='-' && pch[0]!='*' && pch[0]!='/') { //If it is number or character.
			list[i].string[j++]=' ';
			strcat(list[i].string,pch);														
			while(list[i].string[j])															
				j++;
			printf("Postfix : %s ||\t",list[i].string);
			printCharStack();
		}
		
		else if(pch[0]=='(' || pch[0]=='/' ) { 
			push(pch[0]);
		}		
		else if(pch[0]=='*') { 
			tmp = pop();
			while(tmp=='/') { //If there is a higher priority process mark, it is removed from the stack.
				list[i].string[j]=' ';
				list[i].string[j+1]=tmp;
				tmp=pop();	
				j+=2;
				printf("Postfix : %s ||\t",list[i].string);
				printCharStack();
			}
			push(tmp);
			push(pch[0]);
			printf("Postfix : %s ||\t",list[i].string);
			printCharStack();
		}
		
		else if(pch[0]==')') { 
			tmp = pop();
			while(tmp!='(') {
				list[i].string[j]=' ';
				list[i].string[j+1]=tmp;
				tmp=pop();
				j+=2;
				printf("Postfix : %s ||\t",list[i].string);
				printCharStack();
			}
			printf("Postfix : %s ||\t",list[i].string);
			printCharStack();
		}
		
		else if(pch[0]=='+' || pch[0]=='-') { 
			tmp = pop();
			while(tmp=='/' || tmp=='*' || tmp=='+' || tmp=='-') { 
				list[i].string[j]=' ';
				list[i].string[j+1]=tmp;
				tmp=pop();	
				j+=2;
				printf("Postfix : %s ||\t",list[i].string);
				printCharStack();
			}
			push(tmp);
			push(pch[0]);
			printf("Postfix : %s ||\t",list[i].string);
			printCharStack();
		}
		pch=strtok(NULL," ");
	}	
	
	while(isEmpty()==0) {
		tmp = pop();
		list[i].string[j]=' ';
		list[i].string[j+1]=tmp;
		j+=2;
		printf("Postfix : %s ||\t",list[i].string);
		printCharStack();
	}
	
	printf("PostFix Final Version : %c =%s\n",list[i].variable ,list[i].string);
}

//It does mathematical operations and generates final int value.	
void postFixtoInt(int i) {
	char *pch;
	int number1;
	int number2;
	int j;
	pch=strtok(list[i].string," ");
	printIntStack();
	while(pch!=NULL) {
		if(pch[0]>47 && pch[0]<57) { //If it is a number.
			pushInt(atoi(pch));
		}
		else if(pch[0]=='+') {
			number1 = popInt();
			number2 = popInt();
			pushInt(number2+number1);
		}
		else if(pch[0]=='-') {
			number1 = popInt();
			number2 = popInt();
			pushInt(number2-number1);
		}
		else if(pch[0]=='*') {
			number1 = popInt();
			number2 = popInt();
			pushInt(number2*number1);
		}
		else if(pch[0]=='/') {
			number1 = popInt();
			number2 = popInt();
			pushInt((int) number2/number1);
		}
		else {
			j=0;
			//Finds the value of the variable in the list.
			while(j<N && list[j].variable!=pch[0]) 
							j++;
			pushInt(list[j].value);
		}
		printIntStack();
		pch=strtok(NULL," ");
	}
	
	list[i].value=popInt();
	printf("Result : %c = %d\n\n",list[i].variable,list[i].value);
}	
	
void printCharStack() { //Prints the char stack.
	printf("Stack : ");
	for(int i=0 ; i<stack.top ; i++)
			printf(" %c",stack.st[i]);
	printf("\n");
	printf("STACK BOYUT : %d\n",stack.top);
}

void printIntStack() { //Prints the integer stack.
	printf("Stack :");
	for(int i=0 ; i<stack.top ; i++)
		printf(" %d",stack.sti[i]);
	printf("\n");	
}
