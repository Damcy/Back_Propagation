//
//  main.c
//  BP
//
//  Created by Damcy on 13-11-2.
//  Copyright (c) 2013年 Damcy. All rights reserved.
//
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#define Feature 256
#define Hide_node 100
#define Feature_plus_1 257
#define Hide_node_plus_1 101
#define max_err 0.000005
#define max_train 3000000
#define Data_size 7291

double data[Data_size][Feature_plus_1];
double o_in[Feature_plus_1]={1.0};				//plus 1 for bias
double o_hide[Hide_node];						//count for zero
double o_out[10];								//0 to 9
double T[10];									//standard answer
double Err[10];									//value of each output error
double w_hide[Hide_node][Feature_plus_1], w_hide_last[Hide_node][Feature_plus_1]={0};		//w_hide, w_hide[][0] represent theta
double w_output[10][Hide_node_plus_1], w_output_last[10][Hide_node_plus_1]={0};			//w_output, w_output[][0] represent theta
double delta_output[10], delta_hide[Hide_node];	//误差

double learn_rate = 0.6;		//learn_rate when count>2000 learn_rate = 0.3
double alpha = 0.6;

void init_data()	//read all training data
{
	FILE *fq;
	int temp_i, temp_j;
	fq = fopen("/Users/Damcy/Documents/cplusplus/bp/bp/bp/train.txt","r");
	if(fq == NULL)
	{
		printf("cannt open the training data\n");
		exit(0);
	}
	for(temp_i = 0; temp_i < Data_size; ++temp_i)
	{
		for(temp_j = 0; temp_j < Feature_plus_1; ++temp_j)
		{
			//read an element data[temp_i][temp_j]
			fscanf(fq, "%lf", &data[temp_i][temp_j]);
		}
	}
	fclose(fq);
}
/*
 void init_data()
 {
 FILE *fq;
 int temp_i, temp_j;
 fq = fopen("output.txt","r");
 if(fq == NULL)
 {
 printf("cannt open the training data\n");
 exit(0);
 }
 for(temp_i = 0; temp_i < Data_size; ++temp_i)
 {
 for(temp_j = 0; temp_j < Feature_plus_1; ++temp_j)
 {
 //read an element data[temp_i][temp_j]
 fscanf(fq, "%d", &data[temp_i][temp_j]);
 }
 }
 fclose(fq);
 }
 */
//function to init w_hide and w_output every elements range from -2.4/F to 2.4/F
void init_w()
{
	double w_min, w_max, temp;
	int rand_num, temp_i, temp_j;
	w_min = -((double)2.4/Feature);
	w_max = (double)2.4/Feature;
	srand(time(NULL));
	for(temp_i = 0; temp_i < Hide_node; ++temp_i)
		for(temp_j = 0; temp_j < Feature_plus_1; ++temp_j)
		{
			rand_num = rand()%19;
			temp = (double)rand_num / 1000;
			w_hide[temp_i][temp_j] = temp + w_min;
		}
	for(temp_i = 0; temp_i < 10; ++temp_i)
		for(temp_j = 0; temp_j < Hide_node_plus_1; ++temp_j)
		{
			rand_num = rand()%19;
			temp = (double)rand_num / 1000;
			w_output[temp_i][temp_j] = temp + w_min;
		}
}

//read o_in and init T
void init_o_T(int line)
{
	int temp_i=1, x;
    
	line = line % Data_size;
	for(temp_i = 1; temp_i < Feature_plus_1; ++temp_i)
	{
		o_in[temp_i] = data[line][temp_i];
	}
	for(temp_i = 0; temp_i < 10; ++temp_i)
	{
		T[temp_i] = 0;
	}
	x = data[line][0];
	T[x] = 1;
}

//mean of Err
int mean_of_err()
{
	int k = 0;
	double sum = 0;
	for(k = 0; k < 10; ++k)
	{
		sum += Err[k];
	}
	sum /= 10.0;
	if(sum < max_err)
		return 1;
	else
		return 0;
}

//sigmod
double sigmod(double x)
{
	return (double)1.0/(1.0 + exp(-x));
}

