#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 30

//GetStringB2
char getString(char message[],char name[]);
char getString2(char message[],char name[],int numofchars);
char name[N];
char middlename[N];
int main(){
	printf("Askisi 2 \n\n");
    getString("Dose onoma: ",name);
	getString2("Dose epwnimo: ",middlename,N);
	printf("%s %s \n",middlename,name);
	printf("Mege8os onomatos: %d \n", strlen(name));
	printf("Mege8os epwnimou: %d \n", strlen(middlename));
	return 0;
}
char getString(char message[],char str[]){
	printf(message);
	scanf("%s",str);
	 fflush(stdin);
}
char getString2(char message[],char str[],int numofchars){
	printf(message);
	int i;
	char ch;
	for(i=0;i<numofchars&&ch!='\n';i++){
		ch = getchar();
		if(ch=='\n'){
	 		str[i]='\0'; return 0;
	 	}
	else str[i]=ch;
	}
	str[i]='\0';
	fflush(stdin);
}
