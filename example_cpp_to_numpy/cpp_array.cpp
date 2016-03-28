#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

// build a hardcoded cpp array 
float* c_array(float constant, int size) {

  float* array;
  array = new float[size];
  // array = malloc(sizeof(float)*size);

  int i;
  for (i = 0; i < size; i++) {
    array[i] = constant;
    // printf("%f\n", array[i]);
  }
  return array;
}
