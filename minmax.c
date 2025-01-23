#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "i2p.h"

int getInt(char message[]);
int numofterms,max,min;
int i;
void sortIntArrayInc(int ar[],int numOfElements);

int main(int argc, char *argv[]){
	numofterms = getInt("Dose pli8os ari8mwn ");
	int number[numofterms];
	
	for(i=0;i<numofterms;i++)
		number[i] = getInt("Dose ari8mo ");
	
	sortIntArrayInc(number,numofterms);
	min = number[0]; max = number[numofterms-1];
	printf("min = %d and max = %d",min,max);
	return 0;
}
int getInt(char message[]){
	printf(message);
	int num;
	scanf("%d",&num);
	return num;
}


//V3
//int getInt(char message[]);
//int numofterms,max,min;
//int updateminmax(int number[]);
//int i;
//
//int main(int argc, char *argv[]){
//	numofterms = getInt("Dose pli8os ari8mwn ");
//	int number[numofterms];
//	for(i=0;i<numofterms;i++)
//		number[i] = getInt("Dose ari8mo ");
//		updateminmax(number);
//	printf("min = %d and max = %d",min,max);
//	return 0;
//}
//int getInt(char message[]){
//	printf(message);
//	int num;
//	scanf("%d",&num);
//	return num;
//}
//int updateminmax(int number[]){
//	max = min = number[numofterms-1];
//	for(i=numofterms-2;i>=0;i--){
//	if(number[i]>max)
//        max = number[i];
//    else if(number[i]<min)
//		min = number[i];
//	}
//}
