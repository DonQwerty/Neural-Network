#include <stdlib.h>
#include <stdio.h>
#include "neural_network_functions.h"



void upd_neuron_mcculloch_pitts (Neuron * neuron){
	int i;
	double value = 0;
	double weight;
	for ( i = 0; i < neuron->n_cons; i++){
		weight = neuron->cons[i].weight;
		if(weight < 0){
			value = 0;
			break;
		}
		value += weight * neuron->cons[i].from->d;
	}
	if(value < neuron->threshold)
		neuron->d_new = 0;
	else
		neuron->d_new = 1;
	return;
}


void upd_weights_perceptron (Neuron * neuron){
	int i;
	double value = 0;
	double weight;
	for ( i = 0; i < neuron->n_cons; i++){
		weight = neuron->cons[i].weight;
		if(weight < 0){
			value = 0;
			break;
		}
		value += weight * neuron->cons[i].from->d;
	}
	if(value < neuron->threshold)
		neuron->d_new = 0;
	else
		neuron->d_new = 1;
	return;
}