#include <stdio.h>
#include <stdlib.h>

int getInt(char message[]);
int Prime(int num);
void DisplayDivisors(int num);
int number=1;

int main(int argc, char *argv[]) {
	printf("Divisors V1\n\n\n");
	while(number<=1){
	number = getInt("Dose ari8mo: ");
	if(number==2||number==1)	
		printf("O %d einai prwtos ari8mos",number);
	else if(Prime(number)==1)
		printf("O %d einai prwtos ari8mos",number);
	else	DisplayDivisors(number);
		}
	return 0;
}

int Prime(int num){
	int i,k=1;
	for(i=2;i*i<=num;i++){
		if(num%i==0) k = 0;}
	return k;
}

int getInt(char message[]){
	int num;
	printf(message);
	scanf("%d",&num);
	return num;
}

void DisplayDivisors(int num){
	int i;
	int count=0;
	printf("Oi diairetes tou %d ektos apo ton eauto tou kai to 1 einai: \n",num);
	
	for(i=2;i<num;i++){
		if(num%i==0){
			printf("%d\n",i); count++;}}
	printf("Exei %d diairetes",count);	
}