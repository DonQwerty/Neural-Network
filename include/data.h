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
void train_and_test_from_data(Data ** train,Data ** test, Data * data,int porcen,int normal);
Data * data_from_file(char * file,int unique);
void data_free(Data * d);

/* Private Methods */
Data * data_new();
int data_init(Data * d, int n_classes, int n_samples, int n_attrs);

Sample * sample_new(int n_attrs,int n_classes);
void sample_fill(Sample * sample, char * text,int unique);
void sample_free(Sample * sample);
int data_normalize(Data *d, double * means, double * desv);
/*Setter*/
void n_classes_set(Data * d, int n_classes);
void n_samples_set(Data * d, int n_samples);

/*Getter */
int data_get_n_classes(Data  d);
int data_get_n_samples(Data  d);
Sample ** data_get_samples(Data * d);

int sample_get_n_attrs(Sample  s);
int sample_get_n_classes(Sample  s);
double * sample_get_values(Sample  s);
int sample_get_class(Sample s);

#endif /* DATA_H */ 
