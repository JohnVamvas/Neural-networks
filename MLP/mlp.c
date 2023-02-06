#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

//learning rate
#define eta 0.5
//number of input neurons
#define d 3
//number of categories 
#define K 3
//nuber of neurons per layer
#define H1 7
#define H2 4
//activation function for second layer
#define activationH2(x,y) (x==1 ? ((exp(y) - exp(-y)) / (exp(y)+exp(-y))) : y)

int L = 1;
double activation=1;
int check = 0;
//overall error
double error = 0.0;
double Gerror = 0.0;

double output[K];
double deltaO[K];

//need to initialize at some point
double input[d];

//store results of each hidden layer
double valuesHL1[H1];
double deltaHL1[H1];
double valuesHL2[H2];
double deltaHL2[H2];

//store the weights in matrices
double weightsHL1[d][H1];
double derivWL1[d][H1];
double weightsOldHL1[d][H1];

double weightsHL2[H1][H2];
double derivWL2[H1][H2];
double weightsOldHL2[H1][H2];

double weightsO[H2][K];
double derivWO[H2][K];
double weightsOldO[H2][K];


void printPlot();
double randomNums(void);
double sq(double x);
double sigmoid(double x);
void calcError(double *x,int y);
void initializeWeights();
void feedForward(double *a,int l);
void backprop(double *x, int r, int *t, int k);
void gradientDescent();

void file_examples();
void genikeush();

double examples[3000][2],train[3000][2];
int C[3000][3],C1[3000][3];

void file_examples(){
	char str[3000];
	char s[3000];
	FILE *fp,*fc;
	fp = fopen("examples1.txt", "r+");
	fc = fopen("check1.txt", "r+");
    // exiting program 
    if (fp == NULL) {
        printf("Error!");
        exit(1);
    }
	 if (fc == NULL) {
        printf("Error!");
        exit(1);
    }
	int i = 0;
	int j = 0;
	char delim[] = ",";
	while (fscanf(fp, "%s", str)!=EOF){
		char *ptr = strtok(str, delim);
		while(ptr != NULL){
			sscanf(ptr, "%lf", &examples[i][j]);
			j ++;
			ptr = strtok(NULL, delim);
		}
		j = 0;
		i++;
	}
    fclose(fp);
	i = 0;
	j = 0;
	while (fscanf(fc, "%s", s)!=EOF){
		char *ptr = strtok(s, delim);
		while(ptr != NULL){
			sscanf(ptr, "%lf", &train[i][j]);
			j ++;
			ptr = strtok(NULL, delim);
		}
		j = 0;
		i++;
	}
    fclose(fc);
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
		if(sq(train[i][0]-1)+sq(train[i][1]-1)<=0.49 || sq(train[i][0]+1)+sq(train[i][1]+1)<=0.49){
			C1[i][0] = 0;
			C1[i][1] = 1;
			C1[i][2] = 0;
		}else if(sq(train[i][0]+1)+sq(train[i][1]-1)<=0.49 || sq(train[i][0]-1)+sq(train[i][1]+1)<=0.49){
			C1[i][0] = 0;
			C1[i][1] = 0;
			C1[i][2] = 1;
		}else{
			C1[i][0] = 1;
			C1[i][1] = 0;
			C1[i][2] = 0;
		}
	}
	
}

