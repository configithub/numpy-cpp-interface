#include <iostream>
#include <stdlib.h>
#include "src/file_reader.h"


// read one sample file page
void read_sample_page(void* file_readerv, int*& a, int*& b, int& size) {
  FileReader* file_reader = (FileReader*) file_readerv;
  file_reader->read_page("files/sample.csv", 7);

  a = &(file_reader->a[0]);
  b = &(file_reader->b[0]);
  size = file_reader->a.size();
}
