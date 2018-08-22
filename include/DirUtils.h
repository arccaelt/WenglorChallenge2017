#ifndef DIRUTILS_H
#define DIRUTILS_H

#include <windows.h>
#include <vector>
#include <string>
#include <iostream>

class DirUtils
{
public:
    static bool dirExits(std::string path);
    static void dirCreate(std::string path);
    static std::string getcwd();
    static std::vector<std::string> getFiles(std::string path);
};

#endif // DIRUTILS_H
