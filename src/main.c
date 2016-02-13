#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "neural_network_functions.h"


int main(int argc, char *argv[]){
    double entrada1[3]={1,0,0};
    double entrada2[3]={0,1,0};
    double entrada3[3]={0,0,1};
    printf("Reading data...\n");
    Data * d = data_from_file("../data/databases/problema_real1.txt");
    data_free(d);

    printf("Creating network\n");
    Neural_Network * nn = nn_read_from_file("../data/models/McCulloch-Pitts.txt");

    nn->upd_neuron = upd_neuron_mcculloch_pitts;

    nn_update(nn, entrada1, 3); 
    nn_update(nn, entrada2, 3); 
    nn_update(nn, entrada3, 3);
    nn_update(nn, entrada2, 3);
    nn_update(nn, entrada2, 3);
    nn_update(nn, entrada3, 3);
    nn_update(nn, entrada1, 3);   
    nn_update(nn, entrada1, 3);
    nn_update(nn, entrada1, 3);
    nn_update(nn, entrada2, 3);
    

    
    if (nn) {
        printf("Neural network successfully created\n");
        //print_output(nn);
        nn_free(nn);
    } else {
        printf("Error reading from file\n");
    }

    
    return 0;
}
