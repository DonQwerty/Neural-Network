#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "data.h"
#include "classifier.h"
#include "neural_network_functions.h"

/* Global definitions, for options processing */
/* Execution modes */
#define MODE_PERCEPTRON   0
#define MODE_ADELINE      1
#define MODE_PRESET       2
/* Presets */
#define PRESET_MCCULLOCH  0

/* Preset files */
static const char P_MCCULLOCH_NEURAL[]      = "data/models/McCulloch-Pitts.txt";
static const char P_MCCULLOCH_INPUT[]       = "data/databases/McCulloch-Pitts.txt";
static const char P_MCCULLOCH_OUTPUT[]      = "out/McCulloch-Pitts.txt";


/* Global variables for options processing */
int mode            = -1;
int preset          = -1;
int max_epochs      = -1;
char * neural_file  = NULL;
char * input_file   = NULL;
char * output_file  = NULL;
int predict_flag = 0;
int save_flag = 0;
int percen = -1;

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

    /* PRESET MODE */
    if (mode == MODE_PRESET) {
        switch (preset) {
        case PRESET_MCCULLOCH:
            if (neural_file) {
                nn = nn_read_from_file(neural_file);
            } else {
                nn = nn_read_from_file(P_MCCULLOCH_NEURAL);
            }
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
    } else {
        /* STANDARD MODE */
        /* Read data */

        data = data_from_file(input_file, 1);
        if (!data) {
            printf("[ ERROR] Error reading input file.\n");
            return -1;
        }
        n_attrs = sample_get_n_attrs(*(data_get_samples(*data)[0]));
        n_clases = data_get_n_classes(*data);
        /* Create network */
        if (neural_file && save_flag==0) {
            nn = nn_read_from_file(neural_file);
        } else {
            nn = nn_init(n_attrs, n_clases,2, 1 ,1);
        }
        if (!nn) {
            printf("[ ERROR] Error reading neural network file.\n");
            return -1;
        }
        
        /* Create Classifier */
        nnc = nnc_new(output_file);
        nnc_set_data(nnc, data, predict_flag, percen);
        nnc_set_neural_network(nnc, nn);

        /* Assign mode */
        switch (mode) {
        case MODE_PERCEPTRON:
            nn_set_function_neuron(nn , upd_neuron_perceptron);
            nn_set_function_weight(nn , upd_weights_perceptron);
            nnc_set_training_parameters(nnc, 0, 1, 0);
            break;
        case MODE_ADELINE:
            nn_set_function_neuron(nn , upd_neuron_adeline);
            nn_set_function_weight(nn , upd_weights_adeline);
            nnc_set_training_parameters(nnc, 0, 1, 1);
            break;
        }
        fflush(stdout);
        if(max_epochs!=-1){
            nnc_set_stopping_conditions(nnc,max_epochs,0,0);
        }
        if (!predict_flag)
            /* No predictions file. Train network */
            nnc_train_network(nnc);
        if (save_flag)
            nn_save_to_file(nn, neural_file);
        nnc_classifier(nnc, predict_flag);
        if(!predict_flag) nnc_print_info(nnc);
        nnc_free(nnc);
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
                {"predict",    no_argument,  0, 'f'},
                {"train-percent",    required_argument,  0, 't'},
                {"save-file",    no_argument,  0, 's'},
                {0, 0, 0, 0}
            };

        int option_index = 0;

        c = getopt_long (argc, argv, "m:n:i:o:p:ft:s", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 'm':
            printf("[ INFO ] Mode: %s\n", optarg);
            if (!strcmp(optarg, "perceptron")) {
                mode = MODE_PERCEPTRON;
            } else if (!strcmp(optarg, "adaline")) {
                mode = MODE_ADELINE;
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
        case 'f':
            printf("[ INFO ] Prediction mode: predictions written to output_file\n");
            predict_flag = 1;
            break;
        case 'e':
            max_epochs = atoi(optarg);
            break;
        case 't':
            percen = atoi(optarg);
            break;
        case 's':
            save_flag = 1;
            break;
        case 'p':
            printf("[ INFO ] Preset: %s\n", optarg);
            mode = MODE_PRESET;
            if (!strcmp(optarg, "macculloch")) {
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
    printf("[ INFO ] Usage:\n");
    printf("             neural-network -m MODE -i INPUT -o OUTPUT    [-n NETWORK -s -t PERCEN -e EPOCHS -f]\n");
    printf("             neural-network -p PRESET [-n NETWORK -i INPUT -o OUTPUT]\n");
    printf("         Options:\n");
    printf("             -m, --mode:           Neuron mode [perceptron, adaline].\n");
    printf("             -n, --neural-network: File with network description.\n");
    printf("             -i, --input-file:     File with the inputs (or data) as rows.\n");
    printf("             -o, --ouput-file:     File to write the output of the network.\n");
    printf("             -e, --max-epochs:     Maximum number of epochs to train.\n");
    printf("             -f, --predict:   File to write the predictions [Activates predict mode].\n");
    printf("             -p, --preset:         Load a predefined network [macculloch].\n");
    printf("             -s, --save:        Writes the resulting network to NETWORK file.\n");
    printf("             -t, --percent:         Percen of train.\n");

}