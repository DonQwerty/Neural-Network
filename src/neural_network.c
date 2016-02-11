#include <stdio.h>
#include "../include/neural_network.h"

/* Public Methods */
Neural_Network * nn_new() {
  Neural_Network * nn = (Neural_Network *) malloc(sizeof(Neural_Network));
  if (!nn) return null;
  nn->n_layers = 0;
  nn->n_perceptrons = 0;
  nn->layers = null;
  return nn;
}

