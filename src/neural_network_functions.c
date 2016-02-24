#include <stdlib.h>
#include <stdio.h>
#include "neural_network_functions.h"



void upd_neuron_mcculloch_pitts (Neuron * neuron){
	int i;
	double value = 0;
	double weight;
	Connection * c=connections_neuron_get(*neuron);
	for ( i = 0; i < n_cons_neuron_get(*neuron); i++){
		weight = weight_connecion_get(c[i]);
		if(weight < 0){
			value = 0;
			break;
		}
		Neuron * n = neuron_from_connecion_get(c[i]);
		value += weight * value_neuron_get(*n);
	}
	if(value < threshold_neuron_get(*neuron))
		new_value_neuron_set(neuron,0);
	else
		new_value_neuron_set(neuron,1);
	return;
}

void upd_neuron_perceptron (Neuron * neuron){
	int i;
	double value = 0;
	double weight;

	Connection * c=connections_neuron_get(*neuron);
	for ( i = 0; i < n_cons_neuron_get(*neuron); i++){
		weight = weight_connecion_get(c[i]);
		Neuron * n = neuron_from_connecion_get(c[i]);
		value += weight * value_neuron_get(*n);
		
	}
	double threshold = threshold_neuron_get(*neuron);
	if(value < (-threshold))
		value_neuron_set(neuron,-1);
	else if (value > threshold)
		value_neuron_set(neuron,1);
	else
		value_neuron_set(neuron,0);
	return;
}

void upd_neuron_adeline (Neuron * neuron){
	int i;
	double value = 0;
	double weight;
	Connection * c=connections_neuron_get(*neuron);
	for ( i = 0; i < n_cons_neuron_get(*neuron); i++){
		weight = weight_connecion_get(c[i]);
		Neuron * n = neuron_from_connecion_get(c[i]);
		value += weight * value_neuron_get(*n);
	}
	value_neuron_set(neuron,value);
	return;
}


void upd_weights_perceptron (Neuron * neuron,double alpha, double t){
	int i;
	//t = (t == 0) ? -1 : 1;
	Connection * c=connections_neuron_get(*neuron);
	for ( i = 0; i < n_cons_neuron_get(*neuron); i++){
		if(value_neuron_get(*neuron) != t){
			Neuron * n = neuron_from_connecion_get(c[i]);
			weight_connection_set(&c[i] , weight_connecion_get(c[i]) + alpha * t * value_neuron_get(*n));
		}
	}
	return;
}

void upd_weights_adeline (Neuron * neuron,double alpha, double t){
	int i;
	Connection * c=connections_neuron_get(*neuron);
	for ( i = 0; i < n_cons_neuron_get(*neuron); i++){
			Neuron * n = neuron_from_connecion_get(c[i]);
			weight_connection_set(&c[i] , weight_connecion_get(c[i]) + alpha * (t - value_neuron_get(*neuron)) * value_neuron_get(*n));
	}
	return;
}