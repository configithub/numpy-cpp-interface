#ifndef _READ_FILE_H_
#define _READ_FILE_H_

#include <vector>


class FileReader{

public:
  void read_file(char* filename);
  void display_buffer();

  std::vector<int> a;
  std::vector<int> b;
};



#endif
