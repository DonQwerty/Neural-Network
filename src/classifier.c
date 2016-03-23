#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "classifier.h"

/* Public Methods */
Classifier * nnc_new(char * output, char * stats_file) {
    Classifier * c;
    c = (Classifier *) malloc(sizeof(Classifier));
    if (!c) return NULL;

    /* Neural Network and Data inicialization */
    c->nn = NULL;
    c->data_training = NULL;
    c->data_generalization = NULL;
    c->data_validation = NULL;
    c->epoch = 0;
    c->bipolar = 0;
    c->function_transfer=0;

    /* Default Parameters */
    c->learning_rate = DEF_LEARNING_RATE;
    c->max_epochs = DEF_MAX_EPOCHS;
    c->max_accuracy = DEF_MAX_ACCURACY;
    c->max_mse = DEF_MAX_MSE;

    c->predictions = fopen(output, "w");
    /* Statistics Inicialization */
    c->file_statistics = fopen(stats_file,"w");
    c->accuracy_training = 0.0;
    c->accuracy_generalization = 0.0;
    c->accuracy_validation = 0.0;
    c->mse_training = 0.0;
    c->mse_generalization = 0.0;
    c->mse_validation = 0.0;
    return c;
}

int nnc_set_training_parameters(Classifier * c, double learning_rate,int bipolar,int function_transfer) {
    if (!c) return -1;

    if (learning_rate) c->learning_rate = learning_rate;
    if (bipolar) c->bipolar = bipolar;
    if (function_transfer) c->function_transfer = function_transfer;

    return 0;
}

int nnc_set_neural_network(Classifier * c, Neural_Network * nn){
    c->nn = nn;
    return 0;
}

int nnc_set_data(Classifier * c, Data * d, int flag, int percen) {
    if (!c || !d) return -1;
    if(flag){
        c->data_training = d;
        c->data_validation = d;
        return 0;
    }
    if(percen == -1)
        percen = DEF_TRAINIG_PERCENT;
    train_and_test_from_data( &(c->data_training) , &(c->data_validation),d, percen);

    return 0;
}

int nnc_free(Classifier * c){
    if(!c)
        return -1;
    fclose(c->file_statistics );
    fclose(c->predictions);
    nn_free(c->nn);
    data_free(c->data_training);
    free(c);
    return 0;

}

int nnc_set_stopping_conditions(Classifier * c, int max_epochs, double max_accuracy, double max_mse) {
    if (!c) return -1;

    if (max_epochs)   c->max_epochs = max_epochs;
    if (max_accuracy) c->max_accuracy = max_accuracy;
    if (max_mse)      c->max_mse = max_mse;

    return 0;
}

int nnc_train_network(Classifier * c){

    double accuracy;

    while(nnc_check_stopping_conditions(c) != 1){
        // printf("---Epoca %d--\n" , c->epoch);
        fflush(stdout);
        nnc_run_training_epoch(c);
        accuracy = nnc_run_statistics(c);
        c->epoch++;
    }

    c->accuracy_training = accuracy;

    return 0;
}

double nnc_classifier(Classifier * c, int predict_flag){
    int i,j, pos;
    double aux;
    double * output;
    int sum = 0;
    double res;
    int n_clases = data_get_n_classes(*c->data_validation);
    for ( i = 0; i < data_get_n_samples(*(c->data_validation)); i++){
        Sample * s = data_get_samples(*(c->data_validation))[i];
        int n_attrs = sample_get_n_attrs(*s);
        nn_update_neurons(c->nn, sample_get_values(*s), n_attrs, 0,1);
        output = nn_get_output(c->nn);
        //printf("%lf %lf %lf %lf %lf\n", s->values[0],s->values[1],s->values[2],s->values[3],output[0]);
        if(n_clases == 2 && c->bipolar==1){
            res = output[0] ;
            if(c->function_transfer){
                res = (res < 0) ? -1 : 1;
                if((res >= 0 && sample_get_class(*s)==1) || (res < 0 && sample_get_class(*s)==-1) )
                    sum++;
            }
            else{
                if(output[0] == sample_get_class(*s))
                    sum++;
            }
            if(!predict_flag)
                fprintf(c->predictions, "%d\t%d\n",sample_get_class(*s), (int)res);
            else
                fprintf(c->predictions, "%d\n", (int)res);
        }
        else{
            if(c->function_transfer){
                aux=output[0];
                for(j = 0 ; j< n_clases ; j++){
                    if(output[j]>= aux){
                        pos = j;
                        aux = output[j];
                    }
                }
                if(pos == sample_get_class(*s) )
                    sum++;
            }
            else{
                if(output[sample_get_class(*s)] == 1)
                  sum++;
            }
        }
    }
    c->accuracy_validation = ((double) sum * 100)/data_get_n_samples(*(c->data_validation));
    // printf("El porcentaje de acierto es de %lf \n", ((double) sum * 100)/data_get_n_samples(*(c->data_validation)));
    return ((double) sum * 100)/data_get_n_samples(*(c->data_validation));
}



