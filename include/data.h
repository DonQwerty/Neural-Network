#ifndef DATA_H
#define DATA_H

typedef struct Sample_ {
  int n_attrs;
  double * attrs;
} Sample;

typedef struct Data_ {
  int size;
  int n_classes;

  int n_samples;
  Sample ** samples;
} Data;

/* Public Methods */
Data * data_from_file(char * file);
void data_free(Data * d);

/* Private Methods */
Data * data_new();
int data_init(Data * d, int n_samples, int n_attrs);

Sample * sample_new(int n_attrs);


#endif /* DATA_H */ 
