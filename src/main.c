#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "neural_network.h"


int main(int argc, char *argv[])
{
    printf("Reading data...\n");
    Data * d = data_from_file("../data/databases/problema_real1.txt");
    data_free(d);

    printf("Creating network\n");
    Neural_Network * nn = nn_read_from_file("../data/models/prueba1.txt");
    printf("Neural network successfully created\n");
    nn_free(nn);
    
    return 0;
}