void nnc_print_info(Classifier * c) {
    printf("[ INFO ] Number of iterations: %d\n", c->epoch);
    printf("[ INFO ] Accuracy (%% of success):\n");
    printf("           Train: %lf\n", c->accuracy_training);
    printf("           Test:  %lf\n", c->accuracy_validation);
    return;
}


/* Private Methods */
void nnc_run_training_epoch(Classifier * c){
    int i , j;
    double * values;
    Sample * s;

    int n_clases = data_get_n_classes(*c->data_training);

    values = (double *) malloc(n_clases* sizeof(double));
    for( i = 0 ; i < data_get_n_samples(*(c->data_training)) ; i++){
        s = data_get_samples(*(c->data_training))[i];
        int n_attrs = sample_get_n_attrs(*s);
        nn_update_neurons(c->nn, sample_get_values(*s), n_attrs, 0, 1);
        //fprint_output(c->nn, stdout);
        if(n_clases == 2 && c->bipolar==1){
            values[0] = sample_get_class(*s);
        }
        else{
            for( j = 0 ; j< n_clases ; j++ ){
            values[j] = 0;
            }

            values[sample_get_class(*s)] = 1;
        }
        //fprint_w(c->nn,stdout);

        nn_compute_out_err(c->nn, values);
        nn_update_weights(c->nn, c->learning_rate, values);

        //nnc_run_statistics(c);

    }

    free(values);
}

double nnc_run_statistics(Classifier * c){
    int i ,j, pos;
    double aux;
    int sum = 0;
    double * output;
    double error=0;
    int n_clases = data_get_n_classes(*c->data_training);
    for( i = 0 ; i < data_get_n_samples(*(c->data_training)) ; i++){
        Sample * s = data_get_samples(*(c->data_training))[i];
        int n_attrs = sample_get_n_attrs(*s);
        nn_update_neurons(c->nn, sample_get_values(*s), n_attrs, 0,1);
        output = nn_get_output(c->nn);

        if(n_clases == 2 && c->bipolar==1){
            if(c->function_transfer){
                if((output[0] >= 0 && sample_get_class(*s)==1) || (output[0] < 0 && sample_get_class(*s)==-1) )
                    sum++;
            }
            else{
                if(output[0] == sample_get_class(*s))
                    sum++;
            }
            error+= pow(((double)sample_get_class(*s) - output[0]),2);

        }
        else{
            if(c->function_transfer){
                aux=output[0];
                for(j = 0 ; j< n_clases ; j++){
                    if(output[j]>= aux){
                        pos = j;
                        aux = output[j];
                    }
                }
                printf("%d  %lf  %lf\n",pos ,output[0],output[1] );
                if(pos == sample_get_class(*s) )
                    sum++;
            }
            else{
                pos = sample_get_class(*s);
                if(output[sample_get_class(*s)] == 1)
                  sum++;
            }

        }

    }
    fprintf(c->file_statistics, "%d;%lf;%lf\n",c->epoch , ((double) sum*100 )/ data_get_n_samples(*(c->data_training)),error/data_get_n_samples(*(c->data_training)));
    return ((double) sum*100 )/ data_get_n_samples(*(c->data_training));
}

int nnc_check_stopping_conditions(Classifier * c) {
    // TODO Completar
    if (c->epoch >= c->max_epochs) {
        return 1;
    }
    return 0;
}
