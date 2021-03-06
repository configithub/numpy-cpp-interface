#include "file_reader.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t read_mutex;
pthread_cond_t page_finished_cv;
pthread_cond_t next_page_requested_cv;

void* read_page_t(void* arg) {
  Param* param = (Param*) arg;
  std::ifstream str_file(param->filename);
  std::string line;
  int idx = 0;
  std::vector<int> a;
  a.reserve(param->page_size+1); 
  std::vector<int> b;
  b.reserve(param->page_size+1);
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
      // we have one page ready to swap
      // swap buffers and wait
      pthread_mutex_lock(&read_mutex);
      param->fr->a.clear();
      param->fr->a.swap(a);
      a.clear();
      param->fr->b.clear();
      param->fr->b.swap(b);
      b.clear();
      waiting_ref = 1;
      // wake up user thread
      pthread_cond_signal(&page_finished_cv);
      pthread_mutex_unlock(&read_mutex);
      // wait for next page request from user thread
      pthread_mutex_lock(&read_mutex);
      while(waiting_ref) {
        pthread_cond_wait(&next_page_requested_cv, &read_mutex);
        std::cout << "waiting in thread" << std::endl; 
      }
      pthread_mutex_unlock(&read_mutex);
    }
  }
  // last page ready
  pthread_mutex_lock(&read_mutex);
  param->fr->a.clear();
  param->fr->a.swap(a);
  param->fr->b.clear();
  param->fr->b.swap(b);
  param->fr->thread_created=2;
  waiting_ref = 1;
  // wake up user thread
  pthread_cond_signal(&page_finished_cv);
  pthread_mutex_unlock(&read_mutex);
  pthread_exit(NULL); // reader thread finished execution
}


FileReader::FileReader() {
  thread_created = 0;
  waiting = 0;
}

FileReader::~FileReader() {}

void FileReader::read_page(char* filename, int page_size) {
  if(thread_created==0) { // reader thread has to be created
    thread_created = 1;
    pthread_mutex_init(&read_mutex, NULL);
    pthread_cond_init (&page_finished_cv, NULL);
    pthread_cond_init (&next_page_requested_cv, NULL);
    param.filename = filename;
    param.page_size = page_size;
    a.reserve(page_size+1);
    b.reserve(page_size+1);
    waiting = 0;
    param.waiting = &waiting;
    param.fr = this;
    void* param_ptr = &param;
    // create reader thread
    int threadid = pthread_create(&page_thread, NULL, read_page_t, param_ptr);
    // wait for reader thread to read one page
    pthread_mutex_lock(&read_mutex);
    while(waiting == 0) {
      pthread_cond_wait(&page_finished_cv, &read_mutex);
    } 
    pthread_mutex_unlock(&read_mutex);
    return;
  }else if(thread_created==1){ // thread is already created and waiting
    waiting = 0;
    // wake up existing reader thread to make it read a page
    pthread_mutex_lock(&read_mutex);
    pthread_cond_signal(&next_page_requested_cv);
    pthread_mutex_unlock(&read_mutex);
    // wait for reader thread to read one page
    pthread_mutex_lock(&read_mutex);
    while(waiting == 0) {
      pthread_cond_wait(&page_finished_cv, &read_mutex);
      std::cout << "waiting in function" << std::endl; 
    }
    pthread_mutex_unlock(&read_mutex);
    return;
  }else{ // file finished, nothing more to be done
    a.clear(); b.clear();
    return;
  }
  
}


void FileReader::display_buffer() {
  std::vector<int>::iterator itA = a.begin();
  std::vector<int>::iterator itB = b.begin();
  for(; itA != a.end(), itB != b.end(); ++itA, ++itB) {
    std::cout << *itA << ", " << *itB << std::endl;
  }
}


void read_next_page(FileReader& file_reader) {
  file_reader.read_page("../files/sample.csv", 8);
  file_reader.display_buffer();
}


// for testing purpose in a pure c++ executable
int main(int argc, const char *argv[])
{
  std::cout << "starting file read" << std::endl;
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