//update w
void training()
{
	int count = 0, mark_count = 0;
	int mark = 1;
	int temp_i = 0, temp_j = 0;
	double temp_z = 0;
	double sum = 0;
	while(mark)
	{
		init_o_T(count);
		printf("\n%d\n",count);
		//calculate o_hide
		for(temp_i = 0; temp_i < Hide_node; ++temp_i)
		{
			sum = 0;
			for(temp_j = 0; temp_j < Feature_plus_1; ++temp_j)
			{
				sum += o_in[temp_j] * w_hide[temp_i][temp_j];
			}
			o_hide[temp_i] = sigmod(sum);
		}
        
		//calculate o_out
		for(temp_i = 0; temp_i < 10; ++temp_i)
		{
			sum = w_output[temp_i][0];
			for(temp_j = 1; temp_j < Hide_node_plus_1; ++temp_j)
			{
				sum += o_hide[temp_j-1] * w_output[temp_i][temp_j];
			}
			o_out[temp_i] = sigmod(sum);
		}
        
		//calculate Err
		for(temp_i = 0; temp_i < 10; ++temp_i)
		{
			temp_z = o_out[temp_i] - T[temp_i];
			temp_z = temp_z * temp_z;
			Err[temp_i] = (double)temp_z / 2;
		}
		//计算误差
		for(temp_i = 0; temp_i < 10; ++temp_i)
		{
			delta_output[temp_i] = o_out[temp_i] * (1 - o_out[temp_i]) * (o_out[temp_i] - T[temp_i]);
		}
		for(temp_i = 0; temp_i < Hide_node; ++temp_i)
		{
			sum = 0;
			for(temp_j = 0; temp_j < 10; ++temp_j)
			{
				sum += w_output[temp_j][temp_i+1] * delta_output[temp_j];
			}
			delta_hide[temp_i] = o_hide[temp_i] * (1 - o_hide[temp_i]) * sum;
		}
		
		//update w_output
		for(temp_i = 0; temp_i < 10; ++temp_i)
		{
			w_output[temp_i][0] = w_output[temp_i][0] - learn_rate * delta_output[temp_i] + alpha*w_output_last[temp_i][0];
			for(temp_j = 1; temp_j < Hide_node_plus_1; ++temp_j)
			{
				w_output[temp_i][temp_j] = w_output[temp_i][temp_j] - (learn_rate*delta_output[temp_i]*o_hide[temp_j-1]) + (alpha*w_output_last[temp_i][temp_j]);
			}
		}
		//update w_hide
		for(temp_i = 0; temp_i < Hide_node; ++temp_i)
		{
			for(temp_j = 0; temp_j < Feature_plus_1; ++temp_j)
			{
				w_hide[temp_i][temp_j] = w_hide[temp_i][temp_j] - (learn_rate*delta_hide[temp_i]*o_in[temp_j]) + (alpha*w_hide_last[temp_i][temp_j]);
			}
		}
        
		//update w_output_last
		for(temp_i = 0; temp_i < 10; ++temp_i)
		{
			for(temp_j = 0; temp_j < Hide_node_plus_1; ++temp_j)
			{
				w_output_last[temp_i][temp_j] =  (alpha*w_output_last[temp_i][temp_j]) - (learn_rate*delta_output[temp_i]*o_hide[temp_j-1]);
			}
		}
		//update w_hide_last
		for(temp_i = 0; temp_i < Hide_node; ++temp_i)
		{
			for(temp_j = 0; temp_j < Feature_plus_1; ++temp_j)
			{
				w_hide_last[temp_i][temp_j] = (alpha*w_hide_last[temp_i][temp_j]) - (learn_rate*delta_hide[temp_i]*o_in[temp_j]);
			}
		}
         
		if(mean_of_err())
		{
			++mark_count;
		}
		else
		{
			mark_count = 0;
		}
		if(mark_count > 250)
		{
			break;
		}
		if(++count > max_train)
		{
			break;
		}
		
		if(count > 800000)
        {
			learn_rate = 0.4;
            alpha = 0.2;
        }
		else if(count > 400000)
        {
			learn_rate = 0.5;
            alpha = 0.3;
        }
	}
}

//record w_hide and w_output
void save_w_hide()
{
	FILE *fq = NULL;
	int temp_i , temp_j;
	fq = fopen("/Users/Damcy/Documents/cplusplus/bp/bp/bp/w_hide.txt","w");
	if(fq == NULL)
	{
		printf("cannt open w_hide.txt\n");
	}
	else
	{
		for(temp_i = 0; temp_i < Hide_node; ++temp_i)
		{
			for(temp_j = 0; temp_j < Feature_plus_1; ++temp_j)
				fprintf(fq, "%.16lf ", w_hide[temp_i][temp_j]);
			fprintf(fq, "\n");
		}
	}
	fclose(fq);
}
void save_w_output()
{
	FILE *qf = NULL;
	int temp_i , temp_j;
	qf = fopen("/Users/Damcy/Documents/cplusplus/bp/bp/bp/w_output.txt","w");
	if(qf == NULL)
	{
		printf("cannt open w_output.txt\n");
	}
	else
	{
		for(temp_i = 0; temp_i < 10; ++temp_i)
		{
			for(temp_j = 0; temp_j < Hide_node_plus_1; ++temp_j)
				fprintf(qf, "%.16lf ", w_output[temp_i][temp_j]);
			fprintf(qf, "\n");
		}
	}
	fclose(qf);
}

int main()
{
	init_data();
	init_w();
	training();
	save_w_hide();
	save_w_output();
	system("pause");
}