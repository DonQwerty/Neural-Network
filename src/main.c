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
#define MODE_MULTILAYER   3
#define MODE_AUTOENCODER  4
/* Presets */
#define PRESET_MCCULLOCH  0

/* Preset files */
static const char P_MCCULLOCH_NEURAL[]      = "data/models/McCulloch-Pitts.txt";
static const char P_MCCULLOCH_INPUT[]       = "data/databases/McCulloch-Pitts.txt";
static const char P_MCCULLOCH_OUTPUT[]      = "out/McCulloch-Pitts.txt";

#define MAX_LEN_STR 50

/* Global variables for options processing */
/* Variables */
int mode            = -1;
int preset          = -1;
int max_epochs      = -1;
int percen          = -1;
int hidden_neurons  =  0;
int n_layers        =  2;
double learning_rate = 0;
int unique_neuron   =  0;
int normalize       =  0;
/* Flags */
int predict_flag    = 0;
int save_flag       = 0;
/* Files */
char neural_file[MAX_LEN_STR]  = "no_file";
char input_file[MAX_LEN_STR]   = "no_file";
char output_file[MAX_LEN_STR]  = "/dev/null";
char stats_file[MAX_LEN_STR]   = "out/stats.csv";

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
            if (strcmp("no_file", neural_file) != 0) {
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
            if (strcmp("no_file", input_file) != 0) {
                f_in = fopen(input_file, "r");
            } else {
                f_in = fopen(P_MCCULLOCH_INPUT,"r");
            }
            if (!f_in) {
                printf("[ ERROR] Error opening input file.\n");
                nn_free(nn);
                return -1;
            }
            if (strcmp("no_file", output_file) != 0) {
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

        data = data_from_file(input_file, unique_neuron);
        if (!data) {
            printf("[ ERROR] Error reading input file.\n");
            return -1;
        }
        n_attrs = sample_get_n_attrs(*(data_get_samples(data)[0]));
        n_clases = data_get_n_classes(*data);
        /* Create network */
        if ((strcmp("no_file", neural_file) != 0) && save_flag == 0) {
            nn = nn_read_from_file(neural_file);
        } else {
            nn = nn_init(n_attrs, n_clases, n_layers, 1, 1, unique_neuron, hidden_neurons);
        }
        if (!nn) {
            printf("[ ERROR] Error reading neural network file.\n");
            data_free(data);
            return -1;
        }

        /* Create Classifier */
        nnc = nnc_new(output_file, stats_file, normalize);
        nnc_set_neural_network(nnc, nn);
        nnc_set_data(nnc, data, predict_flag, percen);


        /* Assign mode */
        switch (mode) {
        case MODE_PERCEPTRON:
            nn_set_function_neuron(nn , upd_neuron_perceptron);
            nn_set_function_weight(nn , upd_weights_perceptron);
            nnc_set_training_parameters(nnc, learning_rate, 1, 0, unique_neuron);
            break;
        case MODE_ADELINE:
            nn_set_function_neuron(nn , upd_neuron_adeline);
            nn_set_function_weight(nn , upd_weights_adeline);
            nnc_set_training_parameters(nnc, learning_rate, 1, 1, unique_neuron);
            break;
        case MODE_AUTOENCODER:
            /* TODO Hacer */
            // Poner a 1 el encoder_flag
        case MODE_MULTILAYER:
            nn_set_function_neuron(nn, upd_neuron_sigmoid);
            nn_set_function_weight(nn, upd_weights_sigmoid);
            nnc_set_training_parameters(nnc, learning_rate, 1, 1, unique_neuron);
            break;
        }
        fflush(stdout);
        if(max_epochs != -1){
            nnc_set_stopping_conditions(nnc, max_epochs, 0, 0);
        }
        if (!predict_flag)
            /* No predictions file. Train network */
            nnc_train_network(nnc);
        if (save_flag){
            if(normalize)
                nn_save_to_file(nn, neural_file, n_attrs);
            else
                nn_save_to_file(nn, neural_file, 0);
        }
        nnc_classifier(nnc, predict_flag);
        nnc_print_info(nnc, predict_flag);
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
                {"stats-file",      required_argument,  0, 'c'},
                {"preset",          required_argument,  0, 'p'},
                {"hidden-neurons",  required_argument,  0, 'h'},
                {"max-epochs",  required_argument,  0, 'e'},
                {"predict",         no_argument,        0, 'f'},
                {"train-percent",   required_argument,  0, 't'},
                {"learning-rate",   required_argument,  0, 'l'},
                {"save-file",       no_argument,        0, 's'},
                {"normalize",       no_argument,        0, 'z'},
                {"help",            no_argument,        0, 'k'},
                {0, 0, 0, 0}
            };

        int option_index = 0;

        c = getopt_long (argc, argv, "m:n:i:o:p:ft:skl:c:h:e:z", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 'm':
            printf("[ INFO ] Mode: %s\n", optarg);
            if (!strcmp(optarg, "perceptron")) {
                mode = MODE_PERCEPTRON;
                unique_neuron = 1;
            } else if (!strcmp(optarg, "adaline")) {
                mode = MODE_ADELINE;
                unique_neuron = 1;
            } else if (!strcmp(optarg, "multilayer")) {
                mode = MODE_MULTILAYER;
            } else {
                printf("[ ERROR] Unrecogniced mode: %s\n", optarg);
                return -1;
            }
            break;
        case 'n':
            printf("[ INFO ] Neural Network: %s\n", optarg);
            strncpy(neural_file, optarg, MAX_LEN_STR);
            break;
        case 'i':
            printf("[ INFO ] Input file: %s\n", optarg);
            strncpy(input_file, optarg, MAX_LEN_STR);
            break;
        case 'o':
            printf("[ INFO ] Output file: %s\n", optarg);
            strncpy(output_file, optarg, MAX_LEN_STR);
            break;
        case 'c':
            printf("[ INFO ] Stats file: %s\n", optarg);
            strncpy(stats_file, optarg, MAX_LEN_STR);
            break;
        case 'f':
            printf("[ INFO ] Prediction mode: predictions written to output_file\n");
            predict_flag = 1;
            break;
        case 'e':
            max_epochs = atoi(optarg);
            break;
        case 'z':
            normalize = 1;
            break;
        case 't':
            percen = atoi(optarg);
            break;
        case 'l':
            learning_rate = atof(optarg);
            break;
        case 'h':
            hidden_neurons = atoi(optarg);
            if(hidden_neurons > 0){
                n_layers++;
            }
            break;
        case 's':
            save_flag = 1;
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
        case 'k':
            print_help();
            exit(0);
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
    if ( (mode == MODE_MULTILAYER) && (hidden_neurons < 0)) {
        printf("[ WARN ] Mode set to multilayer but no neurons in hidden layer set.");
    }
    return 0;
}

void print_help() {
    printf("[ INFO ] Usage:\n");
    printf("             neural-network -m MODE -i INPUT -o OUTPUT    [-n NETWORK -s -t PERCEN -e EPOCHS -f]\n");
    printf("             neural-network -p PRESET                     [-n NETWORK -i INPUT -o OUTPUT]\n");
    printf("         Options:\n");
    printf("             -m, --mode:           Neuron mode [perceptron, adaline, multilayer].\n");
    printf("             -n, --neural-network: File with network description.\n");
    printf("             -i, --input-file:     File with the inputs (or data) as rows.\n");
    printf("             -o, --ouput-file:     File to write the output of the network.\n");
    printf("             -c, --stats-file:     File to write the stats of the network.\n");
    printf("             -h, --hidden-neurons: Number of neurons in the hidden layer.\n");
    printf("             -e, --max-epochs:     Maximum number of epochs to train.\n");
    printf("             -f, --predict:        Activates the predict mode.\n");
    printf("             -z, --normalize:      Normalizes the data.\n");
    printf("             -p, --preset:         Load a predefined network [mcculloch].\n");
    printf("             -s, --save:           Writes the resulting network to NETWORK file.\n");
    printf("             -t, --train-percent:  Percent of train (integer value in range 0-100).\n");
    printf("             -l, --learning-rate:  Learning rate (double value in range 0-1).\n");
    printf("             -h, --help:           Displays this help.\n");

}
