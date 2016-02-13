#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "neural_network.h"

int main(){
    printf("Hello world\n");
    Data * d = data_from_file("../data/databases/problema_real1.txt");
    data_free(d);
    return 1;
}
