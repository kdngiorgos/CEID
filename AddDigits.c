#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//int getInt(char message[]);
//int numberlen(int num);
//int carry,presum,i,sum;
//int main(int argc, char *argv[]) {
//	int number1 = getInt("Dose 1o ari8mo: ");
//	int number2 = getInt("Dose 2o ari8mo: ");
//	int len = numberlen(number1);
//	for(i=1;i<=len;i++){
//	presum =	
//	}
//	return 0;
//}
//
//int getInt(char message[]){
//	int num;
//	printf(message);
//	scanf("%d",&num);
//	return num;
//}
//
//int numberlen(int num){
//	int i=1;
//	int rest=1;
//	while(rest!=0){
//		rest = num/pow(10,i);
//		i++;}
//	return i-1;
//}


//--------------------------------------------------------------------------------------

//////Version2 Me 3 psifia
int getInt(char message[]);
int presum,sum,carry,num1,num2,i;
int main(int argc, char *argv[]) {
	int number1 = getInt("Dose 1o ari8mo: ");
	int number2 = getInt("Dose 2o ari8mo: ");
	presum = (number1%10)+(number2%10);
	if(presum<10){
	sum = presum; carry = 0;}
	else{ sum = presum%10;
		carry = 1;
	}

	presum = ((number1%100)/10)+((number2%100)/10)+carry;

	if(presum<10){
	sum = sum + 10*presum; carry = 0;}
	else{ sum = sum + 10*(presum%10);
		carry = 1;
	}

	presum = (number1/100)+(number2/100)+carry;
	sum = sum + presum*100;

	printf("\nThe result is:\n %d\n+%d\n--------\n",number1,number2);
	printf("%d",sum);
	return 0;
}

int getInt(char message[]){
	int num;
	printf(message);
	scanf("%d",&num);
	return num;
}

//---------------------------------------------------------------------------------

//Version1 Me 2 psifia
//int getInt(char message[]);
//int number1;
//int number2;
//int presum,sum,carry;
//int main(int argc, char *argv[]) {
//	number1 = getInt("Dose 1o ari8mo: ");
//	number2 = getInt("Dose 2o ari8mo: ");
//
//	carry = (number1%10)+(number2%10);
//	if(carry<10){
//	sum = carry; carry = 0;}
//	else{ sum = carry%10;
//		carry = 1;
//	}
//	presum = (number1/10)+(number2/10)+carry;
//	sum += presum*10;
//
//	printf("\nThe result is:\n %d\n+%d\n--------\n",number1,number2);
//	printf("%d",sum);
//	return 0;
//}
//
//int getInt(char message[]){
//	int num;
//	printf(message);
//	scanf("%d",&num);
//	return num;
//}