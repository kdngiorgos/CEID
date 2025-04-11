#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <string.h>

typedef struct order{
	int status,id; //0 = Pending  1 = Ready   2 = Closed
	char fname[10],lname[10];
	char date1[20],date2[20],date3[20];
	int sb,lb;
	float originaltotal,discount,total;
}ORDER;

void SearchOrder(void);
void CloseOrder(ORDER *ord);
void ExecuteOrder(ORDER *ord);
float CalculateTotal(ORDER *ord);
void getOrder(ORDER *ord);
void DisplayOrder(struct order ord);
int GetOption(void);

void SaveOrdersToFile(const char *filename);
void LoadOrdersFromFile(const char *filename);

int option;
ORDER ords[20]={{0,0,"giorgos","kandilas","10/10/2010 03:15","10/10/2011 03:15","10/10/11 04:00",1000,2000,0,0,0},{0,1,"nikos","kandilas","11/12/2013 03:15","10/10/2011 03:15","10/10/11 04:00",10,100,0,0,0},{0,0,"mitsos","kara8anasis","10/10/2010 03:15","10/10/2011 03:15","10/10/11 04:00",1000,1000,0,0,0},{0,1,"nikos","kandilas","11/12/2013 03:15","10/10/2011 03:15","10/10/11 04:00",3000,3000,0,0,0}};
int numoforders=4;
char namefile[10];
int file;

int main(int argc, char *argv[]){
	SetConsoleTitle("-- Orders App --");
	int ordernumber=0;
	int b;
	
	do{
		option = GetOption();
		switch(option){
		case 0: printf("Terminated :("); break;
		case 1: printf("Add an Order: \n");getOrder(&ords[numoforders]);numoforders++; break;	
		
		case 2:	printf("Which order do you want to search for?\n");
				scanf("%d",&ordernumber);DisplayOrder(ords[ordernumber]);break;
				
		case 3:	for(int y=0;y<numoforders;y++){
					if(ords[y].status==0){
						DisplayOrder(ords[y]);
						printf("\n-------------------------------\n");
					}
				}break;
				
		case 4:	printf("Saving Pending orders to file...\n");
				if(file==0){printf("Name of save file: ");
					scanf("%s",namefile);
					SaveOrdersToFile(namefile);
				break;
				}
    			SaveOrdersToFile("orders.dat");
    			break;
		
		case 5:	printf("Loading Pending orders from file...\n");
				file=1;
    			LoadOrdersFromFile("orders.dat");
    			break;
		
		case 6: printf("Which Order do you want to execute ?\n"); 
				scanf("%d",&b); if(ords[b].status==0){ExecuteOrder(&ords[b]);printf("\nOrder[%d] Executed!\n",b);} 
				else{printf("Order is not pending");}
				break;
				
		case 7: for(int y=0;y<numoforders;y++){
					if(ords[y].status==1){
						DisplayOrder(ords[y]);
						printf("\n-------------------------------\n");
					}
				}break;
				
		case 8: printf("\nWhich Order do you want to close ?\n"); 
				scanf("%d",&b);if(ords[b].status==1){CloseOrder(&ords[b]);printf("\nOrder %d Closed!\n",b);} 
				else{printf("Order is not ready\n");}
				break;
				
		case 9: for(int y=0;y<numoforders;y++){
					if(ords[y].status==2){
						DisplayOrder(ords[y]);
						printf("\n-------------------------------\n");
					}
				}break;	
				
		case 10:	printf("Saving Pending orders to file...\n");
    			SaveOrdersToFile("orders.dat");
    			break;
		
		case 11:	printf("Loading Pending orders from file...\n");
    			LoadOrdersFromFile("orders.dat");
    			break;
		case 12:SearchOrder();break;		
		default: printf("Error try again\n\n"); break;
		}
	}while(option!=0);
	return 0;
}

void SaveOrdersToFile(const char *filename){
    FILE *file = fopen(filename, "wb");
    if(!file){
        printf("\nFailed to open file for writing\n");
        return;
    }
    fwrite(&numoforders, sizeof(int), 1, file);
	fwrite(ords, sizeof(ORDER), numoforders, file);
    fclose(file);
    printf("Orders saved successfully to %s\n", filename);
}

