#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "data.h"
#include "classifier.h"
#include "neural_network_functions.h"

/* Global definitions, for options processing */
/* Execution modes */
#define MODE_SIMULATE     0
#define MODE_CLASSIFY     1
#define MODE_PRESET       2
/* Presets */
#define PRESET_MCCULLOCH  0

/* Preset files */
static const char P_MCCULLOCH_NEURAL[]      = "data/models/McCulloch-Pitts.txt";
static const char P_MCCULLOCH_INPUT[]       = "data/databases/McCulloch-Pitts.txt";
static const char P_MCCULLOCH_OUTPUT[]      = "out/McCulloch-Pitts.txt";


/* Global variables for options processing */
int mode = -1;
int preset = -1;
char * neural_file = NULL;
char * input_file  = NULL;
char * output_file = NULL;

/* Stores the values for the options in the propper global variables */
int process_opts(int argc, char *const *argv);

/* Prints the program ussage */
void print_help();


int main(int argc, char *argv[]){

    Neural_Network * nn;
    Data * data;
    FILE * f_in;
    FILE * f_out;
	Classifier * nnc;
    double * values;
    char text[20];
    char * ptr;
    int j;
    int n_attrs, n_clases;
    if (process_opts(argc, argv) == -1) {
        printf("[ ERROR] Error parsing options.\n");
        print_help();
        return -1;
    }

    switch (mode) {
    case MODE_SIMULATE:
        printf("Mode not implemented.\n");
        return 0;
    case MODE_CLASSIFY:
        /*TODO leer la red neuronal y crear clasificDOR*/
        data = data_from_file(input_file,1);
		n_attrs = sample_get_n_attrs(*(data_get_samples(*data)[0]));
		n_clases = data_get_n_classes(*data);
		nn = nn_read_from_file("data/models/prueba2.txt");
		if (!nn) {
                printf("[ ERROR] Error reading neural network file.\n");
                return -1;
            }
		nn_set_function_neuron(nn , upd_neuron_adeline);
		//nn_set_function_neuron(nn , upd_neuron_perceptron);
		nn_set_function_weight(nn , upd_weights_adeline);
		//nn_set_function_weight(nn , upd_weights_perceptron);
		fflush(stdout);
		nnc = nnc_new();
		nnc_set_data(nnc, data);
		nnc_set_neural_network(nnc , nn);
		nnc_set_training_parameters(nnc , 0 , 1,1);
		nnc_train_network(nnc);
		nnc_classifier(nnc);
		nnc_free(nnc);
        return 0;
    case MODE_PRESET:
        switch (preset) {
        case PRESET_MCCULLOCH:
            nn = nn_read_from_file(P_MCCULLOCH_NEURAL);
            if (!nn) {
                printf("[ ERROR] Error reading neural network file.\n");
                return -1;
            }
            nn_set_function_neuron(nn , upd_neuron_mcculloch_pitts);
            values = (double*) malloc(3 * sizeof(double));
            if (input_file) {
                f_in = fopen(input_file, "r");
            } else {
                f_in = fopen(P_MCCULLOCH_INPUT,"r");
            }
            if (!f_in) {
                printf("[ ERROR] Error opening input file.\n");
                nn_free(nn);
                return -1;
            }
            if (output_file) {
                f_out = fopen(output_file, "w");
            } else {
                f_out = fopen(P_MCCULLOCH_OUTPUT ,"w");
            }
            if (!f_out) {
                printf("[ ERROR] Error opening output file.\n");
                fclose(f_in);
                nn_free(nn);
                return -1;
            }
            
            while(fgets(text,20,f_in)){
                ptr = strtok(text,SEP);
                values[0] = atof(ptr);
                for(j=1 ; j < 3; j++){
                    ptr = strtok(NULL,SEP);
                    values[j] = atof(ptr); 
                }
                nn_update_neurons(nn, values, 3, 1,0);
                fprint_output(nn, f_out);
            }

            nn_free(nn);
            fclose(f_in);
            fclose(f_out);
            
            break;
        }
        return 0;
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
