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
  void read_page(char* filename, int page_size);
  void display_buffer();
  FileReader();
  ~FileReader();
  // to know at which step of the process we are :
  // first call : thread_created = 0
  // second to nth call : thread_created = 1
  // file finished reading : thread_created = 2
  int thread_created;  
  // to know if we the user thread is waiting for the reader thread
  volatile int waiting;
  // the thread on which we call the read_page static function
  pthread_t page_thread; 
  Param param;
  std::vector<int> a;
  std::vector<int> b;
};



#endif
