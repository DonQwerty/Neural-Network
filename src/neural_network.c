#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "neural_network.h"


/* Public Methods */
Neural_Network * nn_new(int n_layers, int * n_neurons_layer, double * thresholds) {
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
        layer_init(&(nn->layers[i]), n_neurons_layer[i], neurons_arr + offset);
        offset += n_neurons_layer[i];
    }
    nn->array_mean = NULL;
    nn->array_desv = NULL;

    return nn;
}

Neural_Network * nn_init(int n_attrs, int n_clas, int n_layers, int bipolar , int sesg, int mode, int hidden_neurons) {
    Neural_Network * nn;
    int  n_neurons;
    int * n_neurons_layer;
    double * thresholds;
    double * cons, * cons_empty;
    int i, j, k;
    double r;
    srand(time(NULL));
    //TODO V 2.0 Create different layers
    n_neurons_layer = (int * ) malloc(n_layers * sizeof(int));
    // for (i = 0; i < n_layers; i++) {
    //  n_neurons_layer[i];
    // printf("%d ", n_neurons_layer[i]);
    // }
    n_neurons_layer[0] = n_attrs+sesg;
    if(n_layers > 2){
        n_neurons_layer[1] = hidden_neurons + sesg;
        n_neurons_layer[2] = n_clas;
    }
    else{
        n_neurons_layer[1] = (mode ==  1) ? 1 : n_clas;
    }



    n_neurons = 0;
    for (i = 0; i < n_layers; i++) {
        n_neurons += n_neurons_layer[i];
    }
    /* thresholds */
    // printf("[ INFO ]     Thresholds: ");
    thresholds = (double * ) malloc(n_neurons * sizeof(double));
    for (i = 0; i < n_neurons; i++) {
        thresholds[i] = 0;
    }
    // printf("\n");
    /* CREATE NETWORK */
    nn = nn_new(n_layers, n_neurons_layer,thresholds);
    free(n_neurons_layer);
    /* Add neurons connections */
    cons = (double * ) malloc(n_neurons * sizeof(double));
    cons_empty = (double * ) malloc(n_neurons * sizeof(double));

    for(j = 0; j < n_neurons; j++){
        cons_empty[j] = 0;
    }

    Neural_Layer * layers = nn_get_layers(nn);
    int n = 0;
    for (i = 0 ; i < n_layers ; i++){

        for(j = 0; j < n_neurons; j++){
            cons[j] = 0;
        }
        for(j = 0 ; j < layer_get_n_neurons(layers + i) ; j++){
            if( i != 0){
                for (k = 0 ; k < layer_get_n_neurons(layers +i -1) ; k++){
                    r = ((double)rand())/RAND_MAX-0.5;
                    while(r==0){
                        r = ((double)rand())/RAND_MAX-0.5;
                    }
                    cons[k+n] = r;
                }
                if(j == (layer_get_n_neurons(layers + i)-1) && i != (n_layers-1)){
                    nn_connect_neuron(nn, j+n+layer_get_n_neurons(layers +i -1), cons_empty);
                    nn_array(nn)[j+n+layer_get_n_neurons(layers +i -1)].d = 1;
                }
                else
                    nn_connect_neuron(nn, j+n+layer_get_n_neurons(layers +i -1), cons);
            }
            else
                nn_connect_neuron(nn, j, cons_empty);
        }
        if(i!=0) n+= layer_get_n_neurons(layers +i -1);
    }
    free(thresholds);
    free(cons);
    free(cons_empty);
    return nn;
}

