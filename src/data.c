#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "math.h"

/* Public Methods */

void train_and_test_from_data(Data ** train,Data ** test, Data * data,int porcen,int normal, int shuffle){
    int i,r,n;
    int index;
    Sample * aux;
    n = data->n_samples;
    *train = data_new();
    *test = data_new();
    if(shuffle){
        srand(time(NULL));
        for (i = 0 ; i < n ; i++){
            r = rand()%(n-i);
            aux =  data->samples[i];
            data->samples[i]= data->samples[r+i];
            data->samples[r+i] =  aux;
        }
    }
    if (porcen==100){
        n_samples_set(*train,n);
        n_samples_set(*test, n );
        n_classes_set(*train,data->n_classes);
        n_classes_set(*test,data->n_classes);
        (*train)->samples = data->samples;
        (*test)->samples = data->samples;

    }
    else{
        index = (porcen* n)/100;
        n_samples_set(*train,index);
        n_samples_set(*test, n - index);
        n_classes_set(*train,data->n_classes);
        n_classes_set(*test,data->n_classes);
        (*train)->samples = data->samples;
        (*test)->samples = data->samples + index;

    }
    free(data);
}

Data * data_from_file(char * file, int unique, int output){
    FILE * f = fopen(file,"r");
    if(!f){
        perror( file);
        return NULL;
    }
    char text[400];
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
    data->flagOutput=output;
    data_init(data,n_classes,n_samples,n_attrs);
    int i=0;
    while(fgets(text,400,f)){
        //printf("%s\n",text );
        sample_fill(data->samples[i],text, unique,output);
        i++;
    }
    fclose(f);
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

void adapta_fichero_serie(char * src, char * dst, int n_a, int n_s) {
    char cmd[] = "sh script/adapta-fichero-serie.sh %s %s %d %d";
    char cmd2[100];
    sprintf(cmd2, cmd, src, dst, n_a, n_s);
    system(cmd2);
}


/* Private Methods */

Sample * sample_new(int n_attrs,int n_classes, int flagOutput){
    Sample * sample = (Sample *) malloc(sizeof(Sample));
    sample->n_attrs = n_attrs;
    sample->n_classes = n_classes;
    if(flagOutput)
        sample->values = (double *) malloc((n_attrs+n_classes) * sizeof(double));
    else
        sample->values = (double *) malloc((n_attrs+1) * sizeof(double));

    return sample;
}

void sample_fill(Sample * sample, char * text, int unique, int output){
    int i,j;
    int class;
    double v;
    char * ptr = NULL;
    ptr = strtok(text,SEP);
    sample->values[0] = atof(ptr);

    for(i=1 ; i < sample->n_attrs ; i++){
        ptr = strtok(NULL,SEP);
        sample->values[i] = atof(ptr);

    }
    if(unique){
        ptr = strtok(NULL,SEP);
        class = atoi(ptr);

        sample->values[i] =(class == 1 ) ? -1 :  1;
        return;
    }
    if(output){
       for(j=0 ; j < sample->n_classes ; j++){
            ptr = strtok(NULL,SEP);
            v = atof(ptr);
            sample->values[j+i]=v;
            
        } 
    }
    else{
        for(j=0 ; j < sample->n_classes ; j++){
            ptr = strtok(NULL,SEP);
            class = atoi(ptr);
            if(class==1){
                sample->values[i]=j;
            }
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
        d->samples[i] = sample_new(n_attrs,n_classes,d->flagOutput);
    }

    return 0;
}
int data_normalize(Data *d, double * means, double * desv){
    int i, j ,n;
    n= d->samples[0]->n_attrs;
    for (i = 0; i < d->n_samples; i++) {
        for (j = 0; j < n; j++) {
            //printf("%lf--M:%lf--D:%lf--",d->samples[i]->values[j],means[j],pow(desv[j],1/2));
            d->samples[i]->values[j] = (d->samples[i]->values[j] - means[j])/pow(desv[j],0.5) ;
            //printf("%lf\n",d->samples[i]->values[j]);
        }
        //printf("\n\n");
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

Sample ** data_get_samples(Data  * d){
    return d->samples ;
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
