#include <stdlib.h>
#include <stdio.h>
#include "neural_network.h"

/* Public Methods */
Neural_Network * nn_new() {
  Neural_Network * nn = (Neural_Network *) malloc(sizeof(Neural_Network));
  if (!nn) return NULL;
  nn->n_layers = 0;
//  nn->n_perceptrons = 0;
  nn->layers = NULL;
  return nn;
}
