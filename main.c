#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define N 15
#define M 18
typedef struct date{
	int day,month,year;
}Date;

typedef struct student{
	int AM;
	char fname[N],lname[N];
	Date reg;
}Student;

void DisplayRow(Student std[][M],int seats);
void DisplayAllStudents(Student std[][M],int zones,int seats);
void AddRow(Student std[][M]);
void SearchSeat(void);
void DisplayStudent(Student st,int zone,int seat);
int Menu(void);
Student InputStudent(void);
void AddStudent(Student std[][M]);
void StudentSearch(Student std[][M],int zones,int rows);
Student classroom[4][M];

int main(int argc, char *argv[]){
	SetConsoleTitle("ClassRoomApp");
	int choise;
	classroom[0][0] = (Student){1115510, "Giorgos", "Kandilas", {1, 9, 2024}};
	classroom[2][5] = (Student){5552424, "Nikos", "Kandilas", {1, 9, 2024}};
	do{
		choise=Menu();
		switch(choise){
			case 0:printf("\nClosing...:(");break;
			case 1:AddStudent(classroom);break;
			case 2:AddRow(classroom);break;
			case 3:DisplayAllStudents(classroom,4,M);break;
			case 4:DisplayRow(classroom,18);break;
			case 5:StudentSearch(classroom,4,M);break;
			case 6:SearchSeat();break;
			default: printf("Error 1");break;	
		}
	}while(choise!=0);
	
	return 0;
}


void AddStudent(Student std[][M]){
	int i,j;
	
	printf("Give Student's Zone (1-4):"); 
	scanf("%d",&i);
	
	printf("Give Student's Seat (1-18):"); 
	scanf("%d",&j);
	if(i<=4&&i>0&&j>0&&j<=18){
		std[i][j] =InputStudent();
	}
	else printf("\n\nError 3\n");
}

void AddRow(Student std[][M]){
	int b,zone,seat,i;
	printf("How many students will you be submitting?");
	scanf("%d",&b);
	printf("Where does the first student sit? Zone: ");
	scanf("%d",&zone);
	printf("Seat: ");
	scanf("%d",&seat);
	if(zone>0&&zone<=4&&seat>0&&seat<=18&&(seat+b-1)<=18){
		for(i=0;i<b;i++){
			std[zone-1][seat+i-1]=InputStudent();
		}
	}
	else printf("Error 2\n");
}

Student InputStudent(void){
	Student std;
	fflush(stdin);
	printf("Give AM of Student: ");
	scanf("%d",&std.AM);
	printf("Give First Name");
	scanf("%s",std.fname);
	fflush(stdin);
	printf("Give Last Name");
	scanf("%s",std.lname);
	
	fflush(stdin);
	
	do{
		printf("Give Day: ");
		scanf("%d",&std.reg.day);}while(std.reg.day>31||std.reg.day<0);
		
	do{			printf("Give Month: ");
		scanf("%d",&std.reg.month);}while(std.reg.month<0||std.reg.month>12);			
	
	do{
		printf("Give Year: ");
		scanf("%d",&std.reg.year);}while(std.reg.year<1980||std.reg.year>2024);
		
	printf("\n STUDENT ADDED SUCCESSFULY\n\n");
	return std;
}


void DisplayAllStudents(Student std[][M],int zones,int seats){
	int i,j;
	for(i=0;i<zones;i++){
		for(j=0;j<seats;j++){
			if(std[i][j].AM!=0) DisplayStudent(std[i][j],i,j);
		}
	}
}

void DisplayRow(Student std[][M],int seats){
	int i,zone,count=0;
	printf("Which Row do you want to print?");
	scanf("%d",&zone);
	for(i=0;i<seats;i++){
		if(std[zone-1][i].AM!=0){
			DisplayStudent(std[zone-1][i],zone-1,i);
			count++;}
	}
	if(count==0) printf("\nNo students sit in Zone %d\n",zone);
}

void StudentSearch(Student std[][M],int zones,int rows){
	int i,j,am;
	char first[N],last[N];
	printf("Search:\n1-AM\n2-Name\n");
	scanf("%d",&i);
	switch(i){
		case 1: 
			printf("Give AM: ");
			scanf("%d",&am);
			for(i=0;i<zones;i++){
				for(j=0;j<rows;j++){
					if(std[i][j].AM==am) DisplayStudent(std[i][j],i,j);
				}
		}break;
		
		case 2:
			printf("Give first name: ");
			scanf("%s",first);
			printf("Give last name: ");
			scanf("%s",last);
			for(i=0;i<zones;i++){
				for(j=0;j<rows;j++){
					if(strcmp(std[i][j].fname,first)==0&&strcmp(std[i][j].lname,last)) DisplayStudent(std[i][j],i,j);
				}
		}break;
	}
}

void SearchSeat(void){
	int i,j;
	printf("Give zone: "); 
	scanf("%d",&i); 
	printf("Give seat: "); 
	scanf("%d",&j); 
	
	if(i>0 && i<=4 && j>0 && j<=18)	DisplayStudent(classroom[i-1][j-1],i-1,j-1);
	else	printf("Invalid zone or seat. Please try again.\n");
    
	return;
}

void DisplayStudent(Student st,int z,int s){
	printf("\nZone %d  -  Seat %d\n",z+1,s+1);
	printf("%s\t%s\n",st.lname,st.fname);
	printf("AM: %d\n",st.AM);
	printf("%d/%d/%d\n",st.reg.day,st.reg.month,st.reg.year);
	return;
}

int Menu(void){
	int m;
	printf("\n0-Terminate\n1-Add Student\n2-Add a bunch of students\n3-Show all students\n4-Show all students in a zone\n5-Search for a student's seat\n6-Show which student sits on a spesific seat\n");
	scanf("%d",&m);
	return m;
}
