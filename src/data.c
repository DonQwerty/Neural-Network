#include <stdio.h>
#include <stdlib.h>
#include "data.h"

/* Public Methods */









/* Private Methods */

Sample * sample_new(int n_attrs){
  Sample * sample = (Sample *) malloc(sizeof(Sample));
  sample->n_attrs = n_attrs;
  sample->attrs = (double *) malloc(n_attrs * sizeof(double));

  return sample;
}

Data * data_new(){
  Data * data = (Data *) malloc(sizeof(Data));
  
  if (!data) return null;
  
  data->size = 0;
  data->n_values = 0;
  data->samples = null;

  return data;
}

int data_init(Data * d, int n_classes, int n_samples, int n_attrs) {
  int i;

  if (!d) return -1;

  d->n_classes = n_classes;
  d->n_samples = n_samples;
  d->samples = (Sample **) malloc(n_samples * sizeof(Sample *));

  for (i = 0; i < n_samples; i++) {
    d->samples[i] = sample_new(n_attrs);
  }

  return 0;
}


