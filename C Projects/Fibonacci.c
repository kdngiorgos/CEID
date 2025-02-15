#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int getInt(char message[]);
int numofterms;

int main(int argc, char *argv[]) {
	int i;
	printf("FibonacciNumbers V2 \n\n\n");
	numofterms = getInt("Dose ari8mo orwn:");
	int fbn[numofterms];
	fbn[0] = getInt("Dose 1o ari8mo");
	fbn[1] = getInt("Dose 2o ari8mo");
	for(i=2;i<numofterms;i++){
		fbn[i] = fbn[i-1] + fbn[i-2];
	}
	for(i=0;i<numofterms;i++)
	printf("term-%d:%d\n",i+1,fbn[i]);
	return 0;
}
int getInt(char message[]){
	int num;
	printf(message);
	scanf("%d",&num);
	return num;
}