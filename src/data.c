#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

/* Public Methods */

Data * data_from_file(char * file){
    FILE * f = fopen(file,"r");
    if(!f){
        perror( file);
        return NULL;
    }
    char text[100];
    int n_samples=-1;
    while(!fgets(text,100,f)){
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
    while(!fgets(text,100,f)){
        sample_fill(data->samples[i],text);
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

void sample_fill(Sample * sample, char * text){
    int i,j;
    int class;
    char * ptr = NULL;
    ptr = strtok(text,SEP);
    sample->values[0] = atof(ptr);
    for(i=1 ; i < sample->n_attrs ; i++){
        ptr = strtok(NULL,SEP);
        sample->values[i] = atof(ptr);
    }
    for(j=i ; j < sample->n_classes ; j++){
        ptr = strtok(NULL,SEP);
        class = atoi(ptr); 
        if(class==1){
            sample->values[i]=j-i;
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
  
  data->size = 0;
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

