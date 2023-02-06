#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define M 10

double sfalma = 0;
double arxeio_centers[M][2];

void file_examples();
void K_means();
double Euclidean_distance(double x1,double y1,double x2,double y2);

double S0[100][2],S1[100][2],S2[100][2],S3[100][2],S4[100][2],S5[100][2];
double centers[M][2],teams[600],plhthos[M],sum[M][2],previous_centers[M][2];
double S[600][2];

void file_examples(){
	char str[3000];
	FILE *fp;
	fp = fopen("examples2.txt", "r+");
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
	for(i=297;i<299;i++){
		printf("(%lf,%lf)\n",S[i][0],S[i][1]);
	}
}

int main(){
	srand(time(NULL)); 
	file_examples();
	int i,j;
	int thesh = 0;
	double minError = 100000;
	for(i=0;i<6;i++){
		K_means();
		if(sfalma<minError){
			minError = sfalma;
			thesh = i;
			for(j=0;j<M;j++){
				arxeio_centers[j][0] = centers[j][0];
				arxeio_centers[j][1] = centers[j][1];
			}
		}
	}
	printf("kaluterh epanalhpsh %d\n",thesh);
	FILE *fp;
	fp = fopen("K_means.txt", "w+");
	if(fp == NULL){
		printf("Error!");
        exit(1);
    }
	for(i=0;i<600;i++){
		fprintf(fp, "%f %f\n", S[i][0],S[i][1]);
	}
	fprintf(fp, "\n");
	fprintf(fp, "\n");
	for(i=0;i<M;i++){
		fprintf(fp, "%f %f\n", arxeio_centers[i][0],arxeio_centers[i][1]);
	}
	fclose(fp);
	return 0;
}

void K_means(){
	int i,j,z;
	double x;
	double min;
	int t;
	int iterations = 0;
	for(i=0;i<M;i++){ //orismos kentrwn
		int a = rand() % 600;
		centers[i][0] = S[a][0];
		centers[i][1] = S[a][1];
		sum[i][0] = 0;
		sum[i][1] = 0;
		plhthos[i] = 0;
	}
	while(1){
		for(i=0;i<600;i++){
			min = 10000;
			t = 0;
			for(z=0;z<M;z++){
				x = Euclidean_distance(S[i][0],S[i][1],centers[z][0],centers[z][1]);
				if(x<=min){
					min = x;
					t = z;
				}
			}
			teams[i] = t;
		}
		for(i=0;i<600;i++){
			for(z=0;z<M;z++){
				if(teams[i]==z){
					sum[z][0]+=S[i][0];
					sum[z][1]+=S[i][1];
					plhthos[z]++;
				}
			}
		}
		for(i=0;i<M;i++){ //orismos kentrwn
			previous_centers[i][0] = centers[i][0];
			previous_centers[i][1] = centers[i][1];
		}
		for(i=0;i<M;i++){ //orismos kentrwn
			centers[i][0] = sum[i][0]/plhthos[i];
			centers[i][1] = sum[i][1]/plhthos[i];
		}
		for(i=0;i<M;i++){ //orismos kentrwn
			plhthos[i] = 0;
			sum[i][1] = 0;
			sum[i][0] = 0;
		}
		int flag = 0;
		for(i=0;i<M;i++){ //orismos kentrwn
			if(centers[i][0]==previous_centers[i][0] && centers[i][1] == previous_centers[i][1]){
				flag++;
			}
		}
		if(flag==M){
			for(i=0;i<M;i++){
				printf("(%lf,%lf)\n",centers[i][0],centers[i][1]);
			}
			sfalma = 0;
			for(i=0;i<600;i++){
				for(z=0;z<M;z++){
					if(teams[i]==z){
						sfalma += Euclidean_distance(S[i][0],S[i][1],centers[z][0],centers[z][1]);
					}	
				}
			}
			printf("iterations needed %d\n",iterations);
			printf("sfalma was %lf\n",sfalma);
			printf("sfalma was %lf\n",sfalma/(double)600);
			break;
		}
		iterations ++;
	}
}

double Euclidean_distance(double x1,double y1,double x2,double y2){
	double distance;
	distance= sqrt(((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1)));
	return distance;
}