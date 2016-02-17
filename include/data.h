#ifndef DATA_H
#define DATA_H
#define SEP " "

typedef struct Sample_ {
  int n_attrs;
  double * values;
  int n_classes;
} Sample;

typedef struct Data_ {
  int n_classes;

  int n_samples;
  Sample ** samples;
} Data;

/* Public Methods */
void train_and_test_from_data(Data * train,Data * test, Data * data,int porcen)
Data * data_from_file(char * file);
void data_free(Data * d);

/* Private Methods */
Data * data_new();
int data_init(Data * d, int n_classes, int n_samples, int n_attrs);

Sample * sample_new(int n_attrs,int n_classes);
void sample_fill(Sample * sample, char * text);
void sample_free(Sample * sample);
void n_classes_set(Data * d, int n_classes);
void n_samples_set(Data * d, int n_samples)

#endif /* DATA_H */ 
