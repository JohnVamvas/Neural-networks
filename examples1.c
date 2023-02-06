#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

double examples[3000][2],check[3000][2];
double S0[100][2],S1[100][2],S2[100][2],S3[100][2],S4[100][2],S5[100][2];
int C[3000][3],C1[3000][3]; //einai me thn skepsh oti p.x. to (1,0,0) anoikei sthn kathgoria 1 klp

void problem1();
void problem2();
double randomNums(void);
double sq(double x);

void problem1(){
	int i,j,c1,c2,c3,s3;
	double Axis,x,p;
	for(i=0;i<3000;i++){
		for(j=0;j<2;j++){		
	       		x = randomNums();
			Axis = (((x + 1 ) * ( 4 ))/ 2 ) - 2 ;
			examples[i][j] = Axis;
		}
	}
	for(i=0;i<3000;i++){
		for(j=0;j<2;j++){		
	       		x = randomNums();
			Axis = (((x + 1 ) * ( 4 ))/ 2 ) - 2 ;
			check[i][j] = Axis;
		}
	}
	for(i=0;i<3000;i++){
		if(sq(examples[i][0]-1)+sq(examples[i][1]-1)<=0.49 || sq(examples[i][0]+1)+sq(examples[i][1]+1)<=0.49){
			C[i][0] = 0;
			C[i][1] = 1;
			C[i][2] = 0;
		}else if(sq(examples[i][0]+1)+sq(examples[i][1]-1)<=0.49 || sq(examples[i][0]-1)+sq(examples[i][1]+1)<=0.49){
			C[i][0] = 0;
			C[i][1] = 0;
			C[i][2] = 1;
		}else{
			C[i][0] = 1;
			C[i][1] = 0;
			C[i][2] = 0;
		}
	}
	for(i=0;i<3000;i++){
		if(sq(check[i][0]-1)+sq(check[i][1]-1)<=0.49 || sq(check[i][0]+1)+sq(check[i][1]+1)<=0.49){
			C1[i][0] = 0;
			C1[i][1] = 1;
			C1[i][2] = 0;
		}else if(sq(check[i][0]+1)+sq(check[i][1]-1)<=0.49 || sq(check[i][0]-1)+sq(check[i][1]+1)<=0.49){
			C1[i][0] = 0;
			C1[i][1] = 0;
			C1[i][2] = 1;
		}else{
			C1[i][0] = 1;
			C1[i][1] = 0;
			C1[i][2] = 0;
		}
	}
	for(i = 0;i<3000;i++){
		if(C[i][1] == 1 || C[i][2] == 1){	
			p = (double)(rand()) /RAND_MAX; //tuxaios arithmos anamesa apo to 0 kai to 1
			if(p<=0.1){
				if(C[i][1]==1){
					C[i][1] = 0;
					C[i][0] = 1;
				}else if(C[i][2]==1){
					C[i][2] = 0;
					C[i][0] = 1;
				}
			}
		}
	}
	FILE *fp,*fc;
	fp = fopen("examples1.txt", "w+");
	fc = fopen("check1.txt", "w+");
    // exiting program 
    if (fp == NULL) {
        printf("Error!");
        exit(1);
    }
	 if (fc == NULL) {
        printf("Error!");
        exit(1);
    }
	for(i=0;i<3000;i++){
		fprintf(fp, "%f,%f\n", examples[i][0],examples[i][1]);
		fprintf(fc, "%f,%f\n", check[i][0],check[i][1]);
	}
    fclose(fp);
	fclose(fc);
	for(i=1000;i<1005;i++){
		printf("(%d,%d,%d)\n",C[i][0],C[i][1],C[i][2]);
	}
	
}

void problem2(){
	int i,j;
	double x;
	double pi = 3.1415926;
	for(i=0;i<100;i++){
		double r = 0.3*sqrtl(fabs(randomNums())); //tuxaios arithmos anamesa apo 0 kai 1
		double angle = fabs(randomNums())*2*pi;
		S0[i][0] = r*cosl(angle);
		S0[i][1] = r*sinl(angle);
	}
	//points within [-1.1,-0.5] x [0.5,1.1]
	for(i=0;i<100;i++){
		for(j=0;j<2;j++){
			x = randomNums();
			//printf("%f\n",x);
			if(j==0)
				S1[i][j] = (((x + 1)*(0.6))/2) - 1.1;
			else
				S1[i][j] = (((x + 1)*(0.6))/2) + 0.5;
		}
	}
	//points within [-1.1,-0.5] x [-1.1,-0.5]
	for(i=0;i<100;i++){
		for(j=0;j<2;j++){
			x = randomNums();
			S2[i][j] = (((x + 1)*(0.6))/2) - 1.1;
		}
	}
	//points within [0.5,1.1] x [-1.1,-0.5]
	for(i=0;i<100;i++){
		for(j=0;j<2;j++){
			x = randomNums();
			if(j==0)
				S3[i][j] = (((x + 1)*(0.6))/2) + 0.5;
			else
				S3[i][j] = (((x + 1)*(0.6))/2) - 1.1;
		}
	}
	//points within [0.5,1.1] x [0.5,1.1]
	for(i=0;i<100;i++){
		for(j=0;j<2;j++){
			x = randomNums();
			S4[i][j] = (((x + 1)*(0.6))/2) + 0.5;
		}
	}
	//points within [-1,1]x[-1,1]
	for(i=0;i<100;i++){
		for(j=0;j<2;j++)
			S5[i][j] = randomNums();
	}
	FILE *fp,*fc;
	fp = fopen("examples2.txt", "w+");
	fc = fopen("plotexamples2.txt","w+");
	if(fc==NULL){
		printf("Error!");
        exit(1);
    }
    // exiting program 
    if (fp == NULL) {
        printf("Error!");
        exit(1);
    }
	for(i=0;i<100;i++){
		fprintf(fp, "%f,%f\n", S0[i][0],S0[i][1]);
		fprintf(fc, "%f %f\n", S0[i][0],S0[i][1]);
	}
	for(i=0;i<100;i++){
		fprintf(fp, "%f,%f\n", S1[i][0],S1[i][1]);
		fprintf(fc, "%f %f\n", S1[i][0],S1[i][1]);
	}
	for(i=0;i<100;i++){
		fprintf(fp, "%f,%f\n", S2[i][0],S2[i][1]);
		fprintf(fc, "%f %f\n", S2[i][0],S2[i][1]);
	}
	for(i=0;i<100;i++){
		fprintf(fp, "%f,%f\n", S3[i][0],S3[i][1]);
		fprintf(fc, "%f %f\n", S3[i][0],S3[i][1]);
	}
	for(i=0;i<100;i++){
		fprintf(fp, "%f,%f\n", S4[i][0],S4[i][1]);
		fprintf(fc, "%f %f\n", S4[i][0],S4[i][1]);
	}
	for(i=0;i<100;i++){
		fprintf(fp, "%f,%f\n", S5[i][0],S5[i][1]);
		fprintf(fc, "%f %f\n", S5[i][0],S5[i][1]);
	}
    fclose(fp);
	fclose(fc);
}

int main(){
	srand ( time (NULL));
	problem1();
	problem2();
	return 0;
}

double randomNums(void){
    	return (double)(rand()) /RAND_MAX*2.0-1.0;//generates a num between -1 and 1;
}
double sq(double x){
	return x*x;
}
