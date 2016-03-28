#include <stdlib.h>
#include <stdio.h>

// build a hardcoded c array 
float* c_array(float constant, int size) {

  float* array;
  array = malloc(sizeof(float)*size);

  int i;
  for (i = 0; i < size; i++) {
    array[i] = constant;
    // printf("%f\n", array[i]);
  }
  return array;
}
