#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "i2p.h"

#define MAX 20
#define MAXLEN 20
#define TERMINATE 0 
#define ADD_WORD 1 
#define DISPLAY_WORDS 2 
#define INCR_SORT 3
#define DECR_SORT 4 
#define WORD_STATS 5 
#define CHAR_STATS 6 
#define SEARCH_WORD 7 

void getName(char message[],char str[]);
void sortIntArrayInc(int ar[],int numOfElements);
int menu(void);
void addWord(void);
void displayWords(void);
void incSort(void);
void decSort(void);
void wordStats(void);
void charStats(void);
void searchWord(void);
void sortStringArrayInc(char base[],int numOfElements, int strWidth);
int wordcount=0;
char word[MAX][MAXLEN];
int choise;
int wordcharactercount(char term[],char character);

int main(int argc, char *argv[]) {
	printf("Word Handling V1 \n");
	
	int selectedOp=menu();
	
	while(selectedOp!= TERMINATE){
		fflush(stdin);
		switch(selectedOp){
			case ADD_WORD:	addWord(); break;
			case DISPLAY_WORDS:	displayWords();break;	
			case INCR_SORT: incSort(); break;
			case DECR_SORT: decSort(); break;
			case WORD_STATS: wordStats(); break;
			case CHAR_STATS: charStats(); break;
			case SEARCH_WORD: searchWord(); break;
			default: printf("Wrong Input"); break;
		}
		selectedOp=menu();
	}
	printf("WordHandling Terminated");
	return 0;
}

int menu(void){
	
	printf("\n----------------------\n");
	printf("0 - TERMINATE\n");
	printf("1 - ADD WORD\n");
	printf("2 - DISPLAY WORDS\n");
	printf("3 - INCREMENTAL SORT\n");
	printf("4 - DECREMENTAL SORT\n");
	printf("5 - WORD STATISTICS\n");
	printf("6 - CHARACTER STATISTICS\n");
	printf("7 - SEARCH WORD\n");
	printf("\n----------------------\n");
	printf("Select Operation:");
	scanf("%d",&choise);
	return choise;
}

void addWord(void){
	printf("add word\n");
	scanf("%s",word[wordcount]);
	fflush(stdin);
	wordcount++;
}

void displayWords(void){
	printf("Display Words:");
	int i;
	for(i=0;i<wordcount;i++){
		printf("\n%d Word: %s",i+1,word[i]);
	}
}
void incSort(void){
	sortStringArrayInc(word,MAX,MAXLEN);
	int i;
	for(i=0;i<wordcount;i++){
		printf("\n%s",word[i]);
	}
}

void decSort(void){
	sortStringArrayInc(word,MAX,MAXLEN);
	int i;
	for(i=wordcount;i>=0;i--)
		printf("\n%s",word[i]);
}

void wordStats(void){
	int i;
	int min, max, sum;
	float average;
	min=max=sum=strlen(word[0]);
	for(i=1; i<wordcount; i++){
		if (strlen(word[i]) < min)
			min=strlen(word[i]);
		if (strlen(word[i]) > max)
			max=strlen(word[i]);
		sum=sum+strlen(word[i]);
}
average=(float)sum/wordcount;
printf("The lengths are:\n minimun: %d \n maximum: %d \n average: %f\n", min, max, average);
}
void charStats(void){
	int i,min,max,minword,maxword;
	int k;
	int chcount=0;
	int count;
	char ch;
	printf("Poio gramma psaxneis? ");
	scanf("%c",&ch);
	min=MAXLEN;max=0;
	for(k=0;k<wordcount;k++){
		count = wordcharactercount(word[k],ch);
			if (count < min){
				min=count;
				minword=k;}
			if (count > max){
				max=count;
				maxword=k;}
		chcount = chcount + count;}
	printf("The character appeared %d times and %f on average. Max is %d and Min is %d\n",chcount,(float)chcount/wordcount,max,min);
	printf("The word that had this character the most times is %s \t and the word with the least is %s\n",maxword,minword);
}
void searchWord(void){
	int i;
	int wordmeter=0;
	char leksi[MAXLEN];
	printf("Poia leksi psaxneis? ");
	scanf("%s",leksi);
		for(i=0;i<MAX;i++){
			if(strcmp(leksi,word[i])==0) wordmeter++;
		}
}
int wordcharactercount(char term[],char character){
int i;
int num=0;
for (i=0; i<MAXLEN; i++){
	if(character==term[i]) num++;
	return num;
}
}




