#include <stdlib.h>
#include <stdio.h>
#include <tgmath.h>
#include <SDL/SDL.h>

// build a hardcoded c array 
float* c_array(float constant, int size) {

  float* array;
  array = malloc(sizeof(float)*size);

  int i;
  for (i = 0; i < size; i++) {
    array[i] = cos(constant);
    // printf("%f\n", array[i]);
  }
  return array;
}
