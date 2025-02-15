#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int getNumber(void);
int sum(int n1, int n2);
int main(int argc, char *argv[]) {
	printf("Add2Numbers Version_3000 \n\n\n");
int	res = sum(getNumber(),getNumber());
	printf("To apotelesma einai: %d\n",res);
	system("pause");
	return 0;
}

int getNumber(void){
	int num;
	printf("Dose ari8mo:");
	scanf("%d",&num);
	return num;
}

int sum(int n1,int n2){
int	res = n1 + n2;
}