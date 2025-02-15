#include <stdio.h>
#include <stdlib.h>

int getInt(char message[]);
int numofterms,i,k;
int count=1;
int num,prevnum,mostnum,max;

int main(int argc, char *argv[]){
	printf("MostFrequentNumber Version1\n\n\n");
	numofterms = getInt("Dose pli8os ari8mwn ");
	prevnum = getInt("Dose ari8mo");
	while(k<numofterms-1){
		printf("Dose ari8mo ");
		scanf("%d",&num);
		if(num==prevnum){
			count++;
			if(count>max)
				mostnum=num;
				max = count;}
		else{
			count = 1;
			prevnum = num;}
			k++;
	}
	printf("O ari8mos %d emfanistike poio polles fores, sunolika emfanistike %d fores",mostnum,max);
	return 0;
}

int getInt(char message[]){
	int number;
	printf(message);
	scanf("%d",&number);
	return number;
}