#ifndef _READ_FILE_H_
#define _READ_FILE_H_

#include <vector>
#include <pthread.h>

class FileReader;

class Param{
public:
  char* filename;
  int page_size;
  volatile int* waiting;
  FileReader* fr;
} ;

class FileReader{

public:
  void read_file(char* filename);
  void read_page(char* filename, int page_size);
  void display_buffer();
  FileReader();
  ~FileReader();
  //FileReader():thread_created(0), waiting(0){};
  int thread_created; 
  volatile int waiting;
  pthread_t page_thread;
  Param param;
  std::vector<int> a;
  std::vector<int> b;
};



#endif