int nn_save_to_file(Neural_Network * nn, const char * file, int n_attrs) {
    FILE * f;
    Neuron * cur_neuron;
    int n_neurons, n_cur_neuron;
    int i, j;

    f = fopen(file, "w");
    if (!f) return -1;

    fprintf(f, "%d\n", nn->n_layers);
    for(i = 0 ; i < nn->n_layers ; i++){
        fprintf(f, "%d ", nn->layers[i].n_neurons);
    }
    n_neurons = nn->n_neurons;
    fprintf(f, "\n%lf", nn_array(nn)[0].threshold);
    for(i = 1; i < n_neurons ; i++){
        fprintf(f, " %lf", nn_array(nn)[i].threshold);
    }
    fprintf(f, "\n");
    for (n_cur_neuron = 0; n_cur_neuron < n_neurons; n_cur_neuron++) {
        cur_neuron = &(nn_array(nn)[n_cur_neuron]);

        if (cur_neuron->n_cons == n_neurons) {
            /* This neuron has all connections. Write directly */
            for (i = 0; i < n_neurons; i++) {
                fprintf(f, "%lf ", cur_neuron->cons[i].weight);

            }
        } else {
            /* This neuron does not have all connections */

            for (i = 0; i < n_neurons; i++) {
                for (j = 0 ; j < cur_neuron->n_cons ; j++){
                    if (&(nn_array(nn)[i]) == cur_neuron->cons[j].from) {
                        fprintf(f, "%lf ", cur_neuron->cons[j].weight);
                        break;
                    }
                }
                if(j == cur_neuron->n_cons)
                    fprintf(f, "0.0 ");

            }

        }

        fprintf(f, "\n");
    }
    if(!nn->array_mean){
        fprintf(f, "0\n");
    }
    else{
        fprintf(f, "%d\n",n_attrs);
        for (i = 0; i < n_attrs; i++){
            fprintf(f, "%lf ", nn->array_mean[i]);
        }
        fprintf(f, "\n");
        for (i = 0; i < n_attrs; i++){
            fprintf(f, "%lf ", nn->array_desv[i]);
        }
    }
    fclose(f);

    return 0;
}

Neural_Network * nn_read_from_file(const char * file) {
    Neural_Network * nn;
    FILE * f;
    int n_layers, n_neurons;
    int * n_neurons_layer;
    double * thresholds;
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
// printf("[ INFO ] Creating network with %d layers\n", n_layers);
// printf("[ INFO ]     Neurons per layer: ");
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
        // printf("%d ", n_neurons_layer[i]);
    }
// printf("\n");
    fscanf(f, "\n");
/* Total number of neurons */
    n_neurons = 0;
    for (i = 0; i < n_layers; i++) {
        n_neurons += n_neurons_layer[i];
    }
/* thresholds */
// printf("[ INFO ]     Thresholds: ");
    thresholds = (double * ) malloc(n_neurons * sizeof(double));
    for (i = 0; i < n_neurons; i++) {
        count = fscanf(f, "%lf", thresholds + i);
        if (count == EOF) {
            if (ferror(f)) {
                free(thresholds);
                fclose(f);
                return NULL;
            }
        }
        // printf("%lf ", thresholds[i]);
    }
// printf("\n");



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
    fscanf(f, "%d", &count);
    if(count != 0){
        nn->array_mean = (double *)malloc(sizeof(double)*count);
        nn->array_desv = (double *)malloc(sizeof(double)*count);
        for (i = 0; i < count; i++) {
            fscanf(f, "%lf", nn->array_mean + i);
        }
        fscanf(f, "\n");
        for (i = 0; i < count; i++) {
            fscanf(f, "%lf", nn->array_desv + i);
        }
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
    if(nn->array_mean)
        free(nn->array_mean);
    if(nn->array_desv)
        free(nn->array_desv);
    free(nn);
}

void fprint_output(Neural_Network * nn, FILE * f){
    int i;
    for(i = 0; i < nn->layers[nn->n_layers -1].n_neurons ; i++){
        fprintf(f, "%f\t", nn->layers[nn->n_layers -1].neurons[i].d);
    }
    fprintf(f, "\n");
}
void fprint_neurons(Neural_Network * nn, FILE * f){
    int i;
    for(i = 0; i < nn->n_neurons ; i++){
        fprintf(f, "%f\t", nn_array(nn)[i].d);
    }
    fprintf(f, "\n");
}

void fprint_w(Neural_Network * nn, FILE * f){
    int i,j;
    for(i = 0; i < nn->n_neurons ; i++){
        for (j = 0; j< nn_array(nn)[i].n_cons; j++){
            fprintf(f, "%lf;", nn_array(nn)[i].cons[j].weight);
        }
    }

    fprintf(f, "\n");
}


void nn_update_neurons(Neural_Network * nn, double * values, int n_values, int discrete, int sesg){
    int i;
    set_entry_neural_network(nn, values, n_values, discrete,  sesg);
    for (i = n_values + sesg; i < nn->n_neurons; i++) {
        (*nn->upd_neuron)(&nn_array(nn)[i]);
        // Set error = 0 in case of backpropagation
        nn_array(nn)[i].err = 0;
    }
    if(discrete)
        nn_keep_value_neurons(nn);
}

void nn_compute_out_err(Neural_Network * nn, double * values) {
    Neural_Layer * output;
    Neuron * neurons;
    Neuron * cur;
    int i;

    output = nn_get_layers(nn) + nn_get_n_layers(nn) - 1;
    neurons = layer_get_neurons(output);
    for (i = 0; i < layer_get_n_neurons(output); i++) {
        cur = neurons + i;
        neuron_set_err(cur, values[i] - neuron_get_value(cur));
    }

}

