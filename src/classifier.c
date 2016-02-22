#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "classifier.h"

/* Public Methods */
Classifier * nnc_new() {
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

    /* Default Parameters */
    c->learning_rate = DEF_LEARNING_RATE;
    c->max_epochs = DEF_MAX_EPOCHS;
    c->max_accuracy = DEF_MAX_ACCURACY;
    c->max_mse = DEF_MAX_MSE;

    /* Statistics Inicialization */
    c->file_statistics = fopen("out/stats.csv","w");
    c->accuracy_training = 0.0;
    c->accuracy_generalization = 0.0;
    c->accuracy_validation = 0.0;
    c->mse_training = 0.0;
    c->mse_generalization = 0.0;
    c->mse_validation = 0.0;

    return c;
}

int nnc_set_training_parameters(Classifier * c, double learning_rate,int bipolar) {
    if (!c) return -1;
    
    if (learning_rate) c->learning_rate = learning_rate;
    if (learning_rate) c->bipolar = bipolar;

    return 0;
}

int nnc_set_neural_network(Classifier * c, Neural_Network * nn){
    c->nn = nn;
    return 0;
}

int nnc_set_data(Classifier * c, Data * d) {
    if (!c || !d) return -1;

    train_and_test_from_data(d , c->data_training , c->data_validation, DEF_TRAINIG_PERCENT);
    
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
    int i;
    for ( i = 0; i < c->epoch; i++){
        nnc_run_training_epoch(c);
        nnc_run_statistics(c, i);
    }
    return 0;
}

/* Private Methods */

void nnc_run_training_epoch(Classifier * c){
	int i , j;
    double * values;
    int n_clases = data_get_n_classes(*c->data_training);
	for( i = 0 ; i < data_get_n_samples(*(c->data_training)) ; i++){
		Sample * s = data_get_samples(*(c->data_training))[i];
		int n_attrs = sample_get_n_attrs(*s);
		nn_update_neurons(c->nn, sample_get_values(*s), n_attrs, 0); 
        values = (double *) malloc(n_clases* sizeof(double));


        if(n_clases == 2 && c->bipolar==1){
            values[0] = sample_get_class(*s);
        }
        else{
            for( j = 0 ; j< n_clases ; j++ ){
            values[j] = 0; 
            }
            values[sample_get_class(*s)] = 1;
        }
        nn_update_weights(c->nn, c->learning_rate, values);
	}
}

void nnc_run_statistics(Classifier * c, int epoch){
    int i ;
    int s = 0; 
    double * output;
    int n_clases = data_get_n_classes(*c->data_training);
    for( i = 0 ; i < data_get_n_samples(*(c->data_training)) ; i++){
        Sample * s = data_get_samples(*(c->data_training))[i];
        int n_attrs = sample_get_n_attrs(*s);
        nn_update_neurons(c->nn, sample_get_values(*s), n_attrs, 0); //TODO el 0 es porque no queremos discreta
        output = nn_get_output(*c->nn);

        if(n_clases == 2 && c->bipolar==1){
            if(output[0] == sample_get_class(*s))
                s++;
        }
        else{
            if(output[sample_get_class(*s)] == 1)
                s++;
        }
        
    }
    fprintf(c->file_statistics, "%d;%lf\n",epoch , ((double) s*100 )/ data_get_n_samples(*(c->data_training)));
}

