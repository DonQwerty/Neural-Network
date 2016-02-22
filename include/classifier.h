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
#define DEF_LEARNING_RATE          0.2
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
    
    /* Epoch Statistics */
    FILE * file_statistics;
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
//      If not, it cannot allocate memory for the network
Classifier * nnc_new();

/* Changes the parameters. If NULL, the default value is used */
int nnc_set_training_parameters(Classifier * c, double learning_rate,int bipolar);

/* Receives the data and divides it into training, generalization and validation */
// TODO What does this receive? The file? The Data *?
int nnc_set_data(Classifier * c, Data * d);

/* Sets the stopping conditions. If NULL, the condition is not used */
int nnc_set_stopping_conditions(Classifier * c, int max_epochs, double max_accuracy, double max_mse);

/* Trains the network until a stopping condition is reached */
int nnc_train_network(Classifier * c);

/* Deletes the classifier and the neural network and frees its memory */
int nnc_free(Classifier * c);

/*Sets the neural network*/
int nnc_set_neural_network(Classifier * c, Neural_Network * nn);


/* Private Methods */
void nnc_run_training_epoch(Classifier * c);

void nnc_run_statistics(Classifier * c, int epoch);



#endif /* CLASSIFIER_H */
