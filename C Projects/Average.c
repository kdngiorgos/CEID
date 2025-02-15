#include <stdio.h>
#include <stdlib.h>
//V2
float average(int n[],int s);
int getInt(char message[]);
int numofnums,count,number;
int sum=0;
int main(int argc, char *argv[]) {
	printf("Average of Numbers \n\n\n");
	numofnums = getInt("Dose to pli8os twn ari8mwn:"); 
	int num[numofnums];
	for(count=0;count<numofnums;count++){
		printf("Dose %d ari8mo:",count+1);            
		scanf("%d",&num[count]);
	}
		printf("O mesos oros einai: %f",average(num,numofnums));     
	
	return 0;
}
int getInt(char message[]){
	printf(message);
	scanf("%d",&number);
	return number;
}
float average(int n[],int s){
	int sum=0;
	int i;
	for(i=0;i<s;i++)
		sum +=n[i];
	return (float)sum/s;
}
