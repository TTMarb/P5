#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <chrono>
#include <sys/stat.h>

#include <ctime>
#include <iomanip>

class FIO {
  public:
    std::string folderName = "output";
    FIO(); // Constructor
    void changeActiveFile(std::string);
    int createFile();
    int write2file(std::string);

  private:
    std::string activeFile;
    std::string prependFolderToFilename(std::string);
    std::string getTimeStamp();
};