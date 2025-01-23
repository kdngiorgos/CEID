#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void swap(char a[],char b[]);
int bubbleup(char *arr[],int numofelements);
void bubblesort(char *arr[],int numofelements);
void printarray(char *name[],int numofelements);
bool isSorted(int arr[],int numofelements);



char names[5][10] = {"nikos","giorgos","mitsos","kostas","stavros"};
char *name[5];

int main(int argc, char *argv[]) {
	
	for(int i=0;i<10;i++){
		name[i] = names[i];}
	printarray(name,5);

	swap(&name[0],&name[1]);
	printarray(name,5);
	
	bubblesort(name,5);
	
	return 0;
}

void bubblesort(char *arr[],int numofelements){
	int count=1;
	for(int i=0;i<numofelements&&count!=0;i++){
		printf("Iteration %d\n",i+1);
		count = bubbleup(arr,numofelements);
		if(count!=0){
		printarray(arr,numofelements);
		printf("\n------------------\n");}
	}
}

int bubbleup(char *arr[],int numofelements){
	int count=0;
	for(int i=0;i<numofelements-1;i++){
		if(strcmp(arr[i],arr[i+1])>0){
			printarray(arr,5);
			swap(&arr[i],&arr[i+1]);
			count++;}
	}
	return count;
}

bool isSorted(int arr[],int numberofelements){
	for(int i=0;i<numberofelements;i++){
		if(arr[i]>arr[i+1]) return 0;
	}
	return 1;
	
}

void printarray(char *name[],int numofelements){
	for(int i=0;i<numofelements;i++){
		printf("%s ",name[i]);
	}
	printf("\n");
	return;}

void swap(char a[],char b[]){
	char k[10];
	strcpy(k,a);
	strcpy(a,b);
	strcpy(b,k);
}
