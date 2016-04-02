#include "file_reader.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>


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
    std::cout << *itA << ", " << *itB << std::endl;
  }
}


int main(int argc, const char *argv[])
{
  std::cout << "starting file read" << std::endl;
  FileReader file_reader; 
  file_reader.read_file("../files/sample.csv");
  file_reader.display_buffer();
  return 0;
}