void LoadOrdersFromFile(const char *filename){
    FILE *file = fopen(filename, "rb");
    if(!file){
        printf("Failed to open file for reading");
        return;
    }
    fread(&numoforders, sizeof(int), 1, file);
    fread(ords, sizeof(ORDER), numoforders, file);
    fclose(file);
    printf("Orders loaded successfully from %s\n", filename);
}

int GetOption(void){
	int num;
	printf("\n\nChoose:\n");
	printf(" 0 - Terminate\n");
	printf(" 1 - Add Order\n");
	printf(" 2 - Display Order\n");
	printf("---PENDING ORDERS---\n");
	printf(" 3 - Show Pending Orders\n");
	printf(" 4 - Save Pending Orders To File\n");
	printf(" 5 - Load Pending Orders From File\n");
	printf(" 6 - Execute Order - (Order must be pending)\n");
	printf("---READY ORDERS-----\n");
	printf(" 7 - Show Ready Orders\n");
	printf(" 8 - Close Order - (Order must be ready)\n");
	printf("---CLOSED ORDERS----\n");
	printf(" 9 - Show Closed Orders\n");
	printf("10 - Save Closed Orders To File\n");
	printf("11 - Load Closed Orders From File\n");
	printf("12 - Search for an Order\n\n");
	fflush(stdin);
	scanf("%d",&num);
	printf("Option -> %d\n",num);
	return num;
}

void DisplayOrder(struct order ord){
	printf("Order %d\n",ord.id);
	
	switch(ord.status){
		case 0: printf("\n PENDING \n"); break;		
		case 1: printf("\n READY \n"); break;
		case 2: printf("\n CLOSED \n"); break;}
	printf("%s\t%s\n",ord.fname,ord.lname);
	printf("Order Made - %s\n",ord.date1);
	printf("Order Should Execute - %s\n",ord.date2);
	printf("Small Bottles - %d \nLarge Bottles - %d\n\n",ord.sb,ord.lb);

	if(ord.status==2){	printf("%f-%f -> Total is $%f\n",ord.originaltotal,ord.discount,ord.total);
		printf("The order executed on - %s\n",ord.date3);}
}

void getOrder(ORDER *ord){
	ord->id = numoforders;
	printf("Give First Name\n"); 
	scanf("%s",ord->fname);
	fflush(stdin);
	printf("Give Last Name\n"); 
	scanf("%s",ord->lname);
	printf("\nGive Date of Submission (X/X/X X:X)\n"); 
	scanf("%s",ord->date1);
	fflush(stdin);
	printf("Give Date of Completion (X/X/X X:X)\n");
	scanf("%s",ord->date2);
	printf("Give Small Bottles\n");
	fflush(stdin);
	scanf("%d",&(ord->sb));
	printf("Give Large Bottles\n");
	scanf("%d",&(ord->lb));
	printf("\nWhat is the status of the order?\n0->Pending\n1-Ready\n2-Closed\n");
	scanf("%d",&(ord->status));
}

float CalculateTotal(ORDER *ord){
	ord->originaltotal = ord->sb*0.08 + ord->lb*0.35; 
	if((ord->originaltotal)>=600){
		ord->discount=0.24*(ord->originaltotal);
	}
	else if((ord->sb+ord->lb)>=1000||ord->originaltotal>300){
		ord->discount = 0.12*(ord->originaltotal);
	}
	return (ord->originaltotal)-(ord->discount);
}

void ExecuteOrder(ORDER *ord){
	ord->total = CalculateTotal(&*ord);
	ord->status = 1;
}

void CloseOrder(ORDER *ord){
	printf("In which date was the order closed?\n");
	fflush(stdin);
	scanf("%[^\n]",ord->date3);
	ord->status = 2;
}

void SearchOrder(void){
	int id,num;
	char name[15];
	printf("Choose:\n");
	printf("0 - Search by First Name\n");
	printf("1 - Search by Last Name\n");
	scanf("%d",&num);
	
	if(num==0){
		printf("Give First Name:\n");
		fflush(stdin);
		scanf("%s",name);
		for(int i=0;i<numoforders;i++){
			if(strcmp(name,ords[i].fname)==0){
				printf("\nFound in Order %d",i);
			}
		}
	}
	
	else if(num==1){
		printf("Give Last Name:\n");
		fflush(stdin);
		scanf("%s",name);
		for(int i=0;i<numoforders;i++){
			if(strcmp(name,ords[i].lname)==0){
				printf("\nFound in Order %d",i);
			}
		}
	}
	else printf("Wrong Input\n");
}
