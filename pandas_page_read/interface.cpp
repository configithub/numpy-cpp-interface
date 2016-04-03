#include <iostream>
#include <stdlib.h>
#include "src/file_reader.h"

// read sample file 
void read_sample_file(int*& a, int*& b, int& size) {

  FileReader* file_reader = new FileReader();
  file_reader->read_file("files/sample.csv");

  a = &(file_reader->a[0]);
  b = &(file_reader->b[0]);
  size = file_reader->a.size();
}

FileReader* fr_ptr = NULL;

FileReader* get_file_reader() {
  std::cout << "get_file_reader" << std::endl;
  if(fr_ptr == NULL){
    std::cout << "create file_reader" << std::endl;
    fr_ptr = new FileReader();
  }
  return fr_ptr;
}


// read sample file 
void read_sample_page(void* file_readerv, int*& a, int*& b, int& size) {
  FileReader* file_reader = (FileReader*) file_readerv;

  //FileReader* file_reader = get_file_reader();
  std::cout << "before file_reader->read_page"  << std::endl;
  file_reader->read_page("files/sample.csv", 7);
  std::cout << "after file_reader->read_page" << std::endl;

  a = &(file_reader->a[0]);
  b = &(file_reader->b[0]);
  size = file_reader->a.size();
  std::cout << "size" << size << std::endl;
}
