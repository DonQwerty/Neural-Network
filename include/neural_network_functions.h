#ifndef NEURAL_NETWORK_FUNCTIONS_H
#define NEURAL_NETWORK_FUNCTIONS_H

#include "neural_network.h"

void upd_neuron_mcculloch_pitts (Neuron * neuron);
void upd_weights_perceptron (Neuron * neuron,double alpha, double t);
void upd_weights_adeline (Neuron * neuron,double alpha, double t);


#endif /* NEURAL_NETWORK_FUNCTIONS_H */
