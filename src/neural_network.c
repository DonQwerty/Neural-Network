#include <stdlib.h>
#include <stdio.h>
#include "neural_network.h"

/* Public Methods */
Neural_Network * nn_new(int n_layers, int * n_nerons_layer) {
    Neural_Network * nn;
    Neuron * neurons_arr;
    int n_neurons_total;
    int i, offset;

    /* Compute the total number of neurons */
    n_neurons_total = 0;
    for (i = 0; i < n_layers; i++) {
        n_neurons_total += n_neurons_layer[i];
    }

    /* Allocates memory for the neurons */
    neurons_arr = (Neuron *) malloc(n_neurons_total * sizeof(Neuron));
    if (!neurons_arr) return NULL;
    /* Initialize neurons */
    for (i = 0; i < n_neurons_total; i++) {
        neuron_init(neurons_arr + i);
    }
    
    /* Allocates memory for the network */
    nn = (Neural_Network *) malloc(sizeof(Neural_Network));
    if (!nn) {
        free(neurons_arr);
        return NULL;
    }

    /* Allocates memory for the layers */
    nn->layers = (Neural_Layer *) malloc(n_layers * sizeof(Neural_Layer));
    if (!nn->layers) {
        free(nn);
        free(neurons_arr);
        return NULL;
    }

    /* Set the network properties */
    nn->n_neurons = n_neurons_total;
    nn->n_layers = n_layers;
    offset = 0;
    for (i = 0; i < n_layers; i++) {
        /* TODO: Check pointers */
        layer_init(&(nn->layers[i]), n_neurons_layer[i], neurons_arr + offset);
        offset += n_neurons_layer[i];
    }
    
    return nn;
}



/* Private Methods */
/* Neural Network Methods */
int nn_connect_neuron(Neural_Network * nn, int n_neuron_to, double * w_neurons_from) {
    Neuron * n_to;
    int n_connections;
    int i;
    
    if (!nn | !w_neurons_from) return -1;

    /* Get total number of connections */
    n_connections = 0;
    for (i = 0; i < nn->n_neurons; i++) {
        if (w_neurons_from[i] != 0.0) {
            n_connections++;
        }
    }
    if (n_connections == 0) return 0;

    /* Allocate memory for the connections */
    n_to = &(nn_array(nn)[n_neuron_to]);
    n_to->cons = (Connection *) malloc(n_connections * sizeof(Connection));
    if (!(n_to->cons)) return -1;

    /* Stablish the connections */
    n_connections = 0;
    for (i = 0; i < nn->n_neurons; i++) {
        if (w_neurons_from[i] != 0.0) {
            n_to->cons[n_connections].weight = w_neurons_from[i];
            n_to->cons[n_connections].from = &(nn_array(nn)[i]);
            n_connections++;
        }
    }

    return 1;
}


/* Layer Methods */
int layer_init(Neural_Layer * l, int n_neurons, Neuron * first) {
    if (!l) return -1;
    l->n_neurons = n_neurons;
    l->neurons = first;
    return 0;
}

/* Neuron Methods */
int neuron_init(Neuron * n) {
    if (!n) return -1;
    n->d = 0.0;
    n->err = 0.0;
    n->n_cons = 0;
    n->cons = NULL;
    return 0;
}
