#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "neural_network.h"

/* Default Values */
#define DEF_TRAINIG_PERCENT        60.0
#define DEF_GENERALIZATION_PERCENT 20.0
#define DEF_VALIDATION_PERCENT     20.0
#define DEF_LEARNING_RATE          0.2
#define DEF_MAX_EPOCHS             100

typedef struct Classifier_ {
    
    /* Classifier Variables */
    Neural_Network * nn;
    Data           * data_training;
    Data           * data_generalization;
    Data           * data_validation;
    
    /* Classifier Parameters */
    double learning_rate;
    int    epoch;
    int    max_epochs;
    
    /* Epoch Statistics */
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
// TODO Does this receive the number of classes and attributes?
Classifier * nnc_new();

/* Changes the parameters. If NULL, the default value is used */
int nnc_set_training_parameters(double learning_rate, int max_epochs);

// TODO What does this receive? The file? The Data *?
int nnc_set_data();

/* Sets the stopping conditions */

/* Private Methods */













#endif /* CLASSIFIER_H */
