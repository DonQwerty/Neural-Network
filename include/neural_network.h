#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H


typedef struct Connection_ {
    double weight;
    struct Neuron_ * from;
} Connection;

typedef struct Neuron_ {
    double d;                      /* Neuron's value */
    double err;                    /* Error, for training purposes */
    int n_cons;                    /* Number of connections (not sure if necessary) */
    Connection * cons;             /* Neuron's connections */
} Neuron;

typedef struct Neural_Layer_ {
    int n_neurons;                 /* Number of neurons in this layer */
    Neuron * neurons;              /* Neurons in this layer */
} Neural_Layer;

typedef struct Neural_Network_ {
    int n_neurons;                 /* Total number of neurons in the net */
    int n_layers;                  /* Number of layers in the network (including in/out) */
    Neural_Layer * layers;         /* Layers array */
} Neural_Network;



/* Public Methods */
/* Allocates memory for the neural network with n layers and
   n_neurons_layer per layer (includind first/last layers) */
Neural_Network * nn_new(int n_layers, int * n_neurons_layer);

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
Neural_Network * nn_read_from_file(char * file);

/* Reads a neural network from stdin, asking for each layer's weights */
int nn_read_from_stdin(Neural_Network * nn);

/* Saves the network to a file, in the format specified previously */
int nn_save_to_file(Neural_Network * nn, char * file);

/* Destroy the network structure and free memory */
void nn_free(Neural_Network * nn);



/* Private Methods */
/* Neural Network Methods */
/* Access the network array of neurons */
/* TODO: Not sure this works */
#define nn_array(nn) (nn)->layers[0].neurons

/* Sets the connections to a neuron from the others */
int nn_connect_neuron(Neural_Network * nn, int n_neuron_to, double * w_neurons_from);

/* Neural_Layer methods */
/* Sets a previously allocated layer variables (number of neurons and array of neurons) */
int layer_init(Neural_Layer * l, int n_neurons, Neuron * first);


/* Neuron methods */
/* Sets the default values for a neuron */
int neuron_init(Neuron * n);


#endif /* NEURAL_NETWORK_H */
