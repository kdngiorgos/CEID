#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#define N 100
#define ADDITION 1
#define DIFFERENCE 2 
#define MULTIPLICATION 3
#define DIVISION 4
#define TERMINATION 5
#define INSERT 6

int getInt(char message[]);
int sum(int a, int b);
int diff(int a,int b);
int mult(int a,int b);
float divide(int a,int b);
int x,y,res;
float floatres;
char ans[4];
int option=7;
const char summ[]="ADD";
const char difference[]="DIF";
const char multi[]="MUL";
const char divi[]="DIV";
const char term[]="TER";
const char inst[]="INS";
int main(int argc, char *argv[]) {
	printf("Menu Version 2 \n\n");
	x=getInt("Dose ari8mo: ");
	y=getInt("Dose ari8mo: ");

	
	while(option==7){
		printf("Epelekse:\nAddition -> ADD\nDifference -> DIF\nMultiple -> MUL\nDivide -> DIV\nTerminate -> TER\nInstert new numbers -> INS\n\n\n");
		scanf("%s",ans);
		if(strcmp(ans,summ)==0) option=1;
			else if(strcmp(ans,difference)==0) option=2;
				else if(strcmp(ans,multi)==0) option=3;
					else if(strcmp(ans,divi)==0) option=4;
						else if(strcmp(ans,term)==0) option=5;
							else if(strcmp(ans,inst)==0) option=6;
								else option=6;}
	while(option!=5){
		system("cls");
		printf("Result: \n-------------------------------- \n");
		if(strcmp(ans,summ)==0) option=1;
			else if(strcmp(ans,difference)==0) option=2;
				else if(strcmp(ans,multi)==0) option=3;
					else if(strcmp(ans,divi)==0) option=4;
						else if(strcmp(ans,term)==0) option=5;
							else if(strcmp(ans,inst)==0) option=6; 
								else option=7;

	
	switch(option){
		case ADDITION: res = sum(x,y); 
				printf("Addition:\n%d + %d = %d",x,y,res); break;
		case DIFFERENCE: printf("Difference:\n%d - %d = %d \n",x,y,diff(x,y)); 
				printf("%d - %d = %d \n",y,x,diff(y,x)); break;
		case MULTIPLICATION: res = mult(x,y);
				printf("Multiplication:\n%d X %d = %d",x,y,res); break;
		case DIVISION: if(x==0||y==0){printf("Den ginetai diairesh me to 0");break;}
				else {floatres = divide(x,y); printf("Division:\n%d / %d = %f \n",x,y,floatres);
				floatres =divide(y,x); printf("%d / %d = %f \n",y,x,floatres); break;};
		case TERMINATION: return 0;
		case INSERT:	x=getInt("Dose ari8mo: ");fflush(stdin); y=getInt("Dose ari8mo: "); break;
		default: printf("Egine la8os\n");break;
		}
	
	printf(" \n\nEpelekse ksana:\nAddition -> ADD\nDifference -> DIF\nMultiple -> MUL\nDivide -> DIV\nTerminate -> TER\nInstert new numbers -> INS\n\n\n");
	scanf("%s",ans);
	}
	return 0;
}

int getInt(char message[]){
	printf(message);
	int num;
	scanf("%d",&num);
	return num;
}
int sum(int a,int b){
	return x+y;
}
int diff(int a,int b){
	return x-y;
}
int mult(int a,int b){
	return x*y;
	
}
float divide(int a,int b){
	int	d = (float)x/y;
	return d;
}