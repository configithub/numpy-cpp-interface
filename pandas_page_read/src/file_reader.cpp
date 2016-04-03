#include "file_reader.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>


void* read_paget(void* arg) {
  Param* param;
  param = (Param*) arg;
  std::ifstream str_file(param->filename);
  std::string line;
  int idx = 0;
  std::vector<int> a;
  a.reserve(param->page_size+1); // HERE POSSIBLE BUG IF VECTOR ARE REALLOCATED
  std::vector<int> b;
  b.reserve(param->page_size+1);
  //a.reserve(100);
  //b.reserve(100);
  volatile int& waiting_ref = *(param->waiting);
  while(std::getline(str_file,line)) {

    std::stringstream lineStream(line);
    std::string cell;

    std::getline(lineStream,cell,',');
    a.push_back(atoi(cell.c_str()));
    std::getline(lineStream,cell,',');
    b.push_back(atoi(cell.c_str()));
    ++idx;
    if(idx%param->page_size==0) {
      //std::cout << "page finished" << std::endl;
      // swap buffers and wait
      param->fr->a.clear();
      param->fr->a.swap(a);
      a.clear();
      param->fr->b.clear();
      param->fr->b.swap(b);
      b.clear();
      waiting_ref = 1;
      while(waiting_ref) {
        // //std::cout << "waiting in thread" << std::endl; // <<< inifite loop here now
      }
    }
  }
  param->fr->a.clear();
  param->fr->a.swap(a);
  param->fr->b.clear();
  param->fr->b.swap(b);
  param->fr->thread_created=2;
  waiting_ref = 1;
  return NULL;
}


FileReader::FileReader() {
  thread_created = 0;
  waiting = 0;
}
FileReader::~FileReader() {
}

void FileReader::read_page(char* filename, int page_size) {
  //std::cout << "thread creation : " << thread_created << " ptr: " << this << std::endl;
  if(thread_created==0) { 
    //std::cout << "inside if for t creation" << thread_created << std::endl;
    thread_created = 1;
    param.filename = filename;
    param.page_size = page_size;
    a.reserve(page_size+1);
    b.reserve(page_size+1);
    waiting = 0;
    param.waiting = &waiting;
    param.fr = this;
    void* param_ptr = &param;
    int threadid = pthread_create(&page_thread, NULL, read_paget, param_ptr);
    //std::cout << "thread created: " << threadid << std::endl;
    while(waiting == 0) {
      // //std::cout << "waiting in function" << std::endl; 
    }
  }else if(thread_created==1){ // thread is already created and waiting
    waiting = 0;
    // wait for page thread to read one page
    while(waiting == 0) {
      // //std::cout << "waiting in function" << std::endl; 
    }
    return;
  }else{ // file finished
    a.clear(); b.clear();
    return;
  }
  
}


void FileReader::read_file(char* filename) {
  std::ifstream str_file(filename);
  std::string                line;
  while(std::getline(str_file,line)) {

    std::stringstream lineStream(line);
    std::string cell;

    std::getline(lineStream,cell,',');
    a.push_back(atoi(cell.c_str()));
    std::getline(lineStream,cell,',');
    b.push_back(atoi(cell.c_str()));
  }
}


void FileReader::display_buffer() {
  std::vector<int>::iterator itA = a.begin();
  std::vector<int>::iterator itB = b.begin();
  for(; itA != a.end(), itB != b.end(); ++itA, ++itB) {
    //std::cout << *itA << ", " << *itB << std::endl;
  }
}


void read_next_page(FileReader& file_reader) {
  file_reader.read_page("../files/sample.csv", 8);
  //std::cout << "read page:" << std::endl;
  file_reader.display_buffer();
}


int main(int argc, const char *argv[])
{
  //std::cout << "starting file read" << std::endl;
  FileReader file_reader; 
  read_next_page(file_reader);
  read_next_page(file_reader);
  read_next_page(file_reader);
  read_next_page(file_reader);
  read_next_page(file_reader);
  read_next_page(file_reader);
  read_next_page(file_reader);
  read_next_page(file_reader);
  return 0;
}
