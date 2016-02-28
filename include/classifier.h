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
#define DEF_MAX_EPOCHS             1500
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
    int function_transfer;
    
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
    
} Classifier;


/* Public Methods */
/* Allocates memory for the classifier with default parameters */
Classifier * nnc_new(char * output);

/* Changes the parameters. If NULL, the default value is used */
int nnc_set_training_parameters(Classifier * c, double learning_rate,int bipolar, int function_transfer);

/* Receives the data and divides it into training, generalization and validation */
int nnc_set_data(Classifier * c, Data * d, int flag, int percen);

/* Sets the stopping conditions. If NULL, the condition is not used */
int nnc_set_stopping_conditions(Classifier * c, int max_epochs, double max_accuracy, double max_mse);

/* Trains the network until a stopping condition is reached */
int nnc_train_network(Classifier * c);

/*Classifies the data validation and return the success rate*/
double nnc_classifier(Classifier * c,int real_class);

/* Prezi Print */
void nnc_print_info(Classifier * c);

/* Deletes the classifier and the neural network and frees its memory */
int nnc_free(Classifier * c);

/*Sets the neural network*/
int nnc_set_neural_network(Classifier * c, Neural_Network * nn);


/* Private Methods */
void nnc_run_training_epoch(Classifier * c);

double nnc_run_statistics(Classifier * c);



#endif /* CLASSIFIER_H */
