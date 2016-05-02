#ifndef NEURAL_NETWORK_FUNCTIONS_H
#define NEURAL_NETWORK_FUNCTIONS_H

#include "neural_network.h"

void upd_neuron_mcculloch_pitts (Neuron * neuron);
void upd_neuron_perceptron (Neuron * neuron);
void upd_neuron_adeline (Neuron * neuron);
void upd_neuron_sigmoid (Neuron * neuron);
void upd_neuron_linear (Neuron * neuron);

void upd_weights_perceptron (Neuron * neuron,double alpha, double t);
void upd_weights_adeline (Neuron * neuron,double alpha, double t);
void upd_weights_sigmoid (Neuron * neuron, double alpha, double t);
void upd_weights_linear (Neuron * neuron, double alpha, double t);


/* Helper Functions */
double sigmoid(double x);
double sigmoid_derived(double x);

#endif /* NEURAL_NETWORK_FUNCTIONS_H */
