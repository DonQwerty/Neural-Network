#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "neural_network_functions.h"


int main(int argc, char *argv[]){
    
    //printf("Reading data...\n");
    //Data * d = data_from_file("../data/databases/problema_real1.txt");
    //data_free(d);

    /*Ejercicio 1*/
    /*TODO Poner esto prezi, el formato de entrada no es el formato de data.h por eso esto es a mano*/
    printf("Creating network\n");
    Neural_Network * nn = nn_read_from_file("../data/models/McCulloch-Pitts.txt");
    nn->upd_neuron = upd_neuron_mcculloch_pitts;
    double * values = (double*)malloc(3*sizeof(double));
    FILE * f =fopen("../data/databases/McCulloch-Pitts.txt","r");
    char text[20];
    char * ptr;
    int j;
    while(fgets(text,20,f)){
        ptr = strtok(text,SEP);
        values[0] = atof(ptr);
        for(j=1 ; j < 3; j++){
            ptr = strtok(NULL,SEP);
            values[j] = atof(ptr); 
        }
        nn_update_discrete(nn, values, 3);
    }
    


    
    if (nn) {
        printf("Neural network successfully created\n");
        //print_output(nn);
        nn_free(nn);
    } else {
        printf("Error reading from file\n");
    }

    
    return 0;
}