int main(){
	srand ( time (NULL));
	file_examples();
	int groups[4] = {1,300,30,3000};
	int i,j;
	//best network
	activation = 1;
	L = groups[0];
	gradientDescent();
	genikeush();
	printf("the overall general error was %lf\n",Gerror);
	printf("\n\n");
	printPlot();
	return 0;
	
}
void initializeWeights(void){
	int i,j;
	//weights per neuron for hidden layer 1
	for(i=0;i<d;i++){
		for(j=0;j<H1;j++)
			weightsHL1[i][j] = randomNums();
	}
	//weights per neuron for hidden layer 2
	for(i=0;i<H1;i++){
		for(j=0;j<H2;j++)
			weightsHL2[i][j] = randomNums();
	}
	//weights per neuron for output
	for(i=0;i<H2;i++){
		for(j=0;j<K;j++)
			weightsO[i][j] = randomNums();
	}
}
//forward feeding through the network
void feedForward(double *a,int l){
	int i,j=0;
	double z,bias = 0.7;
	for(i=0;i<l;i++){
		input[i] = *(a+i);//edw isws xreiastei to +bias
	}
	for(i=l;i<d;i++){
		input[i]=bias; //+bias //kai na bgoun ta bias apo to output
	}
	//calculating the values per neuron for the first hidden layer using σ(u)
	for(j=0;j<H1;j++){
		z=0.0;
		for(i=0;i<d;i++)
			z+= input[i]*weightsHL1[i][j];//get the dot product 
		valuesHL1[j] = sigmoid(z + bias);//bias can be whatever
	}
	//calculating the values per neuron for the second hidden layer using tahn
	for(j=0;j<H2;j++){
		z=0.0;
		for(i=0;i<H1;i++)
			z+= valuesHL1[i]*weightsHL2[i][j];//get the dot product 
		valuesHL2[j] = activationH2(activation,z+bias);//bias can be whatever
	}
	//calculating output
	for(j=0;j<K;j++){
		z=0.0;
		for(i=0;i<H2;i++)
			z += weightsO[i][j]*valuesHL2[i];
		output[j] = sigmoid(z+bias);
	}
}
//Backwards Pass proccedure
void backprop(double *x, int r, int *t, int k){
	int i,j,activation = 1;
	double sum =0.0;
	feedForward(x,r);
	//Output Layer
	for(i=0;i<k;i++){
		double x =(double)*(t+i);
		deltaO[i] = (output[i]-x)*(output[i])*(1-output[i]);
	}
	//Hidden Layer2
	for(i=0;i<H2;i++){
		sum = 0.0;
		for(j=0;j<K;j++){
			sum+=deltaO[j]*weightsO[i][j];
		}
		if(activation==1)
			deltaHL2[i]=sum*(1-(sq(valuesHL2[i])));
		else
			deltaHL2[i]=sum;//using linear func
		
	}
	//Hidden Layer1
	for(i=0;i<H1;i++){
		sum=0.0;
		for(j=0;j<H2;j++)
			sum+=deltaHL2[j]*weightsHL2[i][j];
		deltaHL1[i] = sum*(1-valuesHL1[i])*(valuesHL1[i]);
	}
}
void gradientDescent(){
	initializeWeights();
	int t,n;
	int i,j,z;
	double temp = 0.0;
	int counter = 0;
	int epochs = 0;
	//group update
	while(1){
		if(L!=1){
			for(i=0;i<d;i++){
				for(j=0;j<H1;j++){
					derivWL1[i][j] = 0;
				}
			}
			for(i=0;i<H1;i++){
				for(j=0;j<H2;j++){
					derivWL2[i][j] = 0;
				}
			}
			for(i=0;i<H2;i++){
				for(j=0;j<K;j++){
					derivWO[i][j] = 0;
				}
			}
			for(z=0;z<L;z++){
				backprop(examples[counter], 2, C[counter], 3);
				for(i=0;i<d;i++){
					for(j=0;j<H1;j++){ //H1
						derivWL1[i][j] += eta*(input[i]*deltaHL1[j]);	
					}
				}
				for(i=0;i<H1;i++){  //H2
					for(j=0;j<H2;j++){
						derivWL2[i][j] += eta*(valuesHL1[i]*deltaHL2[j]);
					}
				}
				for(i=0;i<H2;i++){ //Output
					for(j=0;j<K;j++){
						derivWO[i][j] += eta*(valuesHL2[i]*deltaO[j]);
					}
				}
				counter++;
			}
			//perform the updates in output layer
			for(j=0;j<H2;j++){
				for(i=0;i<K;i++){
					 weightsO[i][j] =  weightsO[i][j] - eta*derivWO[i][j];
				}
			}
			//perform the updates in Hidden layer2
			for(j=0;j<H1;j++){
				for(i=0;i<H2;i++){
					weightsHL2[i][j] =  weightsHL2[i][j] - eta*derivWL2[i][j];
				}
			}
			//perform the updates in input leading into HL1
			for(j=0;j<d;j++){
				for(i=0;i<H1;i++){
					weightsHL1[i][j] = weightsHL1[i][j] - eta*derivWL1[i][j];
				}
			}
			if(counter==3000){
				counter = 0;
				epochs++;
				calcError(*examples ,K);
				if((fabs(error - temp)<=(0.01))&& epochs>499){
					break;
				}
				temp = error;
			}
		
		}else{	
			//serial update
			for(i=0;i<d;i++){ //H1
				for(j=0;j<H1;j++){
					weightsOldHL1[i][j] = weightsHL1[i][j];
				}
			}
			for(i=0;i<H1;i++){ //H2
				for(j=0;j<H2;j++){
					weightsOldHL2[i][j] = weightsHL2[i][j];
				}
			}
			for(i=0;i<H2;i++){
				for(j=0;j<K;j++){
					weightsOldO[i][j] = weightsO[i][j];
				}
			}
			//current error
			for(z=0;z<L;z++){
				//tha baloume kai allh metablhth metrhth ektos aprooptou...wste na diabazoume prwta thn 1h omada N stoixeiwn...meta thn 2h omada N stoixeiwn klp
				backprop(examples[counter], 2, C[counter], 3);
				for(i=0;i<d;i++){
					for(j=0;j<H1;j++){ //H1
						derivWL1[i][j] = eta*(input[i]*deltaHL1[j]);
					}
				}
				for(i=0;i<H1;i++){  //H2
					for(j=0;j<H2;j++){
						derivWL2[i][j] = eta*(valuesHL1[i]*deltaHL2[j]);
					}
				}
				for(i=0;i<H2;i++){ //Output
					for(j=0;j<K;j++){
						derivWO[i][j] = eta*(valuesHL2[i]*deltaO[j]);
					}
				}
			}
			counter++;
			//perform the updates in output layer
			for(j=0;j<H2;j++){
				for(i=0;i<K;i++)
					weightsO[i][j] =  weightsO[i][j] - eta*derivWO[i][j];
			}
			//perform the updates in Hidden layer2
			for(j=0;j<H1;j++){
				for(i=0;i<H2;i++){
					weightsHL2[i][j] =  weightsHL2[i][j] - eta*derivWL2[i][j];
				}
			}
			//perform the updates in input leading into HL1
			for(j=0;j<d;j++){
				for(i=0;i<H1;i++){
					weightsHL1[i][j] = weightsHL1[i][j] - eta*derivWL1[i][j];
				}
			}
			if(counter==3000){
				counter = 0;
				epochs++;
				calcError(*examples ,K);
				if((fabs(error - temp)<=(0.01))&& epochs>499){
					break;
				}
				temp = error;
			}
		
		}
	}
}
//calculate the overall error
void calcError(double *t , int u){
	check = 1;
	error = 0.0;
	int i,j;
	double sum = 0.0;
	for(j=0;j<3000;j++){
		sum = 0.0;
		feedForward((t+j),2);
		for(i = 0;i<u;i++){
			sum+=0.5*sq(((double)C[j][i] - output[i]));
		}
		error += sum;
	}
}
void genikeush(){
	int i,j;
	double max = -1;
	int winner = 0;
	double decision = 1.0;
	for(i=0;i<3000;i++){
		feedForward(train[i],2);
		for(j=0;j<K;j++){
			if(output[j]>max){
				winner = j;
				max = output[j];
			}
		}
		max = -1;
		if(winner==0 && C1[i][0] == 1){
			decision++;
		}else if(winner==1 && C1[i][1]==1){
			decision++;
		}else if(winner==2 && C1[i][2]==1){
			decision++;
		}
	}
	Gerror = 1 - (decision)/(double)3000;
	printf("the error was %lf\n",error);
	printf("right decisions was %lf\n",decision/(double)3000);
}

void printPlot(){
	FILE *fp1,*fp2;
	fp1 = fopen("MLP1.txt", "w+");
	fp2 = fopen("MLP2.txt", "w+");
	if(fp1 == NULL || fp2 == NULL){
		printf("Error!");
		exit(1);
    }
	int i,pos=0;
	double distance;
	for(i=0;i<3000;i++){
		feedForward(train[i],2);
		distance = 1-output[0];
		pos = 0;
		if((1-output[1])<distance){
			distance = 1-output[1];
			pos = 1;
		}
		if((1 - output[2])< distance){
			distance = 1-output[2];
			pos = 2;
		}
		if(C1[i][pos] !=1){
			//wrong category print -
			fprintf(fp1, "%f %f\n", train[i][0],train[i][1]);
		}
		else{
			//correct category print +
			fprintf(fp2, "%f %f\n", train[i][0],train[i][1]);
		}
	}
	fclose(fp1);
	fclose(fp2);
}
double randomNums(void){
    	return (double)(rand()) /RAND_MAX*2.0-1.0;//generates a num between -1 and 1;
}
double sq(double x){
	return x*x;
}
double sigmoid(double x){
	return  1/(1+exp(-x));
}
	


