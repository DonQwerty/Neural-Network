#include <math.h>
#include "perceptron.h"


double perc_process(double input) {
  // TODO This an example with the sigmoid function
  return 1.0 / (1.0 + exp(- input));
}
