#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/stat.h>

class FIO {
  public:
    std::string folderName = "output";
    FIO(); // Constructor
    int createFile(std::string);
    int write2file(std::string, int, float);

  private:
    std::string appendFolder(std::string);
};