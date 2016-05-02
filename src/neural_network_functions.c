#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "neural_network_functions.h"


void upd_neuron_mcculloch_pitts (Neuron * neuron){
    int i;
    double value = 0;
    double weight;
    Connection * c = neuron_get_connections(neuron);
    for ( i = 0; i < neuron_get_n_cons(neuron); i++){
        weight = connection_get_weight(c + i);
        if(weight < 0){
            value = 0;
            break;
        }
        Neuron * n = connection_get_neuron_from(c + i);
        value += weight * neuron_get_value(n);
    }
    if(value < neuron_get_threshold(neuron))
        new_value_neuron_set(neuron,0);
    else
        new_value_neuron_set(neuron,1);
    return;
}

void upd_neuron_perceptron (Neuron * neuron){
    double value;
    double threshold;

    value = neuron_get_input(neuron);
    threshold = neuron_get_threshold(neuron);

    if(value < (-threshold))
        value_neuron_set(neuron,-1);
    else if (value > threshold)
        value_neuron_set(neuron,1);
    else
        value_neuron_set(neuron,0);
    return;
}

void upd_neuron_adeline (Neuron * neuron){
    double value = 0;

    value = neuron_get_input(neuron);
    value_neuron_set(neuron,value);
    return;
}

void upd_neuron_sigmoid (Neuron * neuron) {
    double value=0;

    value = neuron_get_input(neuron);
    if(value != 1)
        value_neuron_set(neuron, sigmoid(value));
    return;
}

void upd_neuron_linear (Neuron * neuron) {
    value_neuron_set(neuron, neuron_get_input(neuron));
}


void upd_weights_perceptron (Neuron * neuron, double alpha, double t){
    int i;
    //t = (t == 0) ? -1 : 1;
    Connection * c=neuron_get_connections(neuron);
    for ( i = 0; i < neuron_get_n_cons(neuron); i++){
        if(neuron_get_value(neuron) != t){
            Neuron * n = connection_get_neuron_from(c + i);
            weight_connection_set(&c[i] , connection_get_weight(c + i) + alpha * t * neuron_get_value(n));
        }
    }
    return;
}

void upd_weights_adeline (Neuron * neuron,double alpha, double t){
    int i;
    Connection * c=neuron_get_connections(neuron);
    for ( i = 0; i < neuron_get_n_cons(neuron); i++){
            Neuron * n = connection_get_neuron_from(c + i);
            weight_connection_set(&c[i] , connection_get_weight(c + i) + alpha * (t - neuron_get_value(neuron)) * neuron_get_value(n));
    }
    return;
}

void upd_weights_sigmoid (Neuron * neuron, double alpha, double t) {
    Neuron * from;
    Connection * cons;
    int i;
    double err;

    /* Calculo el error */
    err = neuron_get_err(neuron) * sigmoid_derived(neuron_get_d_in(neuron));
    neuron_set_err(neuron, err);

    cons = neuron_get_connections(neuron);
    for (i = 0; i < neuron_get_n_cons(neuron); i++) {
        /* Retropropagación de error */
        from = connection_get_neuron_from(cons + i);

        neuron_set_err(from,
                       neuron_get_err(from) +
                       (connection_get_weight(cons + i) * err));

        /* Actualización de pesos */
        weight_connection_set(cons + i,
                              connection_get_weight(cons + i) +
                              (alpha * err * neuron_get_value(from)));
    }

}

void upd_weights_linear (Neuron * neuron, double alpha, double t) {
	Neuron * from;
    Connection * cons;
    int i;
    double err;

    /* Calculo el error */
    err = neuron_get_err(neuron) ;
    

    cons = neuron_get_connections(neuron);
    for (i = 0; i < neuron_get_n_cons(neuron); i++) {
        /* Retropropagación de error */
        from = connection_get_neuron_from(cons + i);

        neuron_set_err(from,
                       neuron_get_err(from) +
                       (connection_get_weight(cons + i) * err));

        /* Actualización de pesos */
        weight_connection_set(cons + i,
                              connection_get_weight(cons + i) +
                              (alpha * err * neuron_get_value(from)));
    }

}


/* Helper Functions */
double sigmoid(double x) {
    return (2 / (1 + exp(-x))) - 1;
}

double sigmoid_derived(double x) {
    double sig = sigmoid(x);
    return 0.5 * (1 + sig) * (1 - sig);
}
