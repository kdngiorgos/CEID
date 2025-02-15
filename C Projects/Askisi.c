#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(){
	float number = 0.123456789;
	printf("%.3f",number);
	return 0;
}
///* Askhsh 1 */
//
///* Lektikh Perigrafi
//
//	Pare to pli8os twn mpoukaliwn gia tin paraggelia
//	Upologise thn timh kai elegse an dexetai akptwsh
//	Ektipwse tin telikh timh
//
//*/
//
//int getInt(char message[]);
//void DiscountCalc(int quantity,float price);
//
//int big;
//int small;
//float price;
//
//int main(int argc, char *argv[]){
//	big = getInt("Dose ari8mo megalwn mpoukaliwn: ");
//	small = getInt("\nDose ari8mo mikrwn mpoukaliwn: ");
//	price = (float)big*0.2 + (float)small*0.08; 
//	DiscountCalc(big+small,price);
//	return 0;
//}
//
//void DiscountCalc(int quantity,float price){
//	if(price>600) {
//		printf("\nEpeidh h timh tis paraggelias ksepernaei ta 600$ sunolika exeis ekptwsh 24% \n ");
//		printf("To arxiko poso itan %f$ kai meta tin ekptwsh einai %f$",price,price*0.76);
//	}
//	else if(price>300||quantity>=1000){
//		if(price>300){ 
//			printf("\nEpeidh h timh tis paraggelias ksepernaei ta 300$ sunolika exeis ekptwsh 12% ");
//			printf("\nTo arxiko poso itan %f$ kai meta tin ekptwsh einai %f$",price,price*0.88);
//		}
//		else{
//			printf("\nEpeidh exeis parei %d, panw apo 1000, mpoukalia exeis ekptwsh 12% ",quantity);
//			printf("\nTo arxiko poso itan %f$ kai meta tin ekptwsh einai %f$",price,price*0.88);
//		} 
//	}
//}
//
//int getInt(char message[]){
//	int num;
//	printf(message);
//	scanf("%d",&num);
//	return num;
//}
//
///* Askhsh 2*/
///* Lektikh Perigrafi
//	Pare ton prwto tripsifio ari8mo
//	An to a8roisma twn kubwn twn psifiwn tou einai iso me ton ari8mo
//		Ektipwse ton ari8mo
//	Epanelabe gia olous tous tripsifious ari8mous	
// */
//
//int num1,num2,num3,i;
//
//int main(int argc, char *argv[]){
//	for(i=100;i<1000;i++){
//		num1=i%10;
//		num2=(i/10)%10;
//		num3=(i/100)%10;		
//		if(pow(num1,3)+pow(num2,3)+pow(num3,3)==i) printf("O ari8mos %d ikanopoiei ta kritiria\n",i);
//	}
//}