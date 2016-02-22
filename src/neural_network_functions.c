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

void upd_neuron_perceptron (Neuron * neuron){
	int i;
	double value = 0;
	double weight;
	for ( i = 0; i < neuron->n_cons; i++){
		weight = neuron->cons[i].weight;
		value += weight * neuron->cons[i].from->d;
	}
	if(value < (-neuron->threshold))
		neuron->d = -1;
	else if (value > neuron->threshold)
		neuron->d = 1;
	else
		neuron->d = 0;
	return;
}

void upd_neuron_adeline (Neuron * neuron){
	int i;
	double value = 0;
	double weight;
	for ( i = 0; i < neuron->n_cons; i++){
		weight = neuron->cons[i].weight;
		value += weight * neuron->cons[i].from->d;
	}
	neuron->d = value;
	return;
}


void upd_weights_perceptron (Neuron * neuron,double alpha, double t){
	int i;
	for ( i = 0; i < neuron->n_cons; i++){
		if(neuron->d != t)
			neuron->cons[i].weight = neuron->cons[i].weight + alpha * t * neuron->cons[i].from->d;
	}
	return;
}

void upd_weights_adeline (Neuron * neuron,double alpha, double t){
	int i;
	for ( i = 0; i < neuron->n_cons; i++){
			neuron->cons[i].weight = neuron->cons[i].weight + alpha * (t - neuron->d) * neuron->cons[i].from->d;
	}
	return;
}