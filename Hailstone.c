#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int num;
int hail(int num);

int main(int argc, char *argv[]) {
	printf("Hailstone Sequence V1 \n\n");
	printf("Dose ari8mo:");
	scanf("%d",&num);
	printf("Hailstone sequence for: %d \n",num);
	printf(hail(num));
    
	return 0;
}
int hail(int num){
		if (num == 1){
		printf("1");
		return 0;
	} 
    else if(num == 0){
    	return 0;
	}
	else
	{
		while(num>1)
		{
			
		
		if(num % 2 == 0){
				num = num/2;
				printf("%d \n",num);
			}
			else {
				num = num*3 + 1;
				printf("%d \n",num);
			}
		}
	}
	
}