void neuron_set_err(Neuron * n, double err) {
    n->err = err;
}

void nn_update_weights(Neural_Network * nn, double alpha, double * t){
    int i,j=0;
    for (i = nn->n_neurons -1; i >= 0; i--) {
        (*nn->upd_weight)(&nn_array(nn)[i], alpha , t[j]);
        j++;
    }
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

double neuron_get_err(Neuron * n) {
    return n->err;
}

double neuron_get_d_in(Neuron * n) {
    return n->d_in;
}

void set_entry_neural_network(Neural_Network * nn, double * values, int n_values, int discrete, int sesg){
    int i;
    /* Init values */
    for (i = 0; i < n_values; i++) {
        if(discrete)
            nn_array(nn)[i].d_new = values[i];
        else
            nn_array(nn)[i].d = values[i];
    }
    if(sesg) nn_array(nn)[i].d = 1;
}


/* Layer Methods */
int layer_init(Neural_Layer * l, int n_neurons, Neuron * first) {
    if (!l) return -1;
    l->n_neurons = n_neurons;
    l->neurons = first;
    return 0;
}

/* Neuron Methods */
int neuron_init(Neuron * n, double threshold) {
    if (!n) return -1;
    n->d = 1.0;
    n->d_in = 0.0;
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

double * nn_get_output(Neural_Network * nn){
    int i;
    int n = nn->layers[nn->n_layers-1].n_neurons;
    double * values = (double *) malloc(n * sizeof(double));

    if (!values) return NULL;

    for( i = 0 ; i< n ; i++ ){
        values[i] = nn->layers[nn->n_layers-1].neurons[i].d;

    }

    return values;
}

/*Getters */

double * nn_get_array_means(Neural_Network * n){
    return n->array_mean;
}

double * nn_get_array_desv(Neural_Network * n){
    return n->array_desv;
}
int nn_get_n_neurons(Neural_Network * n){
    return n->n_neurons;
}
int nn_get_n_layers(Neural_Network * n){
    return n->n_layers;
}

Neural_Layer * nn_get_layers(Neural_Network * n){
    return n->layers;
}

int layer_get_n_neurons(Neural_Layer * nl){
    return nl->n_neurons;
}
Neuron * layer_get_neurons(Neural_Layer * nl){
    return nl->neurons;
}

double neuron_get_value(Neuron * n){
    return n->d;
}

double neuron_get_new_value(Neuron * n){
    return n->d_new;
}
double neuron_get_value_in(Neuron * n){
    return n->d_in;
}
int neuron_get_n_cons(Neuron * n){
    return n->n_cons;
}

double neuron_get_threshold(Neuron * n){
    return n->threshold;
}

Connection * neuron_get_connections(Neuron * n){
    return n->cons;
}

double connection_get_weight(Connection * c){
    return c->weight;
}

Neuron *  connection_get_neuron_from(Connection * c){
    return c->from;
}

double connection_get_delta(Connection * c){
    return c->delta;
}



/*Setters*/
void value_neuron_set(Neuron * n, double v){
    n->d = v;
}

void new_value_neuron_set(Neuron * n, double v){
    n->d_new = v;
}
void weight_connection_set(Connection * c, double weight){
    c->weight = weight;
}

void connection_set_delta(Connection * c, double delta){
    c->delta = delta;
}


void nn_set_function_weight(Neural_Network * nn, nn_upd_weight upd){
    nn->upd_weight = upd;
}
void nn_set_function_neuron(Neural_Network * nn, nn_upd_neuron upd){
    nn->upd_neuron = upd;

}

void nn_set_array_mean(Neural_Network * nn, double *means){
    nn->array_mean = means;
}
void nn_set_array_desv(Neural_Network * nn, double *desv){
    nn->array_desv = desv;
}

double neuron_get_input(Neuron * neuron) {
    int i;
    double value, weight;
    Connection * c;

    value = 0;
    c = neuron_get_connections(neuron);
    for ( i = 0; i < neuron_get_n_cons(neuron); i++){
        weight = connection_get_weight(c + i);
        Neuron * n = connection_get_neuron_from(c + i);
        value += weight * neuron_get_value(n);
    }
    if(neuron_get_n_cons(neuron)==0){
        value = 1;
        neuron->d_in = 1;
    }
    else
        neuron->d_in = value;
    return value;
}
