#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "data.h"
#include "neural_network_functions.h"

/* Global definitions, for options processing */
/* Execution modes */
#define MODE_SIMULATE     0
#define MODE_CLASSIFY     1
#define MODE_PRESET       2
/* Presets */
#define PRESET_MCCULLOCH  0

/* Global variables for options processing */
int mode = -1;
int preset = -1;
char * neural_file = NULL;
char * input_file = NULL;
char * output_file = NULL;

/* Stores the values for the options in the propper global variables */
int process_opts(int argc, char *const *argv);

/* Prints the program ussage */
void print_help();


int main(int argc, char *argv[]){

    if (process_opts(argc, argv) == -1) {
        printf("[ ERROR] Error parsing options.\n");
        print_help();
        return -1;
    }

    
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
        nn_update(nn, values, 3, 1);
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


/* Function definitions */
int process_opts(int argc, char *const *argv) {
    int c;

    while (1) {
        static struct option long_options[] =
            {
                {"mode",            required_argument,  0, 'm'},
                {"neural-network",  required_argument,  0, 'n'},
                {"input-file",      required_argument,  0, 'i'},
                {"output-file",     required_argument,  0, 'o'},
                {"preset",          required_argument,  0, 'p'},
                {0, 0, 0, 0}
            };

        int option_index = 0;

        c = getopt_long (argc, argv, "m:n:i:o:p:", long_options, &option_index);

/* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 'm':
            printf("[ INFO ] Mode: %s\n", optarg);
            if (!strcmp(optarg, "simulate")) {
                mode = MODE_SIMULATE;
            } else if (!strcmp(optarg, "classify")) {
                mode = MODE_CLASSIFY;
            } else if (!strcmp(optarg, "preset")) {
                mode = MODE_PRESET;
            } else {
                printf("[ ERROR] Unrecogniced mode: %s\n", optarg);
                return -1;
            }
            break;
        case 'n':
            printf("[ INFO ] Neural Network: %s\n", optarg);
            neural_file = strdup(optarg);
            break;
        case 'i':
            printf("[ INFO ] Input file: %s\n", optarg);
            input_file = strdup(optarg);
            break;
        case 'o':
            printf("[ INFO ] Output file: %s\n", optarg);
            output_file = strdup(optarg);
            break;
        case 'p':
            printf("[ INFO ] Preset: %s\n", optarg);
            mode = MODE_PRESET;
            if (!strcmp(optarg, "mcculloch")) {
                preset = PRESET_MCCULLOCH;
            } else {
                printf("[ ERROR] Unrecogniced preset: %s\n", optarg);
                return -1;
            }
            break;
        case '?':
        default:
            return -1;
        }
    }
    if ( mode == -1) {
        return -1;
    }
    if ( (mode == MODE_PRESET) && (preset == -1)) {
        return -1;
    }
    return 0;
}

void print_help() {
    printf("[ INFO ] Ussage:\n");
    printf("             neural-network -m MODE -n NETWORK -i INPUT -o OUTPUT\n");
    printf("         Options:\n");
    printf("             -m, --mode:           Execution mode [classify, simulate or preset].\n");
    printf("             -n, --neural-network: File with network description.\n");
    printf("             -i, --input-file:     File with the inputs (or data) as rows.\n");
    printf("             -o, --ouput-file:     File to write the output of the network.\n");
    printf("             -p, --preset:         Load a predefined network.\n");
}
