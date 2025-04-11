#include <stdio.h>
#include <stdlib.h>
#include "i2p.h"

void displayExpression(Expression exp);
Fraction expressionValue(Expression exp);
Fraction addFractions(Fraction fr1, Fraction fr2);
Fraction subFractions(Fraction fr1, Fraction fr2);
Fraction mulFractions(Fraction fr1, Fraction fr2);
Fraction divFractions(Fraction fr1, Fraction fr2);
Fraction simplifyFraction(Fraction fr);
void displayFraction(Fraction fr); 
int mkd(int *a,int *b);

Fraction value;


int main(int argc, char *argv[]) {
	printf("Fractions V2\n\n");
	int numOfExpressions;
	Expression exp;
	int i;

	numOfExpressions = getInt("Enter the number of expressions to evaluate: ");	
	
	for(i=0;i<numOfExpressions;i++){
		exp=getExpressionV2();
		system("cls");
		if(exp.op1.par==0||exp.op2.par==0||(exp.oprtr=='/'&&exp.op2.ar==0)){printf("Den ginetai klasma me paranomasti to 0");}
		else {displayExpression(exp);
			Fraction result = expressionValue(exp);
			displayFraction(result); 
			result= simplifyFraction(result);
			if(result.ar%result.par==0){
				printf("To apotelesma einai: %d",result.ar/result.par);}
			else displayFraction(result);}}
	return 0;
}


int mkd(int *a, int *b){ 
	int t,x,y;
	
	if(*a>0) x = *a; else x = -*a;
	if(*b>0) y = *b; else y = -*b;
	
	if(x<y){ t=y; y=x; x=t;}
	while (y!=0){
		t= y; 
		y= x % t;
		x= t;
		} 
	return x;
} 

void displayExpression(Expression exp){
	printf("\n%d/%d%c%d/%d = ",exp.op1.ar,exp.op1.par,exp.oprtr,exp.op2.ar,exp.op2.par);
}

Fraction expressionValue(Expression exp){
	switch(exp.oprtr){
		case '+': value = addFractions(exp.op1,exp.op2); break;
		case '-': value = subFractions(exp.op1,exp.op2); break;
		case '*': value = mulFractions(exp.op1,exp.op2); break;
		case '/': if(exp.op2.ar==0){printf("Den ginetai diairesh me miden\n"); break;}else{value = divFractions(exp.op1,exp.op2);break;}
		default: printf("La8os operator"); break;
	}
	return value;
}

Fraction addFractions(Fraction fr1, Fraction fr2){
	Fraction add;
	if(fr1.par==fr2.par){
		add.ar = fr1.ar + fr2.ar;
		add.par = fr1.par;}
	else{
		add.ar = fr1.ar*fr2.par + fr2.ar*fr1.par;
		add.par = fr1.par*fr2.par;
	}
	return add;
}

Fraction subFractions(Fraction fr1, Fraction fr2){
	Fraction sub;
	if(fr1.par==fr2.par){
		sub.ar = fr1.ar - fr2.ar;
		sub.par = fr1.par;}
	else{
		sub.ar = fr1.ar*fr2.par - fr2.ar*fr1.par;
		sub.par = fr1.par*fr2.par;
	}
	return sub;
}

Fraction mulFractions(Fraction fr1, Fraction fr2){
	Fraction mul;
	mul.ar = fr1.ar*fr2.ar;
	mul.par = fr1.par*fr2.par;
	return mul;
}

Fraction divFractions(Fraction fr1, Fraction fr2){
	Fraction d;
		d.ar = fr1.ar*fr2.par;
		d.par = fr1.par*fr2.ar;
	printf("Egrapses malakia");
	return d;
}

Fraction simplifyFraction(Fraction fr){
	int k;
	printf("Simplified...");
	k = mkd(&fr.ar,&fr.par);
	fr.ar = fr.ar/k;
	fr.par = fr.par/k;
	return fr;
}

void displayFraction(Fraction fr){
	printf(" %d/%d\n",fr.ar,fr.par);
}







