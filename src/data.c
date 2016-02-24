#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"

/* Public Methods */

void train_and_test_from_data(Data ** train,Data ** test, Data * data,int porcen){
	int i,r,n;
	int index;
	Sample * aux;
	n = data->n_samples;
	*train = data_new();
	*test = data_new();
	srand(time(NULL));
	for (i = 0 ; i < n ; i++){
		r = rand()%(n-i);
		aux =  data->samples[i];
        data->samples[i]= data->samples[r+i];
        data->samples[r+i] =  aux;
	}
	index = (porcen* n)/100;
	n_samples_set(*train,index);
	n_samples_set(*test, n - index);
	n_classes_set(*train,data->n_classes);
	n_classes_set(*test,data->n_classes);
	(*train)->samples = data->samples;
	(*test)->samples = data->samples + index;
	free(data);
}

Data * data_from_file(char * file, int percep){
    FILE * f = fopen(file,"r");
    if(!f){
        perror( file);
        return NULL;
    }
    char text[100];
    int n_samples=-1;
    while(fgets(text,100,f)){
        n_samples++;
    }
    rewind(f);
    fgets(text,100,f);
    char * ptr = NULL;
    ptr = strtok(text," ");
    int n_attrs = atoi(ptr);
    ptr = strtok(NULL," ");
    int n_classes = atoi(ptr);

    Data * data=data_new();
    data_init(data,n_classes,n_samples,n_attrs);
    int i=0;
    while(fgets(text,100,f)){
        sample_fill(data->samples[i],text, percep);
        i++;
    }
    return data;
}



void data_free(Data * d){
    int i;
    for ( i = 0; i < d->n_samples; i++){
        sample_free(d->samples[i]);
    }
    free(d->samples);
    free(d);
}


/* Private Methods */

Sample * sample_new(int n_attrs,int n_classes){
  Sample * sample = (Sample *) malloc(sizeof(Sample));
  sample->n_attrs = n_attrs;
  sample->n_classes = n_classes;
  sample->values = (double *) malloc((n_attrs+1) * sizeof(double));

  return sample;
}

void sample_fill(Sample * sample, char * text, int percep){
    int i,j;
    int class;
    char * ptr = NULL;
    ptr = strtok(text,SEP);
    sample->values[0] = atof(ptr);
    for(i=1 ; i < sample->n_attrs ; i++){
        ptr = strtok(NULL,SEP);
        sample->values[i] = atof(ptr);

    }
    if(percep){
      ptr = strtok(NULL,SEP);
      class = atoi(ptr); 
      
      sample->values[i] =(class == 1 ) ? -1 :  1;
      return; 
    }

    for(j=0 ; j < sample->n_classes ; j++){
        ptr = strtok(NULL,SEP);
        class = atoi(ptr); 
        if(class==1){
            sample->values[i]=j;
        }
    }
	
}

void sample_free(Sample * sample){
    free(sample->values);
    free(sample);
}

Data * data_new(){
  Data * data = (Data *) malloc(sizeof(Data));
  
  if (!data) return NULL;
  
  data->n_classes=0;
  data->n_samples = 0;
  data->samples = NULL;

  return data;
}

int data_init(Data * d, int n_classes, int n_samples, int n_attrs) {
  int i;

  if (!d) return -1;

  d->n_classes = n_classes;
  d->n_samples = n_samples;
  d->samples = (Sample **) malloc(n_samples * sizeof(Sample *));

  for (i = 0; i < n_samples; i++) {
      d->samples[i] = sample_new(n_attrs,n_classes);
  }

  return 0;
}

/*Setter  */

void n_samples_set(Data * d, int n_samples){
	d->n_samples = n_samples;
}

void n_classes_set(Data * d, int n_classes){
	d->n_classes = n_classes;
}


/* Getter */


int data_get_n_classes(Data  d){
	return d.n_classes ;
}

int data_get_n_samples(Data  d){
	return d.n_samples ;
}

Sample ** data_get_samples(Data  d){
	return d.samples ;
}

int sample_get_n_attrs(Sample  s){
	return s.n_attrs ;
}

int sample_get_n_classes(Sample  s){
	return s.n_classes ;
}

double * sample_get_values(Sample  s){
	return s.values ;
}

int sample_get_class(Sample s){
  return (int)s.values[s.n_attrs];
}