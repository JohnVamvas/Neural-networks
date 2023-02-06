#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define M 10
#define N 30

void printPlot(void);
void file_examples();
void LVQ();
void initWeights();
double euclideanDistance(double x,double y,double z,double w);

double error = 0.0;
double minERROR = 100000;
double bestWeights[M][2],arxeio_weights[M][2];
int teams[600];
double weights[M][2];
double S[600][2];
double S0[100][2],S1[100][2],S2[100][2],S3[100][2],S4[100][2],S5[100][2];

double eta = 0.1;


void file_examples(){
	char str[3000];
	FILE *fp;
	fp = fopen("examples2.txt", "r+");
    // exiting program 
    if (fp == NULL) {
        printf("Error!");
        exit(1);
    }
	int i = 0;
	int j = 0;
	int s = 0;
	char delim[] = ",";
	while (fscanf(fp, "%s", str)!=EOF){
		char *ptr = strtok(str, delim);
		while(ptr != NULL){
			if(i==0||i==1){
				printf("%s\n", ptr);
			}
			if(s==0){
				sscanf(ptr, "%lf", &S0[i][j]);
			}else if(s==1){
				sscanf(ptr, "%lf", &S1[i][j]);
			}else if(s==2){
				sscanf(ptr, "%lf", &S2[i][j]);
			}else if(s==3){
				sscanf(ptr, "%lf", &S3[i][j]);
			}else if(s==4){
				sscanf(ptr, "%lf", &S4[i][j]);
			}else if(s==5){
				sscanf(ptr, "%lf", &S5[i][j]);
			}
			j ++;
			ptr = strtok(NULL, delim);
		}
		j = 0;
		i++;
		if(i==100 && s==0){
			s++;
			i=0;
		}else if(i==100 && s==1){
			s++;
			i=0;
		}else if(i==100 && s==2){
			s++;
			i=0;
		}else if(i==100 && s==3){
			s++;
			i=0;
		}else if(i==100 && s==4){
			s++;
			i=0;
		}	
	}
    fclose(fp);
	
	i = 0;
	j = 0;
	s = 0;
	for(j=0;j<600;j++){
		if(s==0){
			S[j][0] = S0[i][0];
			S[j][1] = S0[i][1];
		}else if(s==1){
			S[j][0] = S1[i][0];
			S[j][1] = S1[i][1];
		}else if(s==2){
			S[j][0] = S2[i][0];
			S[j][1] = S2[i][1];
		}else if(s==3){
			S[j][0] = S3[i][0];
			S[j][1] = S3[i][1];
		}else if(s==4){
			S[j][0] = S4[i][0];
			S[j][1] = S4[i][1];
		}else if(s==5){
			S[j][0] = S5[i][0];
			S[j][1] = S5[i][1];
		}
		i++;
		if(i==100 && s==0){
			s++;
			i=0;
		}else if(i==100 && s==1){
			s++;
			i=0;
		}else if(i==100 && s==2){
			s++;
			i=0;
		}else if(i==100 && s==3){
			s++;
			i=0;
		}else if(i==100 && s==4){
			s++;
			i=0;
		}
	}
}

void LVQ(){
	initWeights();
	int t = 0,r = 0,winner = 0,i;
	int n;
	int reader = 0;
	double min = 50000.0;
	while(eta>0.000001){
		//for each example
		for(n=0;n<600;n++){
			//find the closest cluster
			min = 50000.0;
			for(i=0;i<M;i++){
				double x = 0.0;
				x = euclideanDistance(S[n][0],S[n][1],weights[i][0],weights[i][1]);
				if(x<=min){
					min=x;
					winner=i;
				}
			}
			//update the weight of the winner
			weights[winner][0] = weights[winner][0] + eta*(S[n][0]-weights[winner][0]);
			weights[winner][1] = weights[winner][1] + eta*(S[n][1]-weights[winner][1]);
			//classify the example corespondingly
			teams[n] = winner;
		}
		eta = 0.95*eta;
		//calculate overall error
		error = 0.0;
		for(n=0;n<600;n++){
			int x = teams[n];
			error += euclideanDistance(S[n][0],S[n][1],weights[x][0],weights[x][1]);
		}
		if(error<minERROR){
			minERROR = error;
			printf("min error inside LVQ is %lf\n",minERROR);
		}
		if(eta<0.000001){
			printf("r is %d\n",r);
		}
		r++;
	}
}

int main(){
	srand(time(NULL)); 
	file_examples();
	int multitude[6] = {3,4,5,6,7,10};
	int i,j;
	double minError = 100000;
	int thesh = 0;
	for(i=0;i<6;i++){
		LVQ();
		if(minERROR<=minError){
			minError = minERROR;
			printf("min error is %lf\n",minERROR);
			thesh = i;
			for(j=0;j<M;j++){
				arxeio_weights[j][0] = weights[j][0];
				arxeio_weights[j][1] = weights[j][1];
			}
		}
		minERROR = 100000;
		eta = 0.1;
	}
	printf("minError was %lf\n",minError);
	printf("minError was %lf\n",minError/(double)600);
	printf("iteration of min error was %d\n",thesh);
	for(j=0;j<M;j++){
		printf("(%lf,%lf)\n",arxeio_weights[j][0],arxeio_weights[j][0]);
	}
	printPlot();
	return 0;
}

void printPlot(){
	FILE *fp;
	fp = fopen("LVQ.txt", "w+");
	if(fp == NULL){
		printf("Error!");
        	exit(1);
    }
	int i;
	for(i=0;i<600;i++)
		fprintf(fp, "%f %f\n", S[i][0],S[i][1]);
	fprintf(fp, "\n\n\n");
	for(i=0;i<M;i++)
		fprintf(fp, "%f %f\n", arxeio_weights[i][0],arxeio_weights[i][1]);
	fclose(fp);
}
void initWeights(){
	int i,j,p;
	for(i=0;i<M;i++){
		p = rand() % 600;
		weights[i][0] = S[p][0];//may need to check for duplicates
		weights[i][1] = S[p][1];
	}
}
double euclideanDistance(double x1,double y1,double x2,double y2){
	
	return sqrt(((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1)));
}
