#include <stdlib.h>
#include <stdio.h>
#include "neural_network.h"

/* Public Methods */
Neural_Network * nn_new(int n_layers, int * n_neurons_layer, int * thresholds) {
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
        neuron_init(neurons_arr + i,thresholds[i]);
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

Neural_Network * nn_read_from_file(char * file) {
    Neural_Network * nn;
    FILE * f;
    int n_layers, n_neurons;
    int * n_neurons_layer, *thresholds;
    double * cons;
    int i, j, count;
    
    f = fopen(file, "r");
    if (!f) return NULL;

    /* Number of layers */
    count = fscanf(f, "%d\n", &n_layers);
    if (count == EOF) {
        if (ferror(f)) {
            fclose(f);
            return NULL;
        }
    }
    printf("[ INFO ] Creating network with %d layers\n", n_layers);
    printf("[ INFO ]     Neurons per layer: ");
    /* Neurons per layer */
    n_neurons_layer = (int * ) malloc(n_layers * sizeof(int));
    for (i = 0; i < n_layers; i++) {
        count = fscanf(f, "%d", n_neurons_layer + i);
        if (count == EOF) {
            if (ferror(f)) {
                free(n_neurons_layer);
                fclose(f);
                return NULL;
            }
        }
        printf("%d ", n_neurons_layer[i]);
    }
    printf("\n");
    fscanf(f, "\n");
    /* Total number of neurons */
    n_neurons = 0;
    for (i = 0; i < n_layers; i++) {
        n_neurons += n_neurons_layer[i];
    }
    /* thresholds */
    printf("[ INFO ]     Thresholds: ");
    thresholds = (int * ) malloc(n_neurons * sizeof(int));
    for (i = 0; i < n_neurons; i++) {
        count = fscanf(f, "%d", thresholds + i);
        if (count == EOF) {
            if (ferror(f)) {
                free(thresholds);
                fclose(f);
                return NULL;
            }
        }
        printf("%d ", thresholds[i]);
    }
    printf("\n");



    /* CREATE NETWORK */
    nn = nn_new(n_layers, n_neurons_layer,thresholds);
    free(n_neurons_layer);
    /* Add neurons connections */
    cons = (double * ) malloc(n_neurons * sizeof(double));
    for (i = 0; i < n_neurons; i++) {
        /* Read connections for neuron i */
        for (j = 0; j < n_neurons; j++) {
            count = fscanf(f, "%lf", cons + j);
            if (count == EOF) {
                if (ferror(f)) {
                    free(cons);
                    fclose(f);
                    nn_free(nn);
                    return NULL;
                }
            }
        }
        fscanf(f, "\n");
        nn_connect_neuron(nn, i, cons);
    }
    free(cons);
    fclose(f);
    return nn;
}

void nn_free(Neural_Network * nn) {
    int i;
    /* Free connections */
    for (i = 0; i < nn->n_neurons; i++) {
        free(nn_array(nn)[i].cons);
    }
    
    /* Free neurons */
    free(nn_array(nn));

    /* Free layers */
    free(nn->layers);

    /* Free network */
    free(nn);
}

void print_output(Neural_Network * nn){
    int i;
    for(i = 0; i < nn->layers[nn->n_layers -1].n_neurons ; i++){
        printf("%f\t", nn->layers[nn->n_layers -1].neurons[i].d);
    }
    printf("\n");
}

void nn_update(Neural_Network * nn, double * values, int n_values){
    int i;
    set_entry_neural_network(nn, values, n_values);
    for (i = n_values; i < nn->n_neurons; i++) {
        (*nn->upd_neuron)(&nn_array(nn)[i]);
    }
    nn_keep_value_neurons(nn);   
    print_output(nn); 

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
    n_to->n_cons = n_connections;
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
int nn_keep_value_neurons(Neural_Network * nn){
    int i;
    /* Update neurons */
    for (i = 0; i < nn->n_neurons; i++) {
        if(neuron_update(&nn_array(nn)[i])==-1) return -1;
    }
    return 0;
}



void set_entry_neural_network(Neural_Network * nn, double * values, int n_values){
    int i;
    /* Free connections */
    for (i = 0; i < n_values; i++) {
        nn_array(nn)[i].d_new = values[i];
    }
}


/* Layer Methods */
int layer_init(Neural_Layer * l, int n_neurons, Neuron * first) {
    if (!l) return -1;
    l->n_neurons = n_neurons;
    l->neurons = first;
    return 0;
}

/* Neuron Methods */
int neuron_init(Neuron * n, int threshold) {
    if (!n) return -1;
    n->d = 0.0;
    n->d_new = 0.0;
    n->err = 0.0;
    n->n_cons = 0;
    n->threshold=threshold;
    n->cons = NULL;
    return 0;
}
int neuron_update(Neuron * n) {
    if (!n) return -1;
    n->d = n->d_new;
    n->d_new = 0.0;
    return 0;
}
