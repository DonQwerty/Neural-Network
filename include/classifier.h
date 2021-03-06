#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include  <stdio.h>
#include "data.h"
#include "neural_network.h"
#include "neural_network_functions.h"

/* Default Values */
#define DEF_TRAINIG_PERCENT        60.0
#define DEF_GENERALIZATION_PERCENT 20.0
#define DEF_VALIDATION_PERCENT     20.0
#define DEF_LEARNING_RATE          0.02
#define DEF_MAX_EPOCHS             1000
#define DEF_MAX_ACCURACY           95
#define DEF_MAX_MSE                0.001

typedef struct Classifier_ {

    /* Classifier Variables */
    Neural_Network * nn;
    Data           * data_training;
    Data           * data_generalization;
    Data           * data_validation;
    int              epoch;

    /* Classifier Parameters */
    double learning_rate;
    int    max_epochs;
    double max_accuracy;
    double max_mse;
    int bipolar;
	int normalize;
    int function_transfer;
    int mode_unique_neuron;
	int data_flag;

    /* Epoch Statistics */
    FILE * file_statistics;

    /*Output file*/
    FILE * predictions;
    /* Accuracy */
    double accuracy_training;
    double accuracy_generalization;
    double accuracy_validation;
    /* Mean Squared Error */
    double mse_training;
    double mse_generalization;
    double mse_validation;

    double mse_model_training;
    double mse_model_validation; 

} Classifier;


/* Public Methods */
/* Allocates memory for the classifier with default parameters */
Classifier * nnc_new(char * output, char * stats_file, int normalize);

/* Changes the parameters. If NULL, the default value is used */
int nnc_set_training_parameters(Classifier * c, double learning_rate,int bipolar, int function_transfer, int mode_unique_neuron);

/* Receives the data and divides it into training, generalization and validation */
int nnc_set_data(Classifier * c, Data * d, int flag, int percen, int shuffle);

double ** nnc_calculate_mean_desv( Data * d);

/* Sets the stopping conditions. If NULL, the condition is not used */
int nnc_set_stopping_conditions(Classifier * c, int max_epochs, double max_accuracy, double max_mse);

/* Trains the network until a stopping condition is reached */
int nnc_train_network(Classifier * c,int autoencoder);

/*Classifies the data validation and return the success rate*/
double nnc_classifier(Classifier * c,int real_class, int encoder_ts);

void nnc_classifier_recursive (Classifier * c, int n_a, int n_s, int n_f, double * input);

void nnc_compute_error(Classifier * nnc, int n_b, int n_f);

/* Prezi Print */
void nnc_print_info(Classifier * c, int predict_flag, int encoder_ts);

/* Deletes the classifier and the neural network and frees its memory */
int nnc_free(Classifier * c);

/*Sets the neural network*/
int nnc_set_neural_network(Classifier * c, Neural_Network * nn);


/* Private Methods */
void nnc_run_training_epoch(Classifier * c, int encoder_ts);

double nnc_run_statistics(Classifier * c, int encoder_ts);

int nnc_check_stopping_conditions(Classifier * c);

Data * nnc_get_data_validation(Classifier * c);

#endif /* CLASSIFIER_H */
