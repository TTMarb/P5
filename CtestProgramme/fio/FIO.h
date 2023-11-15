#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/stat.h>

class FIO {
  public:
    std::string folderName = "output";
    FIO(); // Constructor
    void changeActiveFile(std::string);
    int createFile();
    int write2file(int, float);

  private:
    std::string activeFile;
    std::string prependFolderToFilename(std::string);
};