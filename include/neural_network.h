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
    int n_layers;                  /* Number of layers in the network (including in/out) */
    Neural_Layer * layers;         /* Layers array */
} Neural_Network;





// OLD

typedef struct Neural_Layer_ {
    int n_perceptrons;             /* Number of perceptrons in this layer */
    int n_perceptrons_t;           /* Number of perceptrons in the whole network*/
    int ** weights;                /* n_perceptrons * n_perceptrons_total */
} Neural_Layer;

typedef struct Neural_Network_ {
    int n_layers;
    int n_perceptrons;
    Neural_Layer ** layers;
} Neural_Network;


/* Public Methods */
/* Allocates memory for the neural network */
Neural_Network * nn_new();

/* Reads a neural network from a file.
   Supposing we have the following network with 3 layers:
   o
   o <   \
   o - o
   o <   /
   o
   Assuming all weights equal to 1, the file to represent it should be as follows:
   2 3 1                       # Number of perceptrons for each layer
   0.0 0.0 1.0 1.0 0.0 0.0     # First perceptron connections (with weights)
   0.0 0.0 0.0 1.0 1.0 0.0     # Second perceptron connections
   0.0 0.0 0.0 0.0 0.0 1.0     # (...)
   0.0 0.0 0.0 0.0 0.0 1.0
   0.0 0.0 0.0 0.0 0.0 1.0

*/
int nn_read_from_file(Neural_Network * nn, char * file);

/* Reads a neural network from stdin, asking for each layer's weights */
int nn_read_from_stdin(Neural_Network * nn);



/* Private Methods */








#endif /* NEURAL_NETWORK_H */
