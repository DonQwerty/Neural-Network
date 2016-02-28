#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H


/* Structures definitions */
typedef struct Connection_ {
    double weight;
    struct Neuron_ * from;
} Connection;

typedef struct Neuron_ {
    double d;
    double d_new;                  /* Neuron's value */
    double err;                    /* Error, for training purposes */
    int n_cons;                    /* Number of connections */
    double threshold;                 
    Connection * cons;             /* Neuron's connections */
} Neuron;

/* Functions definition */
typedef void (*nn_upd_neuron)(Neuron *);
typedef void (*nn_upd_weight)(Neuron *, double , double);

typedef struct Neural_Layer_ {
    int n_neurons;                 /* Number of neurons in this layer */
    Neuron * neurons;              /* Neurons in this layer */
} Neural_Layer;

typedef struct Neural_Network_ {
    int n_neurons;                 /* Total number of neurons in the net */
    nn_upd_neuron upd_neuron;      /* Function to compute the new value of a neuron */
    nn_upd_weight upd_weight;      /* Function to compute the new weight of a connection */
    int n_layers;                  /* Number of layers in the network (including in/out) */
    Neural_Layer * layers;         /* Layers array */
} Neural_Network;


/* Public Methods */
/* Allocates memory for the neural network with n layers and
   n_neurons_layer per layer (includind first/last layers) */
Neural_Network * nn_new(int n_layers, int * n_neurons_layer, double * thresholds);

/* Reads a neural network from a file.
   Supposing we have the following network with 3 layers:
       o
   o <   \
       o - o
   o <   /
       o
   Assuming all weights equal to 1, the file to represent it should be as follows:
   3                           # Number of layers
   2 3 1                       # Number of perceptrons for each layer
   0.0 0.0 0.0 0.0 0.0 0.0     # First perceptron connections (with weights)
   0.0 0.0 0.0 0.0 0.0 0.0     # Second perceptron connections
   1.0 0.0 0.0 0.0 0.0 0.0     # (...)
   1.0 1.0 0.0 0.0 0.0 0.0
   0.0 1.0 0.0 0.0 0.0 0.0
   0.0 0.0 1.0 1.0 1.0 0.0
   
*/
Neural_Network * nn_read_from_file(const char * file);

Neural_Network * nn_init(int n_attrs, int n_clas, int n_layers, int bipolar, int sesg);

/* Reads a neural network from stdin, asking for each layer's weights */
int nn_read_from_stdin(Neural_Network * nn);

/* Saves the network to a file, in the format specified previously */
int nn_save_to_file(Neural_Network * nn, const char * file) ;

/* Destroy the network structure and free memory */
void nn_free(Neural_Network * nn);

/* Keep the values for a neural network */
int nn_keep_value_neurons(Neural_Network * nn);

/*Prints the last layer of the neural network*/
void fprint_output(Neural_Network * nn, FILE * f);

void fprint_w(Neural_Network * nn, FILE * f);

/*Set a entry for the neural network*/
void set_entry_neural_network(Neural_Network * nn, double * values, int n_values,int discrete, int sesg);

/*Updates the neural network*/
void nn_update_neurons(Neural_Network * nn, double * values, int n_values, int discrete, int sesg);

/*Updates the neural network weights*/
void nn_update_weights(Neural_Network * nn, double alpha, double * t);



/* Private Methods */
/* Neural Network Methods */
/* Access the network array of neurons */
#define nn_array(nn) (nn)->layers[0].neurons

/* Sets the connections to a neuron from the others */
int nn_connect_neuron(Neural_Network * nn, int n_neuron_to, double * w_neurons_from);

/* Neural_Layer methods */
/* Sets a previously allocated layer variables (number of neurons and array of neurons) */
int layer_init(Neural_Layer * l, int n_neurons, Neuron * first);


/* Neuron methods */
/* Sets the default values for a neuron */
int neuron_init(Neuron * n,double threshold);
/* Updates the values for a neuron */
int neuron_update(Neuron * n);

/*Return the last layer of the neural network*/
double * nn_get_output(Neural_Network nn);

/*Getters */
int n_neurons_nn_get(Neural_Network n);
int n_layers_get(Neural_Network n);
Neural_Layer * layers_get(Neural_Network n);

int n_neurons_layer_get(Neural_Layer nl);
Neuron * neurons_layer_get(Neural_Layer nl);

double value_neuron_get(Neuron n);
double new_value_neuron_get(Neuron n);
int n_cons_neuron_get(Neuron n);
double threshold_neuron_get(Neuron n);
Connection * connections_neuron_get(Neuron n);

double weight_connecion_get(Connection  c);
Neuron *  neuron_from_connecion_get(Connection  c);


/*Setters*/

void value_neuron_set(Neuron * n, double v);
void new_value_neuron_set(Neuron * n, double v);

void weight_connection_set(Connection * c, double weight);

void nn_set_function_weight(Neural_Network * nn, nn_upd_weight upd);
void nn_set_function_neuron(Neural_Network * nn, nn_upd_neuron upd);

#endif /* NEURAL_NETWORK_H */
