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

    /* Default Parameters */
    c->learning_rate = DEF_LEARNING_RATE;
    c->max_epochs = DEF_MAX_EPOCHS;
    c->max_accuracy = DEF_MAX_ACCURACY;
    c->max_mse = DEF_MAX_MSE;

    /* Statistics Inicialization */
    c->accuracy_training = 0.0;
    c->accuracy_generalization = 0.0;
    c->accuracy_validation = 0.0;
    c->mse_training = 0.0;
    c->mse_generalization = 0.0;
    c->mse_validation = 0.0;

    return c;
}

int nnc_set_training_parameters(Classifier * c, double learning_rate) {
    if (!c) return -1;
    
    if (learning_rate) c->learning_rate = learning_rate;

    return 0;
}

int nnc_set_data(Classifier * c, Data * d) {
    if (!c || !data) return -1;

    // TODO Implement
    
    return 0;
}

int nnc_set_stopping_conditions(Classifier * c, int max_epochs, double max_accuracy, double max_mse) {
    if (!c) return -1;

    if (max_epochs)   c->max_epochs = max_epochs;
    if (max_accuracy) c->max_accuracy = max_accuracy;
    if (max_mse)      c->max_mse = max_mse;

    return 0;
}

/* Private Methods */
