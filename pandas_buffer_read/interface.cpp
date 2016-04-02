#include <iostream>
#include <stdlib.h>
#include "src/file_reader.h"

// read sample file 
void read_sample(int*& a, int*& b, int& size) {

  FileReader* file_reader = new FileReader();
  file_reader->read_file("files/sample.csv");

  a = &(file_reader->a[0]);
  b = &(file_reader->b[0]);
  size = file_reader->a.size();
}
